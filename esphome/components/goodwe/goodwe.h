#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace abb_aurora {

class ABBAurora : public Component, public uart::UARTDevice {
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

  void setup() override;
  void dump_config() override;

 protected:
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
};
}  // namespace abb_aurora
}  // namespace esphome
