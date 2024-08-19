#pragma once

#include <memory>
#include <vector>
#include <source_location>
#include <thread>
#include <format>

#include <quicklog/common.h>
#include <quicklog/details/tool.h>
#include <quicklog/logformat.h>
#include <quicklog/appender/appender.h>

namespace quicklog {
// 日志器
class Logger
{
public:
    using ptr = std::shared_ptr<Logger>;
private:
    std::string m_logName;//日志名称
    LogLevel m_level;//日志级别
    LogLevel m_auto_flush_level{LogLevel::Warning};//自动刷新的级别
    std::vector<Appender::ptr> m_appenderList;//日志输出列表
public:
    explicit Logger(std::string name)
        : m_logName(name)
        , m_level(LogLevel::All)
        , m_auto_flush_level(LogLevel::Warning)
        
    {}
    Logger(std::string name, LogLevel level)
        : m_logName(name)
        , m_level(level)
        , m_auto_flush_level(LogLevel::Warning)
    {}
    Logger(std::string name, LogLevel level, LogLevel auto_flush_level)
        : m_logName(name)
        , m_level(level)
        , m_auto_flush_level(auto_flush_level)
    {}
    virtual ~Logger() = default;

    Logger(const Logger& other);
    Logger(Logger&& other) noexcept;
    Logger& operator=(Logger other) noexcept;

    void swap(quicklog::Logger& other) noexcept;

public://日志消息封装函数
    template <typename... Args>
    void log( LogLevel level, std::format_string<Args...> format, Args &&...args)
    {
        std::string content = std::format(format, args...);
        
        //打印日志
        for (auto& iter : m_appenderList)
        {
            iter->log(content);
        }

        if (should_flush(level))
        {
            flush();
        }
    }
    
    void log(const log_msg& logMessage);
public://常规日志打印函数
    template<typename T>//进行编译时期检查，确保T类型能被format格式化
    void Trace(const T& message \
        , unsigned int threadId = getThreadId() \
        , std::source_location location = std::source_location::current())
    {
        if (LogLevel::Trace >= m_level)
        {
            auto logMessage = log_msg(TimePoint(), location, threadId, LogLevel::Trace, message);
            log(logMessage);
        }
    }
    template<typename T>
    void Debug(const T& message \
        , unsigned int threadId = getThreadId() \
        , std::source_location location = std::source_location::current())
    {
        if (LogLevel::Debug >= m_level)
        {

            auto logMessage = log_msg(TimePoint(), location, threadId, LogLevel::Debug, message);
            log(logMessage);
        }
    }
    template<typename T>
    void Info(const T& message \
        , unsigned int threadId = getThreadId() \
        , std::source_location location = std::source_location::current())
    {
        if (LogLevel::Info >= m_level)
        {
            auto logMessage = log_msg(TimePoint(), location, threadId, LogLevel::Info, message);
            log(logMessage);
        }
    }
    template<typename T>
    void Warning(const T& message \
        , unsigned int threadId = getThreadId() \
        , std::source_location location = std::source_location::current())
    {
        if (LogLevel::Warning >= m_level)
        {
            auto logMessage = log_msg(TimePoint(), location, threadId, LogLevel::Warning, message);
            log(logMessage);
        }
    }
    template<typename T>
    void Error(const T& message \
        , unsigned int threadId = getThreadId() \
        , std::source_location location = std::source_location::current())
    {
        if (LogLevel::Error >= m_level)
        {
            auto logMessage = log_msg(TimePoint(), location, threadId, LogLevel::Error, message);
            log(logMessage);
        }
    }
    template<typename T>
    void Fatal(const T& message \
        , unsigned int threadId = getThreadId() \
        , std::source_location location = std::source_location::current())
    {
        if (LogLevel::Fatal >= m_level)
        {
            auto logMessage = log_msg(TimePoint(), location, threadId, LogLevel::Fatal, message);
            log(logMessage);
        }
    }
public://可变参数日志打印函数
    template <typename... Args>
    void Trace(std::format_string<Args...> format, Args &&...args)
    {
        if (LogLevel::Trace >= m_level)
        {
            log(LogLevel::Trace, format, std::forward<Args>(args)...);
        }       
    }

    template <typename... Args>
    void Debug(std::format_string<Args...> format, Args &&...args) 
    {
        if (LogLevel::Debug >= m_level)
        {
            log(LogLevel::Debug, format, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    void Info(std::format_string<Args...> format, Args &&...args)
    {
        if (LogLevel::Info >= m_level)
        {
            log(LogLevel::Info, format, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    void Warning(std::format_string<Args...> format, Args &&...args)
    {
        if (LogLevel::Warning >= m_level)
        {
            log(LogLevel::Warning, format, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    void Error(std::format_string<Args...> format, Args &&...args)
    {
        if (LogLevel::Error >= m_level)
        {
            log(LogLevel::Error, format, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    void Fatal(std::format_string<Args...> format, Args &&...args)
    {
        if (LogLevel::Fatal >= m_level)
        {
            log(LogLevel::Fatal, format, std::forward<Args>(args)...);
        }
    }
public://日志类的基本操作
    void flush();
         
    const LogLevel getLevel() const;

    const LogLevel getAutoLevel() const;

    const std::string getName() const;
#ifdef USE_RUN_FORMAT
    const std::vector<std::string> getPattern() const;

    void setPattern(const std::string& pattern);
#endif
    void setLevel(LogLevel level);

    void setAutoLevel(LogLevel level);

    void setTimeType(TimeType timeType);

    void addAppender(Appender::ptr appender);

    void clearAppender();
private:
    bool should_flush(const LogLevel level) const;
};
}

#include "logger-inl.h"