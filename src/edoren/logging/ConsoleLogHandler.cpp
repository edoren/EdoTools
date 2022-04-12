#include <edoren/logging/ConsoleLogHandler.hpp>

#include <edoren/util/Platform.hpp>

namespace edoren {

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
