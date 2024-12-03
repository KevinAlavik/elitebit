#include "Bus.hpp"
#include "Emulator.hpp"
#include <cstdio>
#include <stdexcept>
#include <exception>

namespace Bus
{
    void Bus::attachDevice(std::shared_ptr<BusDevice> device, Permission permission)
    {
        if (!device)
        {
            Emulator::logger->error("Cannot attach null device");
            throw std::invalid_argument("Cannot attach null device");
        }

        devices_.push_back(device);
        permissions_[device] = permission;
    }

    uint8_t Bus::read(uint16_t address) const
    {
        for (const auto &device : devices_)
        {
            if (device->handles(address))
            {
                auto permission = permissions_.at(device);
                if (!hasPermission(permission, Permission::Read))
                {
                    std::string errorMessage = "Read access denied for device at address 0x" +
                                               std::to_string(address);
                    Emulator::logger->error(errorMessage);
                    throw std::runtime_error(errorMessage);
                }
                uint8_t value = device->read(address);
                return value;
            }
        }

        std::string errorMessage = "Read address 0x" + std::to_string(address) + " out of range";
        Emulator::logger->error(errorMessage);
        throw std::out_of_range(errorMessage);
    }

    void Bus::write(uint16_t address, uint8_t value)
    {
        for (const auto &device : devices_)
        {
            if (device->handles(address))
            {
                auto permission = permissions_.at(device);
                if (!hasPermission(permission, Permission::Write))
                {
                    std::string errorMessage = "Write access denied for device at address 0x" +
                                               std::to_string(address);
                    Emulator::logger->error(errorMessage);
                    throw std::runtime_error(errorMessage);
                }
                device->write(address, value);
                return;
            }
        }

        std::string errorMessage = "Write address 0x" + std::to_string(address) + " out of range";
        Emulator::logger->error(errorMessage);
        throw std::out_of_range(errorMessage);
    }

}
