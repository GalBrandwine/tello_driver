#include "log_data/LogData.hpp"
namespace tello_protocol
{
    LogImuAtti &LogData::GetLogImuAtti()
    {
        std::lock_guard<std::mutex> lk(m_log_data_mutex);
        return m_LogImuAtti;
    };
    LogNewMvoFeedback &LogData::GetLogMvo()
    {
        std::lock_guard<std::mutex> lk(m_log_data_mutex);
        return m_LogMvoFeedback;
    };

    void LogData::Update(const std::vector<unsigned char> &data)
    {
        std::lock_guard<std::mutex> lk(m_log_data_mutex);

        m_count++;
        short pos = 0;
        while (pos < data.size() - 2)
        {
            if (data[pos + 0] != 0x55) // Little endian
            {
                return;
            }

            short length = int16(data[pos + 1], data[pos + 2]);
            auto checksum = data[pos + 3];
            unsigned short id = uint16(data[pos + 4], data[pos + 5]);
            unsigned char xorval = data[pos + 6];

            auto payload = std::vector<unsigned char>(data.begin() + pos + 10, data.begin() + pos + 10 + length - 12);
            for (auto &i : payload)
            {
                i ^= xorval;
            }

            if (id == ID_NEW_MVO_FEEDBACK)
            {
                m_LogMvoFeedback.Update(payload, m_count);
            }
            else if (id == ID_IMU_ATTI)
            {
                m_LogImuAtti.Update(payload, m_count);
            }
            else
            {
                for (const auto &unknownID : m_UnknownIDs)
                {
                    if (id != unknownID)
                    {
                        m_logger->error("UNHANDLED LOG DATA: id={}, length={}", id, length - 12);
                        m_UnknownIDs.push_back(id);
                    }
                }
            }

            pos += length;
        }
    }

    LogData::LogData(std::shared_ptr<spdlog::logger> logger)
        : m_logger(logger),
          m_LogMvoFeedback(logger),
          m_LogImuAtti(logger)
    {
        m_logger->info(m_logger->name() + " Initiated!");
    }

    LogData::~LogData()
    {
        m_logger->info(m_logger->name() + " Destructing.");
    }
} // namespace tello_protocol
