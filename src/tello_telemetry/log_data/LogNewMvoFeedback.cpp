#include "log_data/LogNewMvoFeedback.hpp"

namespace tello_protocol
{

    LogNewMvoFeedback::LogNewMvoFeedback(std::shared_ptr<spdlog::logger> logger)
        : m_logger(logger)
    {
    }

    LogNewMvoFeedback::~LogNewMvoFeedback()
    {
    }
    void LogNewMvoFeedback::Update(const std::vector<unsigned char> &data, int count)
    {
        m_count = count;
        short tempS;
        std::memcpy(&tempS, &data[2], sizeof(short));
        m_Vel.x = float(tempS) / 100.0f;

        std::memcpy(&tempS, &data[4], sizeof(short));
        m_Vel.y = float(tempS) / 100.0f;

        std::memcpy(&tempS, &data[6], sizeof(short));
        m_Vel.z = float(tempS) / 100.0f;

        float tempF;
        std::memcpy(&tempF, &data[8], sizeof(float));
        m_Pos.x = tempF;

        std::memcpy(&tempF, &data[12], sizeof(float));
        m_Pos.y = tempF;

        std::memcpy(&tempF, &data[16], sizeof(float));
        m_Pos.z = tempF;
    }

    const Vec3 &LogNewMvoFeedback::GetVel() const
    {
        return m_Vel;
    }
    const Vec3 &LogNewMvoFeedback::GetPos() const
    {
        return m_Pos;
    }
} // namespace tello_protocol
