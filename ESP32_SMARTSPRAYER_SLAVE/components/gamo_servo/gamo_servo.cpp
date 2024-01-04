#include <stdio.h>
#include <gamo_servo.h>

static const char *TAG = "[GAMO_SERVO]";

esp_err_t GAMO_SERVO::AddDevice(gpio_num_t pwm_pin_)
{
    if (current_channel >= LEDC_CHANNEL_MAX)
    {
        ESP_LOGE(TAG, "Channel is overflowed!...");
        return ESP_FAIL;
    }
    else
    {
        ledc_channel_config_t ledc_channel = {
            .gpio_num = pwm_pin_,
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .channel = (ledc_channel_t)current_channel,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = LEDC_TIMER_0,
            .duty = 0,
            .hpoint = 0,
            .flags = {.output_invert = 1},
        };
        ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
        current_channel++;
    }
    return ESP_OK;
}

esp_err_t GAMO_SERVO::SetPWM(uint16_t channel_, uint16_t pwm_)
{
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel_, pwm_));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel_));
    return ESP_OK;
}

esp_err_t GAMO_SERVO::SetAngle(uint16_t channel_, int16_t angle_)
{
    if (angle_ < -90)
        angle_ = -90;
    if (angle_ > 90)
        angle_ = 90;
    int16_t pwm = (angle_ * 228) / 100 + 615;
    SetPWM(channel_, pwm);
    return ESP_OK;
}

GAMO_SERVO::GAMO_SERVO(ledc_timer_t LEDC_TIMER_)
{
    LEDC_TIMER = LEDC_TIMER_;
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_13_BIT,
        .timer_num = LEDC_TIMER,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
    ESP_LOGI(TAG, "Servo is initialized...");
}

GAMO_SERVO::~GAMO_SERVO()
{
}