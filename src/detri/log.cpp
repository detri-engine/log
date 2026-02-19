#include "detri/log.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace detri
{
    struct logger::impl
    {
        std::shared_ptr<spdlog::logger> logger;
    };

    void logger::set_level(log_level level) const
    {
        m_impl->logger->set_level(static_cast<spdlog::level::level_enum>(level));
    }

    void logger::write(log_level level, const char* msg) const
    {
        m_impl->logger->log(static_cast<spdlog::level::level_enum>(level), msg);
    }

    void logger::flush() const
    {
        m_impl->logger->flush();
    }

    logger::logger(const std::string& name) : m_impl{std::make_unique<impl>()}
    {
        if (const auto existing = spdlog::get(name))
        {
            m_impl->logger = existing;
        } else
        {
            m_impl->logger = spdlog::default_logger()->clone(name);
            spdlog::register_logger(m_impl->logger);
        }
    }

    log_service::log_service(const log_config& config)
    {
        auto spdlog_default = spdlog::default_logger();
        spdlog_default->set_level(static_cast<spdlog::level::level_enum>(config.level));
        spdlog_default->sinks().emplace_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(config.base_filename, 1024 * 1024 * 2, 3, true));
    }

    log_service::~log_service()
    {
        spdlog::shutdown();
    }

    // Intentionally a member to ensure logging backend is configured before loggers can be created
    logger log_service::get(const std::string& name) const // NOLINT(*-convert-member-functions-to-static)
    {
        return logger{name};
    }
}
