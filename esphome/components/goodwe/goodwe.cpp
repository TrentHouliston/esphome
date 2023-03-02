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
  uint8_t header[2];  // Should be 0xAA 0x55
  uint8_t from;
  uint8_t to;
  uint8_t command;
  uint8_t subcommand;
  uint8_t length;
  Data<uint16_t, 10> solar_string_1_voltage;
  Data<uint16_t, 10> solar_string_2_voltage;
  Data<uint16_t, 10> solar_string_1_current;
  Data<uint16_t, 10> solar_string_2_current;
  Data<uint16_t, 10> ac_grid_voltage;
  Data<uint16_t, 10> ac_grid_current;
  Data<uint16_t, 100> ac_grid_frequency;
  Data<uint16_t, 1> ac_grid_power;
  uint16_t work_mode;
  Data<uint16_t, 10> temperature;
  char unknown1[4];
  Data<uint32_t, 10> lifetime_energy_total;
  Data<uint32_t, 1> hours_total;
  char unknown2[12];
  uint16_t daily_energy;
  char unkonwn3[14];
  uint16_t crc;
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
  if (!this->available()) {
    // Send the request packet and reset our read index
    this->write_array(request_packet, sizeof(request_packet));
    idx = 0;
    ESP_LOGVV(TAG, "Sent request packet to inverter");
  } else {
    while (this->available()) {
      // Read the next byte until we have a full packet
      this->read_byte(buffer + idx);
      idx += 1;

      // When we have a full packet, parse it
      if (idx == sizeof(Response)) {
        // Reset for next packet
        idx = 0;

        // Check the header
        if (buffer[0] != 0xAA || buffer[1] != 0x55) {
          ESP_LOGW(TAG, "Invalid header in response");
          return;
        }

        // Check the CRC
        uint16_t crc = 0;
        for (int i = 0; i < sizeof(Response) - 2; ++i) {
          crc += buffer[i];
        }
        if (crc != swap_endian(((Response *) buffer)->crc)) {
          ESP_LOGW(TAG, "Invalid CRC in response");
          return;
        }

        // Parse the response
        Response *response = reinterpret_cast<Response *>(buffer);
        ESP_LOGVV(TAG, "Received response from inverter");

        // Check all the deduplications and if any have changed, send everything
        bool changed = false;
        changed |= this->work_mode_dedup_.next(response->work_mode);
        changed |= this->grid_voltage_dedup_.next(response->ac_grid_voltage);
        changed |= this->grid_current_dedup_.next(response->ac_grid_current);
        changed |= this->grid_power_dedup_.next(response->ac_grid_power);
        changed |= this->grid_frequency_dedup_.next(response->ac_grid_frequency);
        changed |= this->solar_string_1_voltage_dedup_.next(response->solar_string_1_voltage);
        changed |= this->solar_string_1_current_dedup_.next(response->solar_string_1_current);
        changed |= this->solar_string_2_voltage_dedup_.next(response->solar_string_2_voltage);
        changed |= this->solar_string_2_current_dedup_.next(response->solar_string_2_current);
        changed |= this->temperature_dedup_.next(response->temperature);
        if (!changed) {
          return;
        }

        // Work mode
        if (this->work_mode_sensor_ != nullptr) {
          switch (swap_endian(response->work_mode)) {
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
          this->grid_voltage_sensor_->publish_state(response->ac_grid_voltage);
        }

        // Grid current
        if (this->grid_current_sensor_ != nullptr) {
          this->grid_current_sensor_->publish_state(response->ac_grid_current);
        }

        // Grid power
        if (this->grid_power_sensor_ != nullptr) {
          this->grid_power_sensor_->publish_state(response->ac_grid_power);
        }

        // Grid frequency
        if (this->grid_frequency_sensor_ != nullptr) {
          this->grid_frequency_sensor_->publish_state(response->ac_grid_frequency);
        }

        // Solar string 1 voltage
        if (this->solar_string_1_voltage_sensor_ != nullptr) {
          this->solar_string_1_voltage_sensor_->publish_state(response->solar_string_1_voltage);
        }

        // Solar string 1 current
        if (this->solar_string_1_current_sensor_ != nullptr) {
          this->solar_string_1_current_sensor_->publish_state(response->solar_string_1_current);
        }

        // Solar string 2 voltage
        if (this->solar_string_2_voltage_sensor_ != nullptr) {
          this->solar_string_2_voltage_sensor_->publish_state(response->solar_string_2_voltage);
        }

        // Solar string 2 current
        if (this->solar_string_2_current_sensor_ != nullptr) {
          this->solar_string_2_current_sensor_->publish_state(response->solar_string_2_current);
        }

        // Temperature
        if (this->temperature_sensor_ != nullptr) {
          this->temperature_sensor_->publish_state(response->temperature);
        }
      }
    }
  }
}

void GoodWe::setup() {}

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
