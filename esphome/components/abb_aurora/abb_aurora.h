#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace abb_aurora {

class ABBAurora : public Component, public uart::UARTDevice {
 public:
  struct Response {
    uint8_t transmit_state;
    uint8_t global_state;
    uint8_t data_[4];
    uint16_t crc;
  } __attribute__((packed));

  void set_global_state_sensor(text_sensor::TextSensor *global_state_sensor);
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

  void setup() override;
  void loop() override;

  void dump_config() override;

  uint8_t address_ = 2;
  uint32_t timeout_ = 100;
  uint32_t last_request_time_ = 0;
  bool idle_ = true;

  int idx_ = 0;
  uint8_t buffer_[8];

  struct Processor {
    using Callback = std::function<void(const Response &response)>;
    uint8_t request_[10];  // All requests are 8 bytes + crc
    Callback process_;
  };

  std::vector<Processor> processors_;
  int processors_idx_ = 0;

  text_sensor::TextSensor *global_state_sensor_{nullptr};
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
