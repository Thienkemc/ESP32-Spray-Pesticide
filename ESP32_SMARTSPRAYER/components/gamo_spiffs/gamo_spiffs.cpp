#include <stdio.h>
#include <gamo_spiffs.h>

static const char *TAG = "[GAMO_SPIFFS]";

esp_err_t GAMO_SPIFFS::GetInfo()
{
    size_t total = 0, used = 0;
    esp_err_t ret = esp_spiffs_info(nullptr, &total, &used);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    }
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    return ret;
}

std::string GAMO_SPIFFS::GetFilePath(std::string file_name_)
{
    return config.base_path + std::string("/") + file_name_;
}

uint32_t GAMO_SPIFFS::GetFileSize(std::string file_name_)
{
    struct stat buffer;
    stat(GetFilePath(file_name_).c_str(), &buffer);
    return buffer.st_size;
}

esp_err_t GAMO_SPIFFS::GetFile(std::string file_name_, uint8_t *buffer)
{
    FILE *file_ = fopen(GetFilePath(file_name_).c_str(), "rb");
    ESP_LOGI(TAG, "%s %d", GetFilePath(file_name_).c_str(), GetFileSize(file_name_));
    fread(buffer, 1, GetFileSize(file_name_), file_);
    fclose(file_);
    return ESP_OK;
}

GAMO_SPIFFS::GAMO_SPIFFS(std::string base_path_, size_t max_files_, bool format_if_mount_failded_)
{
    this->base_path = base_path_;
    this->max_files = max_files_;
    this->format_if_mount_failded = format_if_mount_failded_; 
    this->config.base_path = base_path.c_str();
    ESP_LOGI(TAG, "SPIFFS base_path= %s", base_path.c_str());
    this->config.partition_label = nullptr;
    this->config.max_files = max_files;
    this->config.format_if_mount_failed = format_if_mount_failded;
    esp_err_t ret = esp_vfs_spiffs_register(&(this->config));
    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
    }
}

GAMO_SPIFFS::~GAMO_SPIFFS()
{
    esp_err_t ret = esp_vfs_spiffs_unregister(nullptr);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Can't unregister SPIFFS (%s)", esp_err_to_name(ret));
    }
}