#include <cstdint>

inline const char *transmission_state(uint8_t state) {
  switch (state) {
    case 0:
      return "OK";
    case 51:
      return "Command is not implemented";
    case 52:
      return "Variable does not exist";
    case 53:
      return "Variable value is out of range";
    case 54:
      return "EEprom not accessible";
    case 55:
      return "Not toggled service mode";
    case 56:
      return "Cannot send the command to internal micro";
    case 57:
      return "Command not executed";
    case 58:
      return "The variable is not available, retry";
    default:
      return "Unknown";
  }
}

inline const char *global_state(uint8_t state) {
  switch (state) {
    case 0:
      return "Sending parameters";
    case 1:
      return "Waiting on sun/grid";
    case 2:
      return "Checking grid";
    case 3:
      return "Measuring isolation resistance";
    case 4:
      return "DC DC starting";
    case 5:
      return "Inverter starting";
    case 6:
      return "Running";
    case 7:
      return "Recovery";
    case 8:
      return "Pause";
    case 9:
      return "Ground fault";
    case 10:
      return "OTH fault";
    case 11:
      return "Address setting";
    case 12:
      return "Self test";
    case 13:
      return "Self test failure";
    case 14:
      return "Sensor test and measuring isolation resistance";
    case 15:
      return "Leak fault";
    case 16:
      return "Waiting for manual reset";
    case 17:
      return "Internal error E026";
    case 18:
      return "Internal error E027";
    case 19:
      return "Internal error E028";
    case 20:
      return "Internal error E029";
    case 21:
      return "Internal error E030";
    case 22:
      return "Sending wind table";
    case 23:
      return "Failed sending table";
    case 24:
      return "UTH fault";
    case 25:
      return "Remote off";
    case 26:
      return "Interlock fail";
    case 27:
      return "Executing autotest";
    case 30:
      return "Waiting on sun";
    case 31:
      return "Temperature fault";
    case 32:
      return "Fan stuck";
    case 33:
      return "Internal communication fault";
    case 34:
      return "Slave insertion";
    case 35:
      return "DC switch open";
    case 36:
      return "TRAS switch open";
    case 37:
      return "Master exclusion";
    case 38:
      return "Auto exclusion";
    case 98:
      return "Erasing internal EEPROM";
    case 99:
      return "Erasing external EEPROM";
    case 100:
      return "Counting EEPROM";
    case 101:
      return "Freeze";
    default:
      return "Unknown";
  }
}

inline const char *dc_dc_state(uint8_t state) {
  switch (state) {
    case 0:
      return "DC DC off";
    case 1:
      return "Ramp start";
    case 2:
      return "MPPT";
    case 3:
      return "Not used";
    case 4:
      return "Input over current";
    case 5:
      return "Input under voltage";
    case 6:
      return "Input over voltage";
    case 7:
      return "Input low";
    case 8:
      return "No parameters";
    case 9:
      return "Bulk over voltage";
    case 10:
      return "Communication error";
    case 11:
      return "Ramp failure";
    case 12:
      return "Internal error";
    case 13:
      return "Input mode error";
    case 14:
      return "Ground fault";
    case 15:
      return "Inverter failure";
    case 16:
      return "DC DC IGBT Sat";
    case 17:
      return "DC DC current leak failure";
    case 18:
      return "DC DC grid failure";
    case 19:
      return "DC DC communication error";
    default:
      return "Unknown";
  }
}

inline const char *inverter_state(uint8_t state) {
  switch (state) {
    case 0:
      return "Standby";
    case 1:
      return "Checking grid";
    case 2:
      return "Running";
    case 3:
      return "Bulk over voltage";
    case 4:
      return "Output over current";
    case 5:
      return "IGBT saturation";
    case 6:
      return "Bulk under voltage";
    case 7:
      return "Degauss error";
    case 8:
      return "No parameters";
    case 9:
      return "Bulk low";
    case 10:
      return "Grid over voltage";
    case 11:
      return "Communication error";
    case 12:
      return "Degaussing";
    case 13:
      return "Starting";
    case 14:
      return "Bulk capacitor failure";
    case 15:
      return "Leak failure";
    case 16:
      return "DC DC failure";
    case 17:
      return "ILeak sensor failure";
    case 18:
      return "Self test: Relay inverter";
    case 19:
      return "Self test: Wait for sensor test";
    case 20:
      return "Self test: Test relay, DC DC and sensors";
    case 21:
      return "Self test: Relay inverter failure";
    case 22:
      return "Self test: Timeout";
    case 23:
      return "Self test: Relay DC DC failure";
    case 24:
      return "Self test 1";
    case 25:
      return "Waiting to start self test";
    case 26:
      return "DC Injection";
    case 27:
      return "Self test 2";
    case 28:
      return "Self test 3";
    case 29:
      return "Self test 4";
    case 30:
    case 31:
      return "Internal error";
    case 40:
      return "Forbidden state";
    case 41:
      return "Input under current";
    case 42:
      return "Zero power";
    case 43:
      return "Grid not present";
    case 44:
      return "Waiting for start";
    case 45:
      return "MPPT";
    case 46:
      return "Grid fail";
    case 47:
      return "Input over current";
    default:
      return "Unknown";
  }
}

