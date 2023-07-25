#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/CsvFormatter.h>
#include <plog/Formatters/TxtFormatter.h>

#include "logging.h"
#include "settings/settings_globals.h"

namespace Logging
{
    void setup_logs() {
#ifndef CLI_BUILD
        static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("log.csv", 24000, 1); // Create the 1st appender.
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender; // Create the 2nd appender.
        plog::init((plog::Severity)Global::settings.log_level, &fileAppender).addAppender(&consoleAppender); // Initialize the logger with the both appenders.
#else
        static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("cli-log.csv", 24000, 1); // Create the 1st appender.
        plog::init((plog::Severity)Global::settings.log_level, &fileAppender);
#endif

    }
} // namespace Logging
