#include <stdio.h>
#include <main.h>

GAMO_HTTP_CLIENT TEST1("kemc.ddns.net", "/post/postDATA.php");
GAMO_SPIFFS HOME_SPIFFS("/spiffs");
GAMO_PSRAM SHARED_PSRAM(2 * 1024 * 1024, 2);
// GAMO_MODBUS_MASTER MODBUS_LCD(UART_NUM_1, 115200, 14, 32, 4);
GAMO_MODBUS_SLAVE MODBUS_LCD(0x01, UART_NUM_1, 115200, 14, 32, 4);
GAMO_MODBUS_MASTER MODBUS_SLAVE(UART_NUM_2, 9600, 13, 15, UART_PIN_NO_CHANGE);

char NVS_SSID[50];
size_t NVS_SSID_LENGTH = 0;
char NVS_PASSWORD[50];
size_t NVS_PASSWORD_LENGTH = 0;

static char SSID[16] = "KEMC";
static char PASSWORD[16] = "12345678";
static char DEVICE_ID[16] = "ESP_000";

uint8_t *test_data = (uint8_t *)malloc(0x2000 * 2);
uint16_t *test_16 = (uint16_t *)test_data;
mb_register_area_descriptor_t test = {
    .start_offset = 0x0000,
    .type = MB_PARAM_HOLDING,
    .size = 0x4000,
};

nvs_handle_t nvs_handle_data;

static const char *TAG = "[MAIN]";

