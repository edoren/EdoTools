#pragma once

#include <edoren/StringView.hpp>
#include <edoren/container/List.hpp>
#include <edoren/logging/LogHandler.hpp>
#include <edoren/logging/LogPriority.hpp>
#include <edoren/util/Platform.hpp>
#include <edoren/util/Singleton.hpp>

// On Windows undefine this anoying macro defined by windows.h
#if PLATFORM_IS(PLATFORM_WINDOWS)
    #undef ERROR
#endif

namespace edoren {

class EDOTOOLS_API LogManager : public Singleton<LogManager> {
public:
    LogManager();
    ~LogManager();

    void initialize();
    void shutdown();

    template <typename T, class... Args>
    T* registerLogHandler(Args&&... args) {
        m_handlers.emplaceBack(std::make_unique<T>(std::forward<Args>(args)...));
        return reinterpret_cast<T*>(m_handlers.back().get());
    }

    void verbose(StringView tag, StringView message);
    void debug(StringView tag, StringView message);
    void info(StringView tag, StringView message);
    void warning(StringView tag, StringView message);
    void error(StringView tag, StringView message);
    void fatal(StringView tag, StringView message);

    void logMessage(LogPriority priority, StringView tag, StringView message);

private:
    List<std::unique_ptr<LogHandler>> m_handlers;
};

inline void LogVerbose(StringView tag, StringView message) {
    LogManager::GetInstance().verbose(tag, message);
}

template <typename... Args>
inline void LogVerbose(StringView tag, StringView message, Args&&... args) {
    auto internalStringView = fmt::string_view(message.getData(), message.getSize());
    auto formated = fmt::vformat(internalStringView, fmt::make_format_args(args...));
    LogVerbose(tag, StringView(formated.data()));
}

inline void LogDebug(StringView tag, StringView message) {
    LogManager::GetInstance().debug(tag, message);
}

template <typename... Args>
inline void LogDebug(StringView tag, StringView message, Args&&... args) {
    auto internalStringView = fmt::string_view(message.getData(), message.getSize());
    auto formated = fmt::vformat(internalStringView, fmt::make_format_args(args...));
    LogDebug(tag, StringView(formated.data()));
}

inline void LogInfo(StringView tag, StringView message) {
    LogManager::GetInstance().info(tag, message);
}

template <typename... Args>
inline void LogInfo(StringView tag, StringView message, Args&&... args) {
    auto internalStringView = fmt::string_view(message.getData(), message.getSize());
    auto formated = fmt::vformat(internalStringView, fmt::make_format_args(args...));
    LogInfo(tag, StringView(formated.data()));
}

inline void LogWarning(StringView tag, StringView message) {
    LogManager::GetInstance().warning(tag, message);
}

template <typename... Args>
inline void LogWarning(StringView tag, StringView message, Args&&... args) {
    auto internalStringView = fmt::string_view(message.getData(), message.getSize());
    auto formated = fmt::vformat(internalStringView, fmt::make_format_args(args...));
    LogWarning(tag, StringView(formated.data()));
}

inline void LogError(StringView tag, StringView message) {
    LogManager::GetInstance().error(tag, message);
}

template <typename... Args>
inline void LogError(StringView tag, StringView message, Args&&... args) {
    auto internalStringView = fmt::string_view(message.getData(), message.getSize());
    auto formated = fmt::vformat(internalStringView, fmt::make_format_args(args...));
    LogError(tag, StringView(formated.data()));
}

inline void LogFatal(StringView tag, StringView message) {
    LogManager::GetInstance().fatal(tag, message);
}

template <typename... Args>
inline void LogFatal(StringView tag, StringView message, Args&&... args) {
    auto internalStringView = fmt::string_view(message.getData(), message.getSize());
    auto formated = fmt::vformat(internalStringView, fmt::make_format_args(args...));
    LogFatal(tag, StringView(formated.data()));
}

}  // namespace edoren
