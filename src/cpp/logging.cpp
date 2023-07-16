#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/CsvFormatter.h>
#include <plog/Formatters/TxtFormatter.h>

#include "logging.h"

namespace Logging
{
    void setup_logs() {
        static plog::RollingFileAppender<plog::CsvFormatter> fileAppender("log.csv", 8000, 3); // Create the 1st appender.
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender; // Create the 2nd appender.
        plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender); // Initialize the logger with the both appenders.
    }
} // namespace Logging
