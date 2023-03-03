#include "abb_aurora.h"
#include "esphome/core/log.h"
#include "strings.h"
#include "packet_types.h"

namespace esphome {
namespace abb_aurora {

/// The tag to use for logging
static const char *const TAG = "abb_aurora.inverter";

void ABBAurora::set_global_state_sensor(text_sensor::TextSensor *global_state_sensor) {
  global_state_sensor_ = global_state_sensor;
}
void ABBAurora::set_grid_voltage_sensor(sensor::Sensor *grid_voltage_sensor) {
  grid_voltage_sensor_ = grid_voltage_sensor;
}
void ABBAurora::set_grid_current_sensor(sensor::Sensor *grid_current_sensor) {
  grid_current_sensor_ = grid_current_sensor;
}
void ABBAurora::set_grid_power_sensor(sensor::Sensor *grid_power_sensor) {  //
  grid_power_sensor_ = grid_power_sensor;
}
void ABBAurora::set_grid_frequency_sensor(sensor::Sensor *grid_frequency_sensor) {
  grid_frequency_sensor_ = grid_frequency_sensor;
}
void ABBAurora::set_solar_voltage_sensor(sensor::Sensor *solar_voltage_sensor) {
  solar_voltage_sensor_ = solar_voltage_sensor;
}
void ABBAurora::set_solar_current_sensor(sensor::Sensor *solar_current_sensor) {
  solar_current_sensor_ = solar_current_sensor;
}
void ABBAurora::set_solar_power_sensor(sensor::Sensor *solar_power_sensor) {  //
  solar_power_sensor_ = solar_power_sensor;
}
void ABBAurora::set_booster_voltage_sensor(sensor::Sensor *booster_voltage_sensor) {
  booster_voltage_sensor_ = booster_voltage_sensor;
}
void ABBAurora::set_booster_midpoint_voltage_sensor(sensor::Sensor *booster_midpoint_voltage_sensor) {
  booster_midpoint_voltage_sensor_ = booster_midpoint_voltage_sensor;
}
void ABBAurora::set_inverter_temperature_sensor(sensor::Sensor *inverter_temperature_sensor) {
  inverter_temperature_sensor_ = inverter_temperature_sensor;
}
void ABBAurora::set_booster_temperature_sensor(sensor::Sensor *booster_temperature_sensor) {
  booster_temperature_sensor_ = booster_temperature_sensor;
}
void ABBAurora::set_fan_speed_sensor(sensor::Sensor *fan_speed_sensor) {  //
  fan_speed_sensor_ = fan_speed_sensor;
}

void ABBAurora::loop() {
  uint32_t now = millis();
  if (!this->available() && (this->idle_ || now - this->last_request_time_ > this->timeout_)) {
    if ((now - this->last_request_time_) > this->timeout_) {
      ESP_LOGW(TAG, "Timed out waiting for response");
    }

    // Reset buffers and send the next packet in the sequence
    this->idx_ = 0;
    this->processors_idx_ = (this->processors_idx_ + 1) % this->processors_.size();
    const auto &request = processors_[this->processors_idx_].request_;

    // Log the request packet into space separated hex values
    ESP_LOGD(TAG, "Packet: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", request[0], request[1], request[2],
             request[3], request[4], request[5], request[6], request[7], request[8], request[9]);

    this->write_array(request, sizeof(request));
    this->idle_ = false;
    this->last_request_time_ = now;
    ESP_LOGVV(TAG, "Sent request packet to inverter");
  } else {
    // while (this->available()) {
    for (int i = 0; this->available() && i < sizeof(Response); ++i) {
      // Read into the buffer until we reach 10 bytes (all return packets are 6 bytes + crc)
      this->read_byte(this->buffer_ + this->idx_);
      this->idx_ += 1;

      if (this->idx_ == sizeof(Response)) {
        // We have a full packet, process it
        this->idx_ = 0;
        this->idle_ = true;

        Response *response = reinterpret_cast<Response *>(this->buffer_);

        // Check the CRC
        if (response->crc != crc(this->buffer_, sizeof(this->buffer_) - 2)) {
          ESP_LOGW(TAG, "Invalid CRC!");
          return;
        }

        // Check the transmit state
        if (response->transmit_state != 0x00) {
          ESP_LOGW(TAG, "Invalid transmit state!", transmission_state(response->transmit_state));
          return;
        }

        // Check and transmit the global state
        if (this->global_state_sensor_ != nullptr) {
          this->global_state_sensor_->publish_state(global_state(response->global_state));
        }

        // Send the data
        auto &sensor = processors_[this->processors_idx_].sensor_;
        float v = *reinterpret_cast<const float *>(response->data_);
        sensor->publish_state(v);
      }
    }
  }
}

void ABBAurora::setup() {
  // Go through each sensor and make a packet processor for it
  auto create_process = [&](sensor::Sensor *ptr, uint8_t measure_type) {
    if (this->*ptr != nullptr) {
      this->processors_.emplace_back();
      auto &v = this->processors_.back();
      new (v.request_) RequestMeasure(this->address_, measure_type, true);
      v.sensor_ = ptr;
    }
  };

  create_process(grid_voltage_sensor_, 1);
  create_process(grid_current_sensor_, 2);
  create_process(grid_power_sensor_, 3);
  create_process(grid_frequency_sensor_, 4);
  create_process(solar_voltage_sensor_, 23);
  create_process(solar_current_sensor_, 25);
  create_process(solar_power_sensor_, 8);
  create_process(booster_voltage_sensor_, 5);
  create_process(booster_midpoint_voltage_sensor_, 33);
  create_process(inverter_temperature_sensor_, 21);
  create_process(booster_temperature_sensor_, 22);
  create_process(fan_speed_sensor_, 54);
}

void ABBAurora::dump_config() {  // NOLINT(readability-function-cognitive-complexity)
  ESP_LOGCONFIG(TAG, "ABBAurora:");

  LOG_TEXT_SENSOR("  ", "Global State", this->global_state_sensor_);

  LOG_SENSOR("  ", "Grid Voltage", this->grid_voltage_sensor_);
  LOG_SENSOR("  ", "Grid Current", this->grid_current_sensor_);
  LOG_SENSOR("  ", "Grid Power", this->grid_power_sensor_);
  LOG_SENSOR("  ", "Grid Frequency", this->grid_frequency_sensor_);
  LOG_SENSOR("  ", "Solar Voltage", this->solar_voltage_sensor_);
  LOG_SENSOR("  ", "Solar Current", this->solar_current_sensor_);
  LOG_SENSOR("  ", "Solar Power", this->solar_power_sensor_);
  LOG_SENSOR("  ", "Booster Voltage", this->booster_voltage_sensor_);
  LOG_SENSOR("  ", "Booster Midpoint Voltage", this->booster_midpoint_voltage_sensor_);
  LOG_SENSOR("  ", "Inverter Temperature", this->inverter_temperature_sensor_);
  LOG_SENSOR("  ", "Booster Temperature", this->booster_temperature_sensor_);
  LOG_SENSOR("  ", "Fan Speed", this->fan_speed_sensor_);
}

}  // namespace abb_aurora
}  // namespace esphome
