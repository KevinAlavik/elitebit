#include "Memory.hpp"
#include <cstring>

namespace Memory
{
    MemoryDevice::MemoryDevice(uint16_t start, uint16_t end, uint8_t *initBuffer, size_t bufferSize)
        : start_(start), end_(end)
    {
        memory_.resize(end_ - start_ + 1);

        if (bufferSize > memory_.size())
        {
            throw std::invalid_argument("Buffer size exceeds memory capacity");
        }

        if (initBuffer != nullptr && bufferSize > 0)
        {
            size_t copySize = std::min(bufferSize, memory_.size());
            std::memcpy(memory_.data(), initBuffer, copySize);
        }
    }

    bool MemoryDevice::handles(uint16_t address) const
    {
        return address >= start_ && address <= end_;
    }

    uint8_t MemoryDevice::read(uint16_t address) const
    {
        if (address < start_ || address > end_)
        {
            throw std::out_of_range("Read address out of range");
        }
        return memory_[address - start_];
    }

    void MemoryDevice::write(uint16_t address, uint8_t value)
    {
        if (address < start_ || address > end_)
        {
            throw std::out_of_range("Write address out of range");
        }
        memory_[address - start_] = value;
    }
}
