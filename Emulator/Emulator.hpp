#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Emulator
{
public:
    static std::shared_ptr<spdlog::logger> logger;

    static void initializeLogger()
    {
        if (!logger)
        {
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::debug);
            logger = std::make_shared<spdlog::logger>("main", console_sink);
            logger->set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
        }
    }
};
