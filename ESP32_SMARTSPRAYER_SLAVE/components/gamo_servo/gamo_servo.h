#pragma once

#ifndef GAMO_SERVO_H
#define GAMO_SERVO_H

#include <string>
#include <esp_err.h>
#include <esp_log.h>
#include <driver/ledc.h>

class GAMO_SERVO
{
public:
    esp_err_t AddDevice(gpio_num_t pwm_pin_);
    esp_err_t SetPWM(uint16_t channel_, uint16_t pwm_);
    esp_err_t SetAngle(uint16_t channel_, int16_t angle_);
    GAMO_SERVO(ledc_timer_t LEDC_TIMER_);
    ~GAMO_SERVO();

private:
    ledc_timer_t LEDC_TIMER;
    uint32_t current_channel = LEDC_CHANNEL_0;
};

#endif