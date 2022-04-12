#pragma once

#include <edoren/StringView.hpp>
#include <edoren/logging/LogPriority.hpp>

namespace edoren {

class LogHandler {
public:
    virtual void logMessage(LogPriority priority, StringView tag, StringView message) = 0;

    void setEnabled(bool enabled);
    bool isEnabled() const;

private:
    bool m_enabled{true};
};

}  // namespace edoren
