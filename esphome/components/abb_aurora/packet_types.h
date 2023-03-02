#pragma once

#include <cstdint>

namespace esphome {
namespace abb_aurora {

inline uint16_t crc(const void *data, int len) {
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

  return ((~hi & 0xFF) << 8) | (~lo & 0xFF);
}

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

}  // namespace abb_aurora
}  // namespace esphome
