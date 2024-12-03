#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace Bus
{
    enum class Permission
    {
        Read = 1,
        Write = 2,
        ReadWrite = Read | Write,
    };

    inline Permission operator|(Permission lhs, Permission rhs)
    {
        return static_cast<Permission>(
            static_cast<int>(lhs) | static_cast<int>(rhs));
    }

    inline bool hasPermission(Permission perms, Permission check)
    {
        return (static_cast<int>(perms) & static_cast<int>(check)) ==
               static_cast<int>(check);
    }

    class BusDevice
    {
    public:
        virtual ~BusDevice() = default;
        virtual bool handles(uint16_t address) const = 0;
        virtual uint8_t read(uint16_t address) const = 0;
        virtual void write(uint16_t address, uint8_t value) = 0;
    };

    class Bus
    {
    public:
        void attachDevice(
            std::shared_ptr<BusDevice> device,
            Permission permission);
        uint8_t read(uint16_t address) const;
        void write(uint16_t address, uint8_t value);

    private:
        std::vector<std::shared_ptr<BusDevice>> devices_;
        std::unordered_map<std::shared_ptr<BusDevice>, Permission> permissions_;
    };
}
