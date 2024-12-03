#pragma once

#include "Bus.hpp"
#include <cstdint>
#include <unordered_map>
#include <functional>

namespace EB16
{
    enum class InstructionType : uint8_t
    {
        NOP = 0x00,
        HLT = 0x01,
        LDX_IMM = 0x02,
        LDX_ADDR = 0x03,
        LDY_IMM = 0x04,
        LDY_ADDR = 0x05,
        LDZ_IMM = 0x06,
        LDZ_ADDR = 0x07,
        MOV = 0x08,
    };

    class CPU
    {
    public:
        CPU(Bus::Bus &bus);
        void run();
        void dump();

    private:
        uint16_t pc_;
        uint8_t x_;
        uint8_t y_;
        uint8_t z_;
        Bus::Bus &bus_;
        std::unordered_map<uint8_t, std::function<void()>> instructions_;

        void fetchAndExecute();
        void executeNOP();
        void executeHLT();
        void executeLDX_IMM();
        void executeLDX_ADDR();
        void executeLDY_IMM();
        void executeLDY_ADDR();
        void executeLDZ_IMM();
        void executeLDZ_ADDR();
        void executeMOV();
    };
}
