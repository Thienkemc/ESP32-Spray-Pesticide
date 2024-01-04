#include <stdio.h>
#include <main.h>

GAMO_SPIFFS HOME_SPIFFS("/spiffs");
GAMO_MODBUS_SLAVE MODBUS_VALVE(0x10, UART_NUM_1, 9600, 32, 26, 33);
GAMO_SERVO SERVO(LEDC_TIMER_0);

static const char *TAG = "[MAIN]";
extern "C" void app_main(void)
{
    gamo_nvs_init();
    HOME_SPIFFS.GetInfo();
    char *return_string;

    void *test_data = malloc(0x04);
    memset(test_data, 0, 0x04);

    uint16_t *test_16 = (uint16_t *)test_data;
    mb_register_area_descriptor_t test = {
        .start_offset = 0x0000,
        .type = MB_PARAM_HOLDING,
        .size = 0x04,
    };
    MODBUS_VALVE.GiveMemory(test, test_data);

    *(test_16 + ENABLE_VALVE) = 0;
    *(test_16 + ADDRESS_VALVE) = 0x10;

    SERVO.AddDevice(GPIO_NUM_13);
    SERVO.SetAngle(0, -90);
    while (1)
    {
        if (*(test_16 + ENABLE_VALVE) == 0)
        {
            SERVO.SetAngle(0, -90);
            ESP_LOGI(TAG, "-90 deg");
        }
        else
        {
            SERVO.SetAngle(0, 90);
            ESP_LOGI(TAG, "90 deg");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        ESP_LOG_BUFFER_HEX(TAG, test_16, 10);
        ESP_LOGI(TAG, "%d", *(test_16 + ENABLE_VALVE));
    }
}
