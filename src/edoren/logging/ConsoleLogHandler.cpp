#include <edoren/logging/ConsoleLogHandler.hpp>

#include <edoren/util/Platform.hpp>

namespace edoren {

namespace {
#if PLATFORM_IS(PLATFORM_ANDROID)
android_LogPriority sAndroidLogPriorities[] = {ANDROID_LOG_UNKNOWN,
                                               ANDROID_LOG_VERBOSE,
                                               ANDROID_LOG_DEBUG,
                                               ANDROID_LOG_INFO,
                                               ANDROID_LOG_WARN,
                                               ANDROID_LOG_ERROR,
                                               ANDROID_LOG_FATAL};
#endif

}  // namespace

ConsoleLogHandler::ConsoleLogHandler(StringView appName) : m_appName(appName) {}

ConsoleLogHandler::~ConsoleLogHandler() {}

void ConsoleLogHandler::logMessage(LogPriority priority, StringView tag, StringView message) {
#if PLATFORM_IS(PLATFORM_ANDROID)
    __android_log_write(sAndroidLogPriorities[static_cast<int>(priority)], m_appName.getData(), log_message.getData());
#else
    fputs(message.getData(), stdout);
    fputs("\n", stdout);
#endif
}

}  // namespace edoren
