#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace abb_aurora {

class ABBAurora : public Component, public uart::UARTDevice {
 public:
  void set_global_state_sensor(text_sensor::TextSensor *global_state_sensor);
  void set_inverter_state_sensor(text_sensor::TextSensor *inverter_state_sensor);
  void set_dc_dc_channel_1_state_sensor(text_sensor::TextSensor *dc_dc_channel_1_state_sensor);
  void set_dc_dc_channel_2_state_sensor(text_sensor::TextSensor *dc_dc_channel_2_state_sensor);
  void set_alarms_sensor(text_sensor::TextSensor *alarms_sensor);
  void set_grid_voltage_sensor(sensor::Sensor *grid_voltage_sensor);
  void set_grid_current_sensor(sensor::Sensor *grid_current_sensor);
  void set_grid_power_sensor(sensor::Sensor *grid_power_sensor);
  void set_grid_frequency_sensor(sensor::Sensor *grid_frequency_sensor);
  void set_solar_voltage_sensor(sensor::Sensor *solar_voltage_sensor);
  void set_solar_current_sensor(sensor::Sensor *solar_current_sensor);
  void set_solar_power_sensor(sensor::Sensor *solar_power_sensor);
  void set_booster_voltage_sensor(sensor::Sensor *booster_voltage_sensor);
  void set_booster_midpoint_voltage_sensor(sensor::Sensor *booster_midpoint_voltage_sensor);
  void set_inverter_temperature_sensor(sensor::Sensor *inverter_temperature_sensor);
  void set_booster_temperature_sensor(sensor::Sensor *booster_temperature_sensor);
  void set_fan_speed_sensor(sensor::Sensor *fan_speed_sensor);

  void loop() override;

  void dump_config() override;

 protected:
  text_sensor::TextSensor *global_state_sensor_{nullptr};
  text_sensor::TextSensor *inverter_state_sensor_{nullptr};
  text_sensor::TextSensor *dc_dc_channel_1_state_sensor_{nullptr};
  text_sensor::TextSensor *dc_dc_channel_2_state_sensor_{nullptr};
  text_sensor::TextSensor *alarms_sensor_{nullptr};
  sensor::Sensor *grid_voltage_sensor_{nullptr};
  sensor::Sensor *grid_current_sensor_{nullptr};
  sensor::Sensor *grid_power_sensor_{nullptr};
  sensor::Sensor *grid_frequency_sensor_{nullptr};
  sensor::Sensor *solar_voltage_sensor_{nullptr};
  sensor::Sensor *solar_current_sensor_{nullptr};
  sensor::Sensor *solar_power_sensor_{nullptr};
  sensor::Sensor *booster_voltage_sensor_{nullptr};
  sensor::Sensor *booster_midpoint_voltage_sensor_{nullptr};
  sensor::Sensor *inverter_temperature_sensor_{nullptr};
  sensor::Sensor *booster_temperature_sensor_{nullptr};
  sensor::Sensor *fan_speed_sensor_{nullptr};
};
}  // namespace abb_aurora
}  // namespace esphome