void check_wifi_setting(void)
{
    wifi_config_t wifi_config = {
        .sta = {
            .scan_method = WIFI_ALL_CHANNEL_SCAN,
            .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
        },
    };
    while (1)
    {
        if (*(test_data + 0x0200 * 2) != 0)
        {
            char OUT[16];
            ModbusStringSwap((char *)(test_data + 0x0115 * 2), OUT);
            strcpy(SSID, OUT);
            ModbusStringSwap((char *)(test_data + 0x0125 * 2), OUT);
            strcpy(PASSWORD, OUT);
            ESP_ERROR_CHECK(esp_wifi_stop());
            strcpy((char *)wifi_config.sta.ssid, SSID);
            strcpy((char *)wifi_config.sta.password, PASSWORD);
            ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
            ESP_ERROR_CHECK(esp_wifi_start());
            *(test_data + 0x0200 * 2) = 0;
            ESP_LOGI(TAG, "Change WiFi config!");
            nvs_set_str(nvs_handle_data, "MY_SSID", SSID);
            nvs_set_str(nvs_handle_data, "MY_PASSWORD", PASSWORD);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C" void app_main(void)
{
    memset(test_data, 0, 0x2000 * 2);
    gamo_nvs_init();
    gamo_wifi_init_sta(SSID, PASSWORD);
    HOME_SPIFFS.GetInfo();
    char *return_string;
    SHARED_PSRAM.SetMemoryPartition(PARTITION_DEVICE_PARAMETER_SIZE, PARTITION_DEVICE_PARAMETER);
    SHARED_PSRAM.SetMemoryPartition(PARTITION_DEVICE_DATA_SIZE, PARTITION_DEVICE_DATA);
    MODBUS_SLAVE.GiveMemory((mb_parameter_descriptor_t *)SHARED_PSRAM.GetMemoryPartition(nullptr, PARTITION_DEVICE_PARAMETER), (void *)SHARED_PSRAM.GetMemoryPartition(nullptr, PARTITION_DEVICE_DATA));

    // MODBUS SLAVE

    mb_parameter_descriptor_t test_slave = {
        .cid = 0,
        .param_key = "TEMP_HUMI",
        .param_units = "degC_%",
        .mb_slave_addr = 0x01,
        .mb_param_type = MB_PARAM_INPUT,
        .mb_reg_start = 0x0001,
        .mb_size = sizeof(uint16_t),
        .param_type = PARAM_TYPE_U16,
        .param_size = PARAM_SIZE_U16,
        .access = PAR_PERMS_READ_WRITE_TRIGGER,
    };

    MODBUS_SLAVE.AddDeviceParameter(test_slave);
    test_slave.cid = 1;
    test_slave.param_key = "WIND_SPEED";
    test_slave.param_units = "m/s";
    test_slave.mb_slave_addr = 0x02;
    test_slave.mb_param_type = MB_PARAM_HOLDING;
    test_slave.mb_reg_start = 0x0000;
    test_slave.mb_size = sizeof(uint8_t);
    test_slave.param_type = PARAM_TYPE_U8;
    test_slave.param_size = PARAM_SIZE_U8;
    MODBUS_SLAVE.AddDeviceParameter(test_slave);

    test_slave.cid = 2;
    test_slave.param_key = "VALVE000";
    test_slave.param_units = "";
    test_slave.mb_slave_addr = 0x10;
    test_slave.mb_param_type = MB_PARAM_HOLDING;
    test_slave.mb_reg_start = 0x0000;
    test_slave.mb_size = sizeof(uint16_t);
    test_slave.param_type = PARAM_TYPE_U16;
    test_slave.param_size = PARAM_SIZE_U16;
    MODBUS_SLAVE.AddDeviceParameter(test_slave);
    MODBUS_SLAVE.UpdateDeviceParameter();

    MODBUS_LCD.GiveMemory(test, test_data);
    *(test_16 + 1) = 20;

    char OUT[16];
    ModbusStringSwap(SSID, OUT);
    memcpy((test_data + 0x0115 * 2), OUT, sizeof(OUT));
    ModbusStringSwap(PASSWORD, OUT);
    memcpy((test_data + 0x0125 * 2), OUT, sizeof(OUT));
    ModbusStringSwap(DEVICE_ID, OUT);
    memcpy((test_data + 0x0145 * 2), OUT, sizeof(OUT));

    xTaskCreate((TaskFunction_t)check_wifi_setting, "[check_wifi_setting]", 4 * 1024, nullptr, 0, nullptr);

    while (1)
    {
        // for (uint16_t i = 0; i < 28; i++)
        // {
        //     test_data[i] += i;
        // }
        // uint8_t temp_data_pointer = 20;
        // uint8_t temp_data_pointer_type = PARAM_TYPE_U16;
        // const mb_parameter_descriptor_t *param_descriptor = NULL;
        // mbc_master_get_cid_info(0, &param_descriptor);
        // // mbc_master_get_parameter(0, (char*)param_descriptor->param_key, &temp_data_pointer, &temp_data_pointer_type);
        // mbc_master_set_parameter(0, (char *)param_descriptor->param_key, &temp_data_pointer, &temp_data_pointer_type);
        // ESP_LOGI(TAG, "Data is: %d, type is: %d", temp_data_pointer, temp_data_pointer_type);

        int16_t temp_data_pointer[2];
        uint8_t temp_data_pointer_type;
        const mb_parameter_descriptor_t *param_descriptor = NULL;
        mbc_master_get_cid_info(0, &param_descriptor);
        mbc_master_get_parameter(0, (char *)param_descriptor->param_key, (uint8_t *)temp_data_pointer, &temp_data_pointer_type);
        ESP_LOGI(TAG, "1. TEMP is: %d, HUMI is: %d", temp_data_pointer[0], temp_data_pointer[1]);

        *(test_16 + 0x0107) = temp_data_pointer[0] / 100; // 30; //
        *(test_16 + 0x0108) = temp_data_pointer[1] / 100; // 53; //

        vTaskDelay(pdMS_TO_TICKS(100));

        mbc_master_get_cid_info(1, &param_descriptor);
        mbc_master_get_parameter(1, (char *)param_descriptor->param_key, (uint8_t *)temp_data_pointer, &temp_data_pointer_type);

        ESP_LOGI(TAG, "2. WIND is : %d", temp_data_pointer[0]);

        *(test_16 + 0x0110) = temp_data_pointer[0] * 3.6 / 10;

        vTaskDelay(pdMS_TO_TICKS(100));

        mbc_master_get_cid_info(2, &param_descriptor);
        mbc_master_set_parameter(2, (char *)param_descriptor->param_key, (uint8_t *)(test_data + 0x1201 * 2), &temp_data_pointer_type);

        *(test_16 + 0x0109) = 0;
        *(test_16 + 0x0111) = 0;
        *(test_16 + 0x0112) = 0;

        // mbc_master_get_cid_info(1, &param_descriptor);
        // mbc_master_get_parameter(1, (char *)param_descriptor->param_key, (uint8_t*)&temp_data_pointer, &temp_data_pointer_type);
        // ESP_LOGI(TAG, "2. Data is: %d, type is: %d", temp_data_pointer, temp_data_pointer_type);

        // ESP_LOG_BUFFER_HEX(TAG, test_data, 100 * 2);

        char debug_buffer[200];
        sprintf(debug_buffer, "DEVICE_ID=10:52:1c:8b:69:04&TEMPERATURE=%d&WIND_SPEED=%d&SOIL_MOISTURE=0&HUMIDITY=%d&FUEL=0&PESTICIDE=0&ENGINE_TEMPERATURE=0&PRESSURE=0&PROGRESS=0", *(test_16 + 0x0107), *(test_16 + 0x0110), *(test_16 + 0x0108));
        TEST1.POST_FORM(debug_buffer, &return_string);
        // ESP_LOGI(TAG, "%s", return_string);
    }
}
