#pragma once

#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <esp_log.h>
#include <gamo_nvs.h>
#include <gamo_spiffs.h>
#include <gamo_modbus_slave.h>
#include <gamo_servo.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <hal/cpu_hal.h>
#include <esp_heap_caps.h>

#define ENABLE_VALVE 0
#define ADDRESS_VALVE 1

#endif