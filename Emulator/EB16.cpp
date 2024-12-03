#include "EB16.hpp"
#include "Emulator.hpp"

namespace EB16
{
    CPU::CPU(Bus::Bus &bus) : pc_(0), x_(0), y_(0), z_(0), bus_(bus)
    {
        instructions_[static_cast<uint8_t>(InstructionType::NOP)] = std::bind(&CPU::executeNOP, this);
        instructions_[static_cast<uint8_t>(InstructionType::HLT)] = std::bind(&CPU::executeHLT, this);
        instructions_[static_cast<uint8_t>(InstructionType::LDX_IMM)] = std::bind(&CPU::executeLDX_IMM, this);
        instructions_[static_cast<uint8_t>(InstructionType::LDX_ADDR)] = std::bind(&CPU::executeLDX_ADDR, this);
        instructions_[static_cast<uint8_t>(InstructionType::LDY_IMM)] = std::bind(&CPU::executeLDY_IMM, this);
        instructions_[static_cast<uint8_t>(InstructionType::LDY_ADDR)] = std::bind(&CPU::executeLDY_ADDR, this);
        instructions_[static_cast<uint8_t>(InstructionType::LDZ_IMM)] = std::bind(&CPU::executeLDZ_IMM, this);
        instructions_[static_cast<uint8_t>(InstructionType::LDZ_ADDR)] = std::bind(&CPU::executeLDZ_ADDR, this);
        instructions_[static_cast<uint8_t>(InstructionType::MOV)] = std::bind(&CPU::executeMOV, this);
    }

    void CPU::run()
    {
        while (true)
        {
            fetchAndExecute();
        }
    }

    void CPU::fetchAndExecute()
    {
        uint8_t opcode = bus_.read(pc_);
        pc_++;

        if (instructions_.find(opcode) != instructions_.end())
        {
            instructions_[opcode]();
        }
        else
        {
            Emulator::logger->error("Invalid opcode 0x{:02X} at address 0x{:04X}", opcode, pc_ - 1);
            throw std::runtime_error("Invalid opcode encountered");
        }
    }

    void CPU::executeNOP()
    {
        Emulator::logger->info("Executed NOP instruction");
    }

    void CPU::executeHLT()
    {
        Emulator::logger->info("Executed HLT instruction, halting CPU");
        throw std::runtime_error("CPU Halted");
    }

    void CPU::executeLDX_IMM()
    {
        uint8_t value = bus_.read(pc_);
        x_ = value;
        pc_++;
        Emulator::logger->info("Executed LDX_IMM, loaded 0x{:02X} into X", x_);
    }

    void CPU::executeLDX_ADDR()
    {
        uint16_t address = bus_.read(pc_) | (bus_.read(pc_ + 1) << 8);
        uint8_t value = bus_.read(address);
        x_ = value;
        pc_ += 2;
        Emulator::logger->info("Executed LDX_ADDR, loaded 0x{:02X} from address 0x{:04X} into X", x_, address);
    }

    void CPU::executeLDY_IMM()
    {
        uint8_t value = bus_.read(pc_);
        y_ = value;
        pc_++;
        Emulator::logger->info("Executed LDY_IMM, loaded 0x{:02X} into Y", y_);
    }

    void CPU::executeLDY_ADDR()
    {
        uint16_t address = bus_.read(pc_) | (bus_.read(pc_ + 1) << 8);
        uint8_t value = bus_.read(address);
        y_ = value;
        pc_ += 2;
        Emulator::logger->info("Executed LDY_ADDR, loaded 0x{:02X} from address 0x{:04X} into Y", y_, address);
    }

    void CPU::executeLDZ_IMM()
    {
        uint8_t value = bus_.read(pc_);
        z_ = value;
        pc_++;
        Emulator::logger->info("Executed LDZ_IMM, loaded 0x{:02X} into Z", z_);
    }

    void CPU::executeLDZ_ADDR()
    {
        uint16_t address = bus_.read(pc_) | (bus_.read(pc_ + 1) << 8);
        uint8_t value = bus_.read(address);
        z_ = value;
        pc_ += 2;
        Emulator::logger->info("Executed LDZ_ADDR, loaded 0x{:02X} from address 0x{:04X} into Z", z_, address);
    }

    void CPU::executeMOV()
    {
        uint8_t source = bus_.read(pc_);
        uint8_t dest = bus_.read(pc_ + 1);
        pc_ += 2;

        switch (dest)
        {
        case 0x00:
            x_ = (source == 0x00) ? x_ : (source == 0x01) ? y_
                                                          : z_;
            break;
        case 0x01:
            y_ = (source == 0x00) ? x_ : (source == 0x01) ? y_
                                                          : z_;
            break;
        case 0x02:
            z_ = (source == 0x00) ? x_ : (source == 0x01) ? y_
                                                          : z_;
            break;
        default:
            Emulator::logger->error("Invalid MOV destination register");
        }

        Emulator::logger->info("Executed MOV, moved data from register {} to register {}", source, dest);
    }

    void CPU::dump()
    {
        Emulator::logger->info("CPU State Dump:");
        Emulator::logger->info("----------------");
        Emulator::logger->info("PC (Program Counter): 0x{:04X}", pc_);
        Emulator::logger->info("X Register: 0x{:02X}", x_);
        Emulator::logger->info("Y Register: 0x{:02X}", y_);
        Emulator::logger->info("Z Register: 0x{:02X}", z_);
        Emulator::logger->info("----------------");
    }
}
