#pragma once

#include <cstdint>

namespace esphome {
namespace abb_aurora {

uint16_t crc(const void *data, int len) {
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

  return (~hi & 0xFF << 8) | (~lo & 0xFF);
}

struct RequestState {
  char address_;
  char command = 50;
  char reserved_[6] = {0, 0, 0, 0, 0, 0};
  uint16_t crc_;
  RequestState(char address) : address_(address) { crc_ = crc(this, sizeof(RequestState) - 2); }
} __attribute__((packed));
// Response { tr_state, global_state, inverter_state, dc_dc1_state, dc_dc2_state, alarm_state, crc_l, crc_h}

struct RequestPN {
  char address_;
  char command = 52;
  char reserved_[6] = {0, 0, 0, 0, 0, 0};
  uint16_t crc_;
  RequestPN(char address) : address_(address) { crc_ = crc(this, sizeof(RequestPN) - 2); }
} __attribute__((packed));
// Response { char6, char5, char4, char3, char2, char1, crc_l, crc_h}

struct RequestVersion {
  char address_;
  char command = 58;
  char reserved_[6] = {0, 0, 0, 0, 0, 0};
  uint16_t crc_;
  RequestVersion(char address) : address_(address) { crc_ = crc(this, sizeof(RequestVersion) - 2); }
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
  RequestMeasure(char address, char type, bool global) : address_(address), type_(type), global_(global ? 1 : 0) {
    crc_ = crc(this, sizeof(RequestMeasure) - 2);
  }
} __attribute__((packed));
// Response { trans_state, global_state, v_float, crc_l, crc_h}

struct RequestSerialNumber {
  char address_;
  char command = 63;
  char reserved_[6] = {0, 0, 0, 0, 0, 0};
  uint16_t crc_;
  RequestSerialNumber(char address) : address_(address) { crc_ = crc(this, sizeof(RequestSerialNumber) - 2); }
} __attribute__((packed));
// Response { char6, char5, char4, char3, char2, char1, crc_l, crc_h}

struct RequestManufacturingWeekAndYear {
  char address_;
  char command = 65;
  char reserved_[6] = {0, 0, 0, 0, 0, 0};
  uint16_t crc_;
  RequestManufacturingWeekAndYear(char address) : address_(address) {
    crc_ = crc(this, sizeof(RequestManufacturingWeekAndYear) - 2);
  }
} __attribute__((packed));
// Response { TR.State, GlobalState, week2, week1, year2, year1, crc_l, crc_h}

}  // namespace abb_aurora
}  // namespace esphome
