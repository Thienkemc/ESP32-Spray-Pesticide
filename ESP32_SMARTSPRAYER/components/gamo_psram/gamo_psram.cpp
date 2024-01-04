#include <stdio.h>
#include <gamo_psram.h>

static const char *TAG = "[GAMO_PSRAM]";

uint8_t *GAMO_PSRAM::GetMemoryPartition(uint32_t *memory_size_, uint16_t memory_partition_)
{
    if (memory_partition_ < max_memory_partition)
    {
        if (memory_size_ != nullptr)
            *memory_size_ = *(uint32_t *)((uint8_t *)memory_region + sizeof(uint32_t) * memory_partition_);
        uint32_t total_skipped = 0;
        for (uint16_t i = 0; i < memory_partition_; i++)
        {
            total_skipped += *(uint32_t *)((uint8_t *)memory_region + sizeof(uint32_t) * i);
        }
        ESP_LOGI(TAG, "Total skipped %d bytes", total_skipped);
        return (uint8_t *)((uint8_t *)memory_region + sizeof(uint32_t) * max_memory_partition + total_skipped);
    }
    else
    {
        ESP_LOGE(TAG, "Invalid memory partition!");
        return nullptr;
    }
}

void GAMO_PSRAM::SetMemoryPartition(uint32_t memory_size_, uint16_t memory_partition_)
{
    if (memory_partition_ < max_memory_partition)
    {
        *(uint32_t *)((uint8_t *)memory_region + sizeof(uint32_t) * memory_partition_) = memory_size_;
        ESP_LOGI(TAG, "Make memory partition %d bytes", memory_size_);
    }
    else
    {
        ESP_LOGE(TAG, "Invalid memory partition!");
    }
}

GAMO_PSRAM::GAMO_PSRAM(uint32_t max_memory_size_, uint16_t max_memory_partition_)
{
    max_memory_size = {max_memory_size_};
    max_memory_partition = {max_memory_partition_};
    ESP_LOGI(TAG, "Allocating PSRAM region %d bytes...", max_memory_size_);
    memory_region = heap_caps_malloc(max_memory_partition * sizeof(uint32_t) + max_memory_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (memory_region == nullptr)
    {
        ESP_LOGE(TAG, "Failed to allocating PSRAM region!");
    }
    else
    {
        ESP_LOGI(TAG, "Succeeded to allocating PSRAM region");
        std::memset(memory_region, 0, sizeof(uint32_t) * max_memory_partition_);
    }
}

GAMO_PSRAM::~GAMO_PSRAM()
{
}
