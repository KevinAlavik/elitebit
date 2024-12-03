#include "Emulator.hpp"
#include "Bus.hpp"
#include "Memory.hpp"

#include <iostream>
#include <memory>
#include <exception>

std::shared_ptr<spdlog::logger> Emulator::logger = nullptr;

int main()
{
    Emulator::initializeLogger();

    try
    {
        Bus::Bus bus;

        uint8_t rom_data[] = {0x69};

        auto boot_rom = std::make_shared<Memory::MemoryDevice>(0x0000, 0x1FFF, rom_data, 0x2000);
        auto extended_rom = std::make_shared<Memory::MemoryDevice>(0x2000, 0x3FFF, nullptr, 0x0);
        auto ram = std::make_shared<Memory::MemoryDevice>(0x4000, 0xFFFF, nullptr, 0x0);

        bus.attachDevice(boot_rom, Bus::Permission::Read);
        bus.attachDevice(extended_rom, Bus::Permission::Read);
        bus.attachDevice(ram, Bus::Permission::Read | Bus::Permission::Write);

        Emulator::logger->info("Running eb16");

        // Test loop
        bool quit = false;
        uint16_t pc = 0x0000;
        while (!quit)
        {
            uint8_t opcode = bus.read(pc);
            Emulator::logger->info("Executing opcode 0x{:02X} at PC 0x{:04X}", opcode, pc);

            if (opcode == 0x00) // Halt
            {
                quit = true;
                break;
            }

            pc += 1;
        }

        Emulator::logger->info("Finished running eb16");
    }
    catch (const std::exception &e)
    {
        Emulator::logger->error("Error: {}", e.what());
    }

    return 0;
}
