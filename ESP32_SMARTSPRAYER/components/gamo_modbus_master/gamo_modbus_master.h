#pragma once

#ifndef GAMO_MODBUS_MASTER_H
#define GAMO_MODBUS_MASTER_H

#include <string>
#include <esp_err.h>
#include <esp_log.h>
#include <mbcontroller.h>
#include <sdkconfig.h>

class GAMO_MODBUS_MASTER
{
public:
    esp_err_t GiveMemory(mb_parameter_descriptor_t *device_parameters_, void *device_data_);
    esp_err_t AddDeviceParameter(mb_parameter_descriptor_t device_parameters_);
    esp_err_t UpdateDeviceParameter();
    GAMO_MODBUS_MASTER(uart_port_t port_ = UART_NUM_1, uint32_t baudrate_ = 9600, int32_t txd_pin_ = UART_PIN_NO_CHANGE, int32_t rxd_pin_ = UART_PIN_NO_CHANGE, int32_t rts_pin_ = UART_PIN_NO_CHANGE, uart_parity_t parity_ = UART_PARITY_DISABLE);
    ~GAMO_MODBUS_MASTER();

private:
    mb_communication_info_t comm_info;
    mb_parameter_descriptor_t *device_parameters;
    void *device_data;
    uint32_t index_device_parameters = 0;
    uint32_t index_device_data = 0;

};

#endif