#include "goodwe.h"
#include "esphome/core/log.h"

namespace esphome {
namespace goodwe {

/// The tag to use for logging
static const char *const TAG = "goodwe.inverter";

/// The packet to send to the inverter to request the current data
const uint8_t request_packet[9] = {0xaa, 0x55, 0xab, 0x7f, 0x01, 0x01, 0x00, 0x02, 0x2b};

uint16_t swap_endian(const uint16_t &v) {
  // Swap endian
  return (v >> 8) | (v << 8);
}
uint32_t swap_endian(const uint32_t &v) {
  // Swap endian
  return (v >> 24) | ((v << 8) & 0x00FF0000) | ((v >> 8) & 0x0000FF00) | (v << 24);
}

template<typename T, T denominator> struct Data {
  T data;
  operator float() { return float(swap_endian(data)) / float(denominator); }
} __attribute__((packed));

// Data is big endian
struct Response {
  uint8_t header_[2];  // Should be 0xAA 0x55
  uint8_t from_;
  uint8_t to_;
  uint8_t command_;
  uint8_t subcommand_;
  uint8_t length_;
  Data<uint16_t, 10> solar_string_1_voltage_;
  Data<uint16_t, 10> solar_string_2_voltage_;
  Data<uint16_t, 10> solar_string_1_current_;
  Data<uint16_t, 10> solar_string_2_current_;
  Data<uint16_t, 10> ac_grid_voltage_;
  Data<uint16_t, 10> ac_grid_current_;
  Data<uint16_t, 100> ac_grid_frequency_;
  Data<uint16_t, 1> ac_grid_power_;
  uint16_t work_mode_;
  Data<uint16_t, 10> temperature_;
  char unknown1_[4];
  Data<uint32_t, 10> lifetime_energy_total_;
  Data<uint32_t, 1> hours_total_;
  char unknown2_[12];
  uint16_t daily_energy_;
  char unkonwn3_[14];
  uint16_t checksum_;
} __attribute__((packed));

static_assert(sizeof(Response) == 69, "The response is the incorrect size");

void GoodWe::set_work_mode_sensor(text_sensor::TextSensor *work_mode_sensor) {
  this->work_mode_sensor_ = work_mode_sensor;
}
void GoodWe::set_grid_voltage_sensor(sensor::Sensor *grid_voltage_sensor) {
  this->grid_voltage_sensor_ = grid_voltage_sensor;
}
void GoodWe::set_grid_current_sensor(sensor::Sensor *grid_current_sensor) {
  this->grid_current_sensor_ = grid_current_sensor;
}
void GoodWe::set_grid_power_sensor(sensor::Sensor *grid_power_sensor) {  //
  this->grid_power_sensor_ = grid_power_sensor;
}
void GoodWe::set_grid_frequency_sensor(sensor::Sensor *grid_frequency_sensor) {
  this->grid_frequency_sensor_ = grid_frequency_sensor;
}
void GoodWe::set_solar_string_1_voltage_sensor(sensor::Sensor *solar_voltage_sensor) {
  this->solar_string_1_voltage_sensor_ = solar_voltage_sensor;
}
void GoodWe::set_solar_string_1_current_sensor(sensor::Sensor *solar_current_sensor) {
  this->solar_string_1_current_sensor_ = solar_current_sensor;
}
void GoodWe::set_solar_string_2_voltage_sensor(sensor::Sensor *solar_voltage_sensor) {
  this->solar_string_2_voltage_sensor_ = solar_voltage_sensor;
}
void GoodWe::set_solar_string_2_current_sensor(sensor::Sensor *solar_current_sensor) {
  this->solar_string_2_current_sensor_ = solar_current_sensor;
}
void GoodWe::set_temperature_sensor(sensor::Sensor *temperature_sensor) {
  this->temperature_sensor_ = temperature_sensor;
}

void GoodWe::loop() {
  // If we are idle or we timed out, send a request packet
  uint32_t now = millis();
  if (!this->available() && (this->idle_ || (now - this->last_request_time_) > this->timeout_)) {
    if ((now - this->last_request_time_) > this->timeout_) {
      ESP_LOGW(TAG, "Timed out waiting for response");
    }

    // Send the request packet and reset our read index
    this->write_array(request_packet, sizeof(request_packet));
    this->last_request_time_ = now;
    this->idle_ = false;
    this->idx_ = 0;
    ESP_LOGVV(TAG, "Sent request packet to inverter");
  } else {
    while (this->available()) {
      // Read the next byte until we have a full packet
      this->read_byte(this->buffer_ + this->idx_);
      this->idx_ += 1;

      // While we are reading bytes we don't time out
      this->last_request_time_ = now;

      // When we have a full packet, parse it
      if (this->idx_ == sizeof(Response)) {
        // Reset for next packet
        this->idx_ = 0;
        this->idle_ = true;

        // Check the header
        if (this->buffer_[0] != 0xAA || this->buffer_[1] != 0x55) {
          ESP_LOGW(TAG, "Invalid header in response");
          return;
        }

        // Check the checksum
        uint16_t checksum = 0;
        for (int i = 0; i < sizeof(Response) - 2; ++i) {
          checksum += this->buffer_[i];
        }
        if (checksum != swap_endian(((Response *) this->buffer_)->checksum_)) {
          ESP_LOGW(TAG, "Invalid checksum in response");
          return;
        }

        // Parse the response
        Response *response = reinterpret_cast<Response *>(this->buffer_);
        ESP_LOGVV(TAG, "Received response from inverter");

        // Check all the deduplications and if any have changed, send everything
        bool changed = false;
        changed |= this->work_mode_dedup_.next(response->work_mode_);
        changed |= this->grid_voltage_dedup_.next(response->ac_grid_voltage_);
        changed |= this->grid_current_dedup_.next(response->ac_grid_current_);
        changed |= this->grid_power_dedup_.next(response->ac_grid_power_);
        changed |= this->grid_frequency_dedup_.next(response->ac_grid_frequency_);
        changed |= this->solar_string_1_voltage_dedup_.next(response->solar_string_1_voltage_);
        changed |= this->solar_string_1_current_dedup_.next(response->solar_string_1_current_);
        changed |= this->solar_string_2_voltage_dedup_.next(response->solar_string_2_voltage_);
        changed |= this->solar_string_2_current_dedup_.next(response->solar_string_2_current_);
        changed |= this->temperature_dedup_.next(response->temperature_);
        if (!changed) {
          return;
        }

        // Work mode
        if (this->work_mode_sensor_ != nullptr) {
          switch (swap_endian(response->work_mode_)) {
            case 0:
              this->work_mode_sensor_->publish_state("Standby");
              break;
            case 1:
              this->work_mode_sensor_->publish_state("Running");
              break;
            default:
              this->work_mode_sensor_->publish_state("Error");
              break;
          }
        }

        // Grid voltage
        if (this->grid_voltage_sensor_ != nullptr) {
          this->grid_voltage_sensor_->publish_state(response->ac_grid_voltage_);
        }

        // Grid current
        if (this->grid_current_sensor_ != nullptr) {
          this->grid_current_sensor_->publish_state(response->ac_grid_current_);
        }

        // Grid power
        if (this->grid_power_sensor_ != nullptr) {
          this->grid_power_sensor_->publish_state(response->ac_grid_power_);
        }

        // Grid frequency
        if (this->grid_frequency_sensor_ != nullptr) {
          this->grid_frequency_sensor_->publish_state(response->ac_grid_frequency_);
        }

        // Solar string 1 voltage
        if (this->solar_string_1_voltage_sensor_ != nullptr) {
          this->solar_string_1_voltage_sensor_->publish_state(response->solar_string_1_voltage_);
        }

        // Solar string 1 current
        if (this->solar_string_1_current_sensor_ != nullptr) {
          this->solar_string_1_current_sensor_->publish_state(response->solar_string_1_current_);
        }

        // Solar string 2 voltage
        if (this->solar_string_2_voltage_sensor_ != nullptr) {
          this->solar_string_2_voltage_sensor_->publish_state(response->solar_string_2_voltage_);
        }

        // Solar string 2 current
        if (this->solar_string_2_current_sensor_ != nullptr) {
          this->solar_string_2_current_sensor_->publish_state(response->solar_string_2_current_);
        }

        // Temperature
        if (this->temperature_sensor_ != nullptr) {
          this->temperature_sensor_->publish_state(response->temperature_);
        }
      }
    }
  }
}

void GoodWe::dump_config() {  // NOLINT(readability-function-cognitive-complexity)
  ESP_LOGCONFIG(TAG, "GoodWe:");

  LOG_TEXT_SENSOR("  ", "Work Mode", this->work_mode_sensor_);

  LOG_SENSOR("  ", "Grid Voltage", this->grid_voltage_sensor_);
  LOG_SENSOR("  ", "Grid Current", this->grid_current_sensor_);
  LOG_SENSOR("  ", "Grid Power", this->grid_power_sensor_);
  LOG_SENSOR("  ", "Grid Frequency", this->grid_frequency_sensor_);
  LOG_SENSOR("  ", "Solar String 1 Voltage", this->solar_string_1_voltage_sensor_);
  LOG_SENSOR("  ", "Solar String 1 Current", this->solar_string_1_current_sensor_);
  LOG_SENSOR("  ", "Solar String 2 Voltage", this->solar_string_2_voltage_sensor_);
  LOG_SENSOR("  ", "Solar String 2 Current", this->solar_string_2_current_sensor_);
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
}

}  // namespace goodwe
}  // namespace esphome
