import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart, text_sensor
from esphome.const import (
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
    DEVICE_CLASS_FREQUENCY,
    DEVICE_CLASS_TEMPERATURE,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_HERTZ,
    CONF_ID,
    CONF_TEMPERATURE,
    UNIT_VOLT,
    UNIT_WATT,
)

DEPENDENCIES = ["uart", "text_sensor"]

CONF_WORK_MODE = "work_mode"
CONF_GRID_VOLTAGE = "grid_voltage"
CONF_GRID_CURRENT = "grid_current"
CONF_GRID_POWER = "grid_power"
CONF_GRID_FREQUENCY = "grid_frequency"
CONF_SOLAR_STRING_1_VOLTAGE = "solar_string_1_voltage"
CONF_SOLAR_STRING_1_CURRENT = "solar_string_1_current"
CONF_SOLAR_STRING_2_VOLTAGE = "solar_string_2_voltage"
CONF_SOLAR_STRING_2_CURRENT = "solar_string_2_current"
CONF_BOOSTER_TEMPERATURE = "booster_temperature"

goodwe_ns = cg.esphome_ns.namespace("goodwe")
GoodWe = goodwe_ns.class_("GoodWe", uart.UARTDevice, cg.Component)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(GoodWe),
            cv.Optional(CONF_WORK_MODE): text_sensor.text_sensor_schema(),
            cv.Optional(CONF_GRID_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_GRID_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_GRID_POWER): sensor.sensor_schema(
                unit_of_measurement=UNIT_WATT,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_GRID_FREQUENCY): sensor.sensor_schema(
                unit_of_measurement=UNIT_HERTZ,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_FREQUENCY,
            ),
            cv.Optional(CONF_SOLAR_STRING_1_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SOLAR_STRING_1_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SOLAR_STRING_2_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SOLAR_STRING_2_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_WORK_MODE in config:
        conf = config[CONF_WORK_MODE]
        ts = await text_sensor.new_text_sensor(conf)
        cg.add(var.set_work_mode_sensor(ts))
    if CONF_GRID_VOLTAGE in config:
        conf = config[CONF_GRID_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_grid_voltage_sensor(sens))
    if CONF_GRID_CURRENT in config:
        conf = config[CONF_GRID_CURRENT]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_grid_current_sensor(sens))
    if CONF_GRID_POWER in config:
        conf = config[CONF_GRID_POWER]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_grid_power_sensor(sens))
    if CONF_GRID_FREQUENCY in config:
        conf = config[CONF_GRID_FREQUENCY]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_grid_frequency_sensor(sens))
    if CONF_SOLAR_STRING_1_VOLTAGE in config:
        conf = config[CONF_SOLAR_STRING_1_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_solar_string_1_voltage_sensor(sens))
    if CONF_SOLAR_STRING_1_CURRENT in config:
        conf = config[CONF_SOLAR_STRING_1_CURRENT]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_solar_string_1_current_sensor(sens))
    if CONF_SOLAR_STRING_2_VOLTAGE in config:
        conf = config[CONF_SOLAR_STRING_2_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_solar_string_2_voltage_sensor(sens))
    if CONF_SOLAR_STRING_2_CURRENT in config:
        conf = config[CONF_SOLAR_STRING_2_CURRENT]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_solar_string_2_current_sensor(sens))
    if CONF_TEMPERATURE in config:
        conf = config[CONF_TEMPERATURE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temperature_sensor(sens))
