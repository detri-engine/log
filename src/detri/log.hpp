#pragma once

#include <memory>
#include <string>
#include <fmt/format.h>

namespace detri
{
    enum class log_level
    {
        trace,
        debug,
        info,
        warn,
        error,
        critical,
        off
    };

    class logger
    {
        friend class log_service;
    public:
        logger() = delete;
        ~logger() = default;

        logger(const logger&) = delete;
        logger& operator=(const logger&) = delete;
        logger(logger&&) = default;
        logger& operator=(logger&&) = default;

        void set_level(log_level level) const;
        void write(log_level level, const char* msg) const;
        void flush() const;

        template<typename... Args>
        void trace(fmt::format_string<Args...> msg, Args&&... args) const
        {
            const auto formatted = fmt::format(msg, std::forward<Args>(args)...);
            write(log_level::trace, formatted.c_str());
        }

        template<typename... Args>
        void debug(fmt::format_string<Args...> msg, Args&&... args) const
        {
            const auto formatted = fmt::format(msg, std::forward<Args>(args)...);
            write(log_level::debug, formatted.c_str());
        }

        template<typename... Args>
        void info(fmt::format_string<Args...> msg, Args&&... args) const
        {
            const auto formatted = fmt::format(msg, std::forward<Args>(args)...);
            write(log_level::info, formatted.c_str());
        }

        template<typename... Args>
        void warn(fmt::format_string<Args...> msg, Args&&... args) const
        {
            const auto formatted = fmt::format(msg, std::forward<Args>(args)...);
            write(log_level::warn, formatted.c_str());
        }

        template<typename... Args>
        void error(fmt::format_string<Args...> msg, Args&&... args) const
        {
            const auto formatted = fmt::format(msg, std::forward<Args>(args)...);
            write(log_level::error, formatted.c_str());
        }

        template<typename... Args>
        void critical(fmt::format_string<Args...> msg, Args&&... args) const
        {
            const auto formatted = fmt::format(msg, std::forward<Args>(args)...);
            write(log_level::critical, formatted.c_str());
        }
    protected:
        explicit logger(const std::string& name);
    private:
        struct impl;
        std::unique_ptr<impl> m_impl;
    };

    struct log_config
    {
        log_level level = log_level::info;
        std::string base_filename = "engine_log.txt";
    };

    class log_service
    {
    public:
        explicit log_service(const log_config& config);
        ~log_service();

        log_service(const log_service&) = delete;
        log_service& operator=(const log_service&) = delete;
        log_service(log_service&&) = delete;
        log_service& operator=(log_service&&) = delete;

        [[nodiscard]] logger get(const std::string& name) const;
    };
}