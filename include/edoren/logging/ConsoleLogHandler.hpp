#pragma once

#include <edoren/String.hpp>
#include <edoren/logging/LogHandler.hpp>
#include <edoren/logging/LogPriority.hpp>

namespace edoren {

class EDOTOOLS_API ConsoleLogHandler : public LogHandler {
public:
    ConsoleLogHandler(StringView appName);
    ~ConsoleLogHandler();

    void logMessage(LogPriority priority, StringView tag, StringView message) override;

private:
    String m_appName;
};

}  // namespace edoren
