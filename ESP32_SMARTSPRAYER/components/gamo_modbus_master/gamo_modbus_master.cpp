#include <stdio.h>
#include <gamo_modbus_master.h>

static const char *TAG = "[GAMO_MODBUS_MASTER]";

esp_err_t GAMO_MODBUS_MASTER::GiveMemory(mb_parameter_descriptor_t *device_parameters_, void *device_data_)
{
    this->device_parameters = device_parameters_;
    this->device_data = device_data_;
    if (this->device_parameters != nullptr)
        ESP_LOGI(TAG, "Give memory to device_parameters");
    if (this->device_data != nullptr)
        ESP_LOGI(TAG, "Give memory to device_data");
    else
        return ESP_FAIL;
    index_device_data = 0;
    index_device_parameters = 0;
    return ESP_OK;
}

esp_err_t GAMO_MODBUS_MASTER::AddDeviceParameter(mb_parameter_descriptor_t device_parameters_)
{
    if (this->device_parameters == nullptr)
    {
        ESP_LOGE(TAG, "Memory is not initialized!");
        return ESP_FAIL;
    }
    (this->device_parameters + this->index_device_parameters)->cid = device_parameters_.cid;
    (this->device_parameters + this->index_device_parameters)->param_key = device_parameters_.param_key;
    (this->device_parameters + this->index_device_parameters)->mb_slave_addr = device_parameters_.mb_slave_addr;
    (this->device_parameters + this->index_device_parameters)->mb_param_type = device_parameters_.mb_param_type;
    (this->device_parameters + this->index_device_parameters)->mb_reg_start = device_parameters_.mb_reg_start;
    (this->device_parameters + this->index_device_parameters)->mb_size = device_parameters_.mb_size;
    (this->device_parameters + this->index_device_parameters)->param_offset = this->index_device_data;
    (this->device_parameters + this->index_device_parameters)->param_type = device_parameters_.param_type;
    (this->device_parameters + this->index_device_parameters)->param_size = device_parameters_.param_size;
    (this->device_parameters + this->index_device_parameters)->param_opts = device_parameters_.param_opts;
    (this->device_parameters + this->index_device_parameters)->access = device_parameters_.access;
    this->index_device_data += device_parameters_.param_size;
    this->index_device_parameters++;
    return ESP_OK;
}

esp_err_t GAMO_MODBUS_MASTER::UpdateDeviceParameter()
{
    esp_err_t err = mbc_master_set_descriptor(this->device_parameters, this->index_device_parameters);
    return err;
}

GAMO_MODBUS_MASTER::GAMO_MODBUS_MASTER(uart_port_t port_, uint32_t baudrate_, int32_t txd_pin_, int32_t rxd_pin_, int32_t rts_pin_, uart_parity_t parity_)
{
    comm_info.mode = MB_MODE_RTU;
    comm_info.port = port_;
    comm_info.baudrate = baudrate_;
    comm_info.parity = parity_;
    void *handler = nullptr;
    esp_err_t err = mbc_master_init(MB_PORT_SERIAL_MASTER, &handler);
    if (handler == nullptr || err != ESP_OK)
        ESP_LOGE(TAG, "fail to initialization!");
    err = mbc_master_setup(&comm_info);
    if (err != ESP_OK)
        ESP_LOGE(TAG, "fail to setup!");
    err = uart_set_pin(this->comm_info.port, txd_pin_, rxd_pin_, rts_pin_, UART_PIN_NO_CHANGE);
    if (err != ESP_OK)
        ESP_LOGE(TAG, "uart_set_pin fail!");
    err = mbc_master_start();
    if (err != ESP_OK)
        ESP_LOGE(TAG, "start fail!");
    err = uart_set_mode(this->comm_info.port, UART_MODE_RS485_HALF_DUPLEX);
    if (err != ESP_OK)
        ESP_LOGE(TAG, "fail to set mode RS485_HALF_DUPLEX!");
    ESP_LOGI(TAG, "Modbus master stack initialized...");
}

GAMO_MODBUS_MASTER::~GAMO_MODBUS_MASTER()
{
    mbc_master_destroy();
}