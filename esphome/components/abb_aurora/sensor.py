import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, text_sensor, uart
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_FREQUENCY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_HERTZ,
    UNIT_PULSES_PER_MINUTE,
    UNIT_VOLT,
    UNIT_WATT,
)

DEPENDENCIES = ["uart"]

CONF_GLOBAL_STATE = "global_state"
CONF_INVERTER_STATE = "inverter_state"
CONF_DC_DC_CHANNEL_1_STATE = "dc_dc_channel_1_state"
CONF_DC_DC_CHANNEL_2_STATE = "dc_dc_channel_2_state"
CONF_ALARMS = "alarms"
CONF_GRID_VOLTAGE = "grid_voltage"
CONF_GRID_CURRENT = "grid_current"
CONF_GRID_POWER = "grid_power"
CONF_GRID_FREQUENCY = "grid_frequency"
CONF_SOLAR_VOLTAGE = "solar_voltage"
CONF_SOLAR_CURRENT = "solar_current"
CONF_SOLAR_POWER = "solar_power"
CONF_BOOSTER_VOLTAGE = "booster_voltage"
CONF_BOOSTER_MIDPOINT_VOLTAGE = "booster_midpoint_voltage"
CONF_INVERTER_TEMPERATURE = "inverter_temperature"
CONF_BOOSTER_TEMPERATURE = "booster_temperature"
CONF_FAN_SPEED = "fan_speed"

abb_aurora_ns = cg.esphome_ns.namespace("abb_aurora")
ABBAurora = abb_aurora_ns.class_("ABBAurora", uart.UARTDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(ABBAurora),
            cv.Optional(CONF_GLOBAL_STATE): text_sensor.text_sensor_schema(),
            cv.Optional(CONF_INVERTER_STATE): text_sensor.text_sensor_schema(),
            cv.Optional(CONF_DC_DC_CHANNEL_1_STATE): text_sensor.text_sensor_schema(),
            cv.Optional(CONF_DC_DC_CHANNEL_2_STATE): text_sensor.text_sensor_schema(),
            cv.Optional(CONF_ALARMS): text_sensor.text_sensor_schema(),
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
            cv.Optional(CONF_SOLAR_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SOLAR_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SOLAR_POWER): sensor.sensor_schema(
                unit_of_measurement=UNIT_WATT,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_BOOSTER_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_BOOSTER_MIDPOINT_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INVERTER_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_BOOSTER_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_FAN_SPEED): sensor.sensor_schema(
                unit_of_measurement=UNIT_PULSES_PER_MINUTE,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_FREQUENCY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_GLOBAL_STATE in config:
        conf = config[CONF_GLOBAL_STATE]
        ts = await text_sensor.new_text_sensor(conf)
        cg.add(var.set_global_state(ts))
    if CONF_INVERTER_STATE in config:
        conf = config[CONF_INVERTER_STATE]
        ts = await text_sensor.new_text_sensor(conf)
        cg.add(var.set_inverter_state(ts))
    if CONF_DC_DC_CHANNEL_1_STATE in config:
        conf = config[CONF_DC_DC_CHANNEL_1_STATE]
        ts = await text_sensor.new_text_sensor(conf)
        cg.add(var.set_dc_dc_channel_1_state(ts))
    if CONF_DC_DC_CHANNEL_2_STATE in config:
        conf = config[CONF_DC_DC_CHANNEL_2_STATE]
        ts = await text_sensor.new_text_sensor(conf)
        cg.add(var.set_dc_dc_channel_2_state(ts))
    if CONF_ALARMS in config:
        conf = config[CONF_ALARMS]
        ts = await text_sensor.new_text_sensor(conf)
        cg.add(var.set_alarms(ts))

    if CONF_GRID_VOLTAGE in config:
        conf = config[CONF_GRID_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_voltage_sensor(sens))
    if CONF_GRID_CURRENT in config:
        conf = config[CONF_GRID_CURRENT]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_current_sensor(sens))
    if CONF_GRID_POWER in config:
        conf = config[CONF_GRID_POWER]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_power_sensor(sens))
    if CONF_GRID_FREQUENCY in config:
        conf = config[CONF_GRID_FREQUENCY]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_frequency_sensor(sens))
    if CONF_SOLAR_VOLTAGE in config:
        conf = config[CONF_SOLAR_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_solar_voltage_sensor(sens))
    if CONF_SOLAR_CURRENT in config:
        conf = config[CONF_SOLAR_CURRENT]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_solar_current_sensor(sens))
    if CONF_SOLAR_POWER in config:
        conf = config[CONF_SOLAR_POWER]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_solar_power_sensor(sens))
    if CONF_BOOSTER_VOLTAGE in config:
        conf = config[CONF_BOOSTER_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_booster_voltage_sensor(sens))
    if CONF_BOOSTER_MIDPOINT_VOLTAGE in config:
        conf = config[CONF_BOOSTER_MIDPOINT_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_booster_midpoint_voltage_sensor(sens))
    if CONF_INVERTER_TEMPERATURE in config:
        conf = config[CONF_INVERTER_TEMPERATURE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_inverter_temperature_sensor(sens))
    if CONF_BOOSTER_TEMPERATURE in config:
        conf = config[CONF_BOOSTER_TEMPERATURE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_booster_temperature_sensor(sens))
    if CONF_FAN_SPEED in config:
        conf = config[CONF_FAN_SPEED]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_fan_speed_sensor(sens))
