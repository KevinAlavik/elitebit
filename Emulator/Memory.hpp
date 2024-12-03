#pragma once

#include "Bus.hpp"
#include <vector>
#include <stdexcept>

namespace Memory
{
    class MemoryDevice : public Bus::BusDevice
    {
    public:
        MemoryDevice(uint16_t start, uint16_t end, uint8_t *initBuffer = nullptr, size_t bufferSize = 0);

        bool handles(uint16_t address) const override;
        uint8_t read(uint16_t address) const override;
        void write(uint16_t address, uint8_t value) override;

    private:
        uint16_t start_;
        uint16_t end_;
        std::vector<uint8_t> memory_;
    };
}
