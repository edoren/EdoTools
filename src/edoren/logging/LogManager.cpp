#include <edoren/logging/LogManager.hpp>

#include <edoren/String.hpp>

#include <utility>

#if PLATFORM_IS(PLATFORM_ANDROID)
    #include <android/log.h>
#endif

namespace edoren {

namespace {

const char* sLogPriorityNames[] = {nullptr, "VERBOSE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

String DefaultLogCallback(LogPriority priority, StringView tag, StringView message) {
    const char* priorityName = sLogPriorityNames[static_cast<int>(priority)];

    // Get the current system hour
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);

    // Format the log message
    fmt::string_view tagView(tag.getData(), tag.getSize());
    fmt::string_view messageView(message.getData(), message.getSize());
    return "[{:02d}:{:02d}:{:02d}] [{}/{}] : {}"_format(tm->tm_hour,
                                                        tm->tm_min,
                                                        tm->tm_sec,
                                                        tagView,
                                                        priorityName,
                                                        messageView);
}

}  // namespace

LogManager::LogManager() {}

LogManager::~LogManager() = default;

void LogManager::initialize() {}

void LogManager::shutdown() {}

void LogManager::verbose(StringView tag, StringView message) {
    logMessage(LogPriority::VERBOSE, tag, message);
}

void LogManager::debug(StringView tag, StringView message) {
    logMessage(LogPriority::DEBUG, tag, message);
}

void LogManager::info(StringView tag, StringView message) {
    logMessage(LogPriority::INFO, tag, message);
}

void LogManager::warning(StringView tag, StringView message) {
    logMessage(LogPriority::WARN, tag, message);
}

void LogManager::error(StringView tag, StringView message) {
    logMessage(LogPriority::ERROR, tag, message);
}

void LogManager::fatal(StringView tag, StringView message) {
    logMessage(LogPriority::FATAL, tag, message);
}

void LogManager::logMessage(LogPriority priority, StringView tag, StringView message) {
    if (m_handlers.getSize() == 0) {
        return;
    }

#ifndef EDOTOOLS_DEBUG
    if (priority == LogPriority::DEBUG) {
        return;
    }
#endif

    String logMessage = DefaultLogCallback(priority, tag, message);

    for (auto& handler : m_handlers) {
        if (handler->isEnabled()) {
            handler->logMessage(priority, tag, logMessage);
        }
    }

    if (priority == LogPriority::FATAL) {
        std::exit(1);  // TMP
    }
}

}  // namespace edoren
