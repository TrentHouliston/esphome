#include "abb_aurora.h"
#include "esphome/core/log.h"

namespace esphome {
namespace abb_aurora {

uint16_t crc(void *data, int len) {
  uint8_t lo = 0xFF;
  uint8_t hi = 0xFF;

  for (int i = 0; i < len; i++) {
    uint8_t d = ((uint8_t *) data)[i];
    uint8_t n = (d ^ lo);
    uint8_t t = (n << 4);
    n = (t ^ n);
    t = (n >> 5);
    lo = hi;
    hi = (n ^ t);
    t = (n << 3);
    lo = (lo ^ t);
    t = (n >> 4);
    lo = (lo ^ t);
  }

  return ~lo & 0xFF, ~hi & 0xFF;
}

struct RequestState {
  char address_;
  char command = 50;
  char reserved_[6] = {0, 0, 0, 0, 0, 0};
  uint16_t crc_;
} __attribute__((packed));
// Response { tr_state, global_state, inverter_state, dc_dc1_state, dc_dc2_state, alarm_state, crc_l, crc_h}

struct RequestPN {
  char address_;
  char command = 52;
  char reserved_[6] = {0, 0, 0, 0, 0, 0};
  uint16_t crc_;
} __attribute__((packed));
// Response { char6, char5, char4, char3, char2, char1, crc_l, crc_h}

struct RequestVersion {
  char address_;
  char command = 58;
  char reserved_[6] = {0, 0, 0, 0, 0, 0};
  uint16_t crc_;
} __attribute__((packed));

// Response { trans_state, global_state, par1, par2, par3, par4, crc_l, crc_h}
/// Par 1 Indoor/Outdoor and type
// ‘i’ Aurora 2 kW indoor
// ‘o’ Aurora 2 kW outdoor
// ‘I’ Aurora 3.6 kW indoor
// ‘O’ Aurora 3.0-3.6 kW outdoor
// ‘5’ Aurora 5.0 kW outdoor
// ‘6’ Aurora 6 kW outdoor
// ‘P’ 3-phase interface (3G74)
// ‘C’ Aurora 50kW module
// ‘4’ Aurora 4.2kW new
// ‘3’ Aurora 3.6kW new
// ‘2’ Aurora 3.3kW new
// ‘1’ Aurora 3.0kW new
// ‘D’ Aurora 12.0kW
// ‘X’ Aurora 10.0kW
// Par 2 Grid Standard
// ‘A’ UL1741
// ‘E’ VDE0126
// ‘S’ DR 1663/2000
// ‘I’ ENEL DK 5950
// ‘U’ UK G83
// ‘K’ AS 4777
// Par 3 Trafo/Non Trafo
// ‘N’ Transformerless Version
// ‘T’ Transformer Version
// Par 4 Wind/PV
// ‘W’ Wind version
// ‘N’ PV version

struct RequestMeasure {
  char address_;
  char command = 59;
  char type_;
  char global_;
  char reserved_[4] = {0, 0, 0, 0};
  uint16_t crc_;
} __attribute__((packed));
// Response { trans_state, global_state, v_float, crc_l, crc_h}

struct RequestSerialNumber {
  char address_;
  char command = 63;
  char reserved_[6] = {0, 0, 0, 0, 0, 0};
  uint16_t crc_;
} __attribute__((packed));
// Response { char6, char5, char4, char3, char2, char1, crc_l, crc_h}

struct RequestManufacturingWeekAndYear {
  char address_;
  char command = 65;
  char reserved_[6] = {0, 0, 0, 0, 0, 0};
  uint16_t crc_;
} __attribute__((packed));
// Response { TR.State, GlobalState, week2, week1, year2, year1, crc_l, crc_h}

void ABBAurora::set_global_state_sensor(text_sensor::TextSensor *global_state_sensor) {
  global_state_sensor_ = global_state_sensor;
}
void ABBAurora::set_inverter_state_sensor(text_sensor::TextSensor *inverter_state_sensor) {
  inverter_state_sensor_ = inverter_state_sensor;
}
void ABBAurora::set_dc_dc_channel_1_state_sensor(text_sensor::TextSensor *dc_dc_channel_1_state_sensor) {
  dc_dc_channel_1_state_sensor_ = dc_dc_channel_1_state_sensor;
}
void ABBAurora::set_dc_dc_channel_2_state_sensor(text_sensor::TextSensor *dc_dc_channel_2_state_sensor) {
  dc_dc_channel_2_state_sensor_ = dc_dc_channel_2_state_sensor;
}
void ABBAurora::set_alarms_sensor(text_sensor::TextSensor *alarms_sensor) { alarms_sensor_ = alarms_sensor; }

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
  if (!this->available()) {
    // TODO send the packet
  } else {
    while (this->available()) {
      // TODO read into the buffer until we reach 8 bytes
    }
  }
}

void ABBAurora::dump_config() {  // NOLINT(readability-function-cognitive-complexity)
  ESP_LOGCONFIG(TAG, "ABBAurora:");

  LOG_TEXT_SENSOR("  ", "Global State", this->global_state_sensor_);
  LOG_TEXT_SENSOR("  ", "Inverter State", this->inverter_state_sensor_);
  LOG_TEXT_SENSOR("  ", "DC/DC Channel 1 State", this->dc_dc_channel_1_state_sensor_);
  LOG_TEXT_SENSOR("  ", "DC/DC Channel 2 State", this->dc_dc_channel_2_state_sensor_);
  LOG_TEXT_SENSOR("  ", "Alarms", this->alarms_sensor_);

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
