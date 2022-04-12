#include <edoren/logging/LogHandler.hpp>

namespace edoren {

void LogHandler::setEnabled(bool enabled) {
    m_enabled = enabled;
}

bool LogHandler::isEnabled() const {
    return m_enabled;
}

}  // namespace edoren
