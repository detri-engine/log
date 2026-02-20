#include <spdlog/sinks/base_sink.h>
#include <memory>
#include <string>
#include <vector>

#include "spdlog/spdlog.h"
#include "detri/log.hpp"
#include "gtest/gtest.h"


template<typename Mutex>
class test_sink : public spdlog::sinks::base_sink<Mutex>
{
public:
    test_sink() = default;
    ~test_sink() override = default;

    explicit test_sink(std::unique_ptr<spdlog::formatter> formatter) : spdlog::sinks::base_sink<Mutex>(std::move(formatter))
    {
        m_logs.reserve(16);
    }

    [[nodiscard]] const std::vector<spdlog::details::log_msg_buffer>& logs() const
    {
        return m_logs;
    }
protected:
    void sink_it_(const spdlog::details::log_msg &msg) override
    {
        m_logs.emplace_back(msg);
    }
    void flush_() override
    {
        m_logs.clear();
    }
private:
    std::vector<spdlog::details::log_msg_buffer> m_logs;
};

class DetriEngineLogs : public testing::Test
{
protected:
    detri::log_service service;
    detri::logger logger;
    std::shared_ptr<spdlog::logger> raw_logger;
    std::shared_ptr<test_sink<spdlog::details::null_mutex>> sink;

    DetriEngineLogs() : service{detri::log_config{detri::log_level::trace, "na"}},
        logger{service.get("test")},
        raw_logger{spdlog::get("test")}
    {
        raw_logger->sinks().clear();
        raw_logger->set_level(spdlog::level::trace);
        sink = std::make_shared<test_sink<spdlog::details::null_mutex>>();
        raw_logger->sinks().emplace_back(sink);
    }
};

TEST_F(DetriEngineLogs, LoggerWritesToSink)
{
    logger.info("Hello, world!");
    ASSERT_EQ(sink->logs().size(), 1);
    const auto& entry = sink->logs().back();
    ASSERT_EQ(entry.level, spdlog::level::info);
    ASSERT_EQ(std::string(entry.logger_name.data(), entry.logger_name.size()), "test");
    const auto& payload = entry.payload;
    ASSERT_EQ(std::string(payload.data(), payload.size()), "Hello, world!");
}
