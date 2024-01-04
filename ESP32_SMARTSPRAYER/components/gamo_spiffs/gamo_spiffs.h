#pragma once

#ifndef GAMO_SPIFFS_H
#define GAMO_SPIFFS_H

#include <string>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_spiffs.h>
#include <esp_vfs_fat.h>
#include <sys/stat.h>

class GAMO_SPIFFS
{
public:
    std::string GetFilePath(std::string file_name_);
    uint32_t GetFileSize(std::string file_name_);
    esp_err_t GetFile(std::string file_name_, uint8_t *buffer);
    esp_err_t GetInfo();
    GAMO_SPIFFS(std::string base_path_ = "/spiffs", size_t max_files_ = 10, bool format_if_mount_failded_ = true);
    ~GAMO_SPIFFS();

private:
    esp_vfs_spiffs_conf_t config;
    std::string base_path;
    size_t max_files;
    bool format_if_mount_failded;
};

#endif