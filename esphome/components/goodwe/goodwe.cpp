#include "goodwe.h"
#include "esphome/core/log.h"

namespace esphome {
namespace goodwe {

static const char *const TAG = "goodwe.inverter";

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
  char unknown3[14];
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
void GoodWe::set_grid_power_sensor(sensor::Sensor *grid_power_sensor) { this->grid_power_sensor_ = grid_power_sensor; }
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
    // TODO send the packet
  } else {
    while (this->available()) {
      // TODO read into the buffer until we reach 8 bytes
    }
  }
  // TODO if not available send next packet

  // TODO read while available

  // DataPacket buffer;
  if (!this->available()) {
    return;
  }
  // if (read_array((uint8_t *) &buffer, sizeof(buffer))) {
  //   if (validate_checksum(&buffer)) {
  //     received_package_(&buffer);
  //   }
  // } else {
  //   ESP_LOGW(TAG, "Junk on wire. Throwing away partial message");
  //   while (read() >= 0)
  //     ;
  // }
}

void GoodWe::setup() {
  // TODO build all the packets to send
}

void GoodWe::dump_config() {  // NOLINT(readability-function-cognitive-complexity)
  ESP_LOGCONFIG(TAG, "ABBAurora:");

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
