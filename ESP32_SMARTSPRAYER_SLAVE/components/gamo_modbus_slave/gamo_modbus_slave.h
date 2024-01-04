#pragma once

#ifndef GAMO_MODBUS_SLAVE_H
#define GAMO_MODBUS_SLAVE_H

#include <string>
#include <esp_err.h>
#include <esp_log.h>
#include <mbcontroller.h>
#include <sdkconfig.h>

void ModbusStringSwap(char *string_, char *out_string_);

class GAMO_MODBUS_SLAVE
{
public:
    esp_err_t GiveMemory(mb_register_area_descriptor_t register_descriptor_, void *data_pointer_);
    GAMO_MODBUS_SLAVE(uint8_t slave_addr_, uart_port_t port_ = UART_NUM_1, uint32_t baudrate_ = 9600, int32_t txd_pin_ = UART_PIN_NO_CHANGE, int32_t rxd_pin_ = UART_PIN_NO_CHANGE, int32_t rts_pin_ = UART_PIN_NO_CHANGE, uart_parity_t parity_ = UART_PARITY_DISABLE);
    ~GAMO_MODBUS_SLAVE();

private:
    mb_communication_info_t comm_info;
};

#endif