#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace goodwe {

class GoodWe : public Component, public uart::UARTDevice {
 public:
  void set_work_mode_sensor(text_sensor::TextSensor *work_mode_sensor);
  void set_grid_voltage_sensor(sensor::Sensor *grid_voltage_sensor);
  void set_grid_current_sensor(sensor::Sensor *grid_current_sensor);
  void set_grid_power_sensor(sensor::Sensor *grid_power_sensor);
  void set_grid_frequency_sensor(sensor::Sensor *grid_frequency_sensor);
  void set_solar_string_1_voltage_sensor(sensor::Sensor *solar_voltage_sensor);
  void set_solar_string_1_current_sensor(sensor::Sensor *solar_current_sensor);
  void set_solar_string_2_voltage_sensor(sensor::Sensor *solar_voltage_sensor);
  void set_solar_string_2_current_sensor(sensor::Sensor *solar_current_sensor);
  void set_temperature_sensor(sensor::Sensor *temperature_sensor);

  void loop() override;

  void dump_config() override;

 protected:
  uint32_t last_request_time_ = 0;
  bool idle_ = true;
  uint8_t buffer_[255];
  int idx_ = 0;

  // Timeout in milliseconds
  uint32_t timeout_ = 200;

  text_sensor::TextSensor *work_mode_sensor_{nullptr};
  sensor::Sensor *grid_voltage_sensor_{nullptr};
  sensor::Sensor *grid_current_sensor_{nullptr};
  sensor::Sensor *grid_power_sensor_{nullptr};
  sensor::Sensor *grid_frequency_sensor_{nullptr};
  sensor::Sensor *solar_string_1_voltage_sensor_{nullptr};
  sensor::Sensor *solar_string_1_current_sensor_{nullptr};
  sensor::Sensor *solar_string_2_voltage_sensor_{nullptr};
  sensor::Sensor *solar_string_2_current_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};

  Deduplicator<uint16_t> work_mode_dedup_;
  Deduplicator<float> grid_voltage_dedup_;
  Deduplicator<float> grid_current_dedup_;
  Deduplicator<float> grid_power_dedup_;
  Deduplicator<float> grid_frequency_dedup_;
  Deduplicator<float> solar_string_1_voltage_dedup_;
  Deduplicator<float> solar_string_1_current_dedup_;
  Deduplicator<float> solar_string_2_voltage_dedup_;
  Deduplicator<float> solar_string_2_current_dedup_;
  Deduplicator<float> temperature_dedup_;
};
}  // namespace goodwe
}  // namespace esphome
