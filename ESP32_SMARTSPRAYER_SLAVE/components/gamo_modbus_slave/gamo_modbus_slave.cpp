#include <stdio.h>
#include <gamo_modbus_slave.h>

static const char *TAG = "[GAMO_MODBUS_SLAVE]";

void ModbusStringSwap(char *string_, char *out_string_)
{
    uint16_t length = strlen(string_);
    for (uint16_t x = 0; x < length; x += 2)
    {
        out_string_[x] = string_[x + 1];
        out_string_[x + 1] = string_[x];
    }
}

esp_err_t GAMO_MODBUS_SLAVE::GiveMemory(mb_register_area_descriptor_t register_descriptor_, void *data_pointer_)
{
    if (data_pointer_ != nullptr)
    {
        ESP_LOGI(TAG, "Add data pointer to MODBUS_SLAVE");
        register_descriptor_.address = data_pointer_;
        mbc_slave_set_descriptor(register_descriptor_);
    }
    else
    {
        ESP_LOGE(TAG, "Fail to add data pointer to MODBUS_SLAVE!");
        return ESP_FAIL;
    }
    return ESP_OK;
}

GAMO_MODBUS_SLAVE::GAMO_MODBUS_SLAVE(uint8_t slave_addr_, uart_port_t port_, uint32_t baudrate_, int32_t txd_pin_, int32_t rxd_pin_, int32_t rts_pin_, uart_parity_t parity_)
{
    comm_info.mode = MB_MODE_RTU;
    comm_info.slave_addr = slave_addr_;
    comm_info.port = port_;
    comm_info.baudrate = baudrate_;
    comm_info.parity = parity_;
    void *handler = nullptr;
    esp_err_t err = mbc_slave_init(MB_PORT_SERIAL_SLAVE, &handler);
    if (handler == nullptr || err != ESP_OK)
        ESP_LOGE(TAG, "fail to initialization!");
    err = mbc_slave_setup(&comm_info);
    if (err != ESP_OK)
        ESP_LOGE(TAG, "fail to setup!");
    err = mbc_slave_start();
    if (err != ESP_OK)
        ESP_LOGE(TAG, "start fail!");
    err = uart_set_pin(this->comm_info.port, txd_pin_, rxd_pin_, rts_pin_, UART_PIN_NO_CHANGE);
    if (err != ESP_OK)
        ESP_LOGE(TAG, "uart_set_pin fail!");
    err = uart_set_mode(this->comm_info.port, UART_MODE_RS485_HALF_DUPLEX);
    if (err != ESP_OK)
        ESP_LOGE(TAG, "fail to set mode RS485_HALF_DUPLEX!");
    ESP_LOGI(TAG, "Modbus master stack initialized...");
}

GAMO_MODBUS_SLAVE::~GAMO_MODBUS_SLAVE()
{
    mbc_slave_destroy();
}