inline const char *alarm_state(uint8_t state) {
  switch (state) {
    case 0:
      return "No alarm";
    case 1:
      return "Sun low (W001)";
    case 2:
      return "Input over current (E001)";
    case 3:
      return "Input under voltage (W002)";
    case 4:
      return "Input over voltage (E002)";
    case 5:
      return "Sun low (W001)";
    case 6:
      return "No parameters (E003)";
    case 7:
      return "Bulk over voltage (E004)";
    case 8:
      return "Communication error (E005)";
    case 9:
      return "Output over current (E006)";
    case 10:
      return "IGBT saturation (E007)";
    case 11:
      return "Bulk under voltage (W011)";
    case 12:
      return "Internal error (E009)";
    case 13:
      return "Grid fail (W003)";
    case 14:
      return "Bulk low (E010)";
    case 15:
      return "Ramp failure (E011)";
    case 16:
      return "DC DC failure (E012)";
    case 17:
      return "Wrong mode (E013)";
    case 18:
      return "Ground fault";
    case 19:
      return "Over temperature (E014)";
    case 20:
      return "Bulk capacitor failure (E015)";
    case 21:
      return "Inverter failure (E016)";
    case 22:
      return "Start up timeout (E017)";
    case 23:
      return "Ground fault (E018)";
    case 24:
      return "Degauss error";
    case 25:
      return "Current leak sensor failure (E019)";
    case 26:
      return "DC DC failure (E012)";
    case 27:
      return "Self test error 1 (E020)";
    case 28:
      return "Self test error 2 (E021)";
    case 29:
      return "Self test error 3 (E019)";
    case 30:
      return "Self test error 4 (E022)";
    case 31:
      return "DC injection error (E023)";
    case 32:
      return "Grid over voltage (W004)";
    case 33:
      return "Grid under voltage (W005)";
    case 34:
      return "Grid over frequency (W006)";
    case 35:
      return "Grid under frequency (W007)";
    case 36:
      return "Z grid high (W008)";
    case 37:
      return "Internal error (E024)";
    case 38:
      return "Isolation resistance low (E025)";
    case 39:
      return "Reference voltage error (E026)";
    case 40:
      return "Error measuring voltage (E027)";
    case 41:
      return "Error measuring frequency (E028)";
    case 42:
      return "Error measuring impedance (E029)";
    case 43:
      return "Error measuring current leak (E030)";
    case 44:
      return "Error reading voltage (E031)";
    case 45:
      return "Error reading current (E032)";
    case 46:
      return "Table fail (W009)";
    case 47:
      return "Fan fail (W010)";
    case 48:
      return "UTH (E033)";
    case 49:
      return "Interlock fail (E034)";
    case 50:
      return "Remote off (E035)";
    case 51:
      return "Output voltage average error (E036)";
    case 52:
      return "Battery low (W012)";
    case 53:
      return "Clock failure (W013)";
    case 54:
      return "Input under current (E037)";
    case 55:
      return "Zero power (W014)";
    case 56:
      return "Fan stuck (E038)";
    case 57:
      return "DC switch open (E039)";
    case 58:
      return "Transformer switch open (E040)";
    case 59:
      return "AC switch open (E041)";
    case 60:
      return "Bulk under voltage (E042)";
    case 61:
      return "Autoexclusion (E043)";
    case 62:
      return "Grid df/dt (W015)";
    case 63:
      return "Den switch open (W016)";
    case 64:
      return "Junction box failure (W017)";
    default:
      return "Unknown";
  }
}
