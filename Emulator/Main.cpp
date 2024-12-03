#include "Emulator.hpp"
#include "Bus.hpp"
#include "Memory.hpp"
#include "EB16.hpp"

#include <iostream>
#include <memory>
#include <exception>

std::shared_ptr<spdlog::logger> Emulator::logger = nullptr;

int main()
{
    Emulator::initializeLogger();
    std::unique_ptr<EB16::CPU> cpu;
    try
    {
        std::unique_ptr<Bus::Bus> bus = std::make_unique<Bus::Bus>();

        uint8_t rom_data[] = {
            // NOP (No operation)
            0x00, // NOP

            // LDX_IMM (Load immediate value into X register)
            0x02, 0x42, // LDX 0x42

            // MOV (Move value from one register to another)
            0x08, 0x00, 0x01, // MOV X, Y

            // HLT (Halt execution)
            0x01 // HLT
        };

        auto boot_rom = std::make_shared<Memory::MemoryDevice>(0x0000, 0x1FFF, rom_data, sizeof(rom_data));

        auto extended_rom = std::make_shared<Memory::MemoryDevice>(0x2000, 0x3FFF, nullptr, 0);
        auto ram = std::make_shared<Memory::MemoryDevice>(0x4000, 0xFFFF, nullptr, 0);

        bus->attachDevice(boot_rom, Bus::Permission::Read);
        bus->attachDevice(extended_rom, Bus::Permission::Read);
        bus->attachDevice(ram, Bus::Permission::Read | Bus::Permission::Write);

        Emulator::logger->info("Running eb16");

        cpu = std::make_unique<EB16::CPU>(*bus);
        cpu->run();
        Emulator::logger->info("Finished running eb16");
        cpu->dump();
    }
    catch (const std::exception &e)
    {
        Emulator::logger->error("Error: {}", e.what());
        cpu->dump();
    }

    return 0;
}
