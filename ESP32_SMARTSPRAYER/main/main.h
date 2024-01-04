#pragma once

#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <cstring>
#include <esp_log.h>
#include <gamo_nvs.h>
#include <gamo_wlan.h>
#include <gamo_http_client.h>
#include <gamo_spiffs.h>
#include <gamo_psram.h>
#include <gamo_modbus_master.h>
#include <gamo_modbus_slave.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <hal/cpu_hal.h>
#include <esp_heap_caps.h>

#define PARTITION_DEVICE_PARAMETER 0
#define PARTITION_DEVICE_PARAMETER_SIZE 1 * 1024 * 1024 // 1MB SPI-RAM for DEVICE_PARAMETER.
#define PARTITION_DEVICE_DATA 1
#define PARTITION_DEVICE_DATA_SIZE 512 * 1024 // 512KB SPI-RAM for DEVICE_DATA.

#endif