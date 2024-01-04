#pragma once

#ifndef GAMO_PSRAM_H
#define GAMO_PSRAM_H

#include <cstring>
#include <esp_log.h>
#include <esp_heap_caps.h>
#include <esp_err.h>

class GAMO_PSRAM
{
public:
    uint8_t *GetMemoryPartition(uint32_t *memory_size_, uint16_t memory_partition_);
    void SetMemoryPartition(uint32_t memory_size_, uint16_t memory_partition_);
    GAMO_PSRAM(uint32_t max_memory_size_, uint16_t max_memory_partition_);
    ~GAMO_PSRAM();

protected:
    uint32_t max_memory_size;
    uint16_t max_memory_partition;
    void *memory_region;
};

#endif