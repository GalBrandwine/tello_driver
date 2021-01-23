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

        // if (data[pos + 0] != 0x55) // Little endian
        // {
        //     return;
        // }
        // std::cout << "data.size(): " << data.size() << "\n";
        while (pos < data.size() - 2)
        {
            // std::cout << "pos: " << pos << "\n";
            // short length = int16(data[pos + 1], data[pos + 2]);

            short length; // = uint16(data[pos + 4], data[pos + 5]);
            std::memcpy(&length, &data[pos + 1], sizeof(short));

            auto checksum = data[pos + 3];

            unsigned short id; // = uint16(data[pos + 4], data[pos + 5]);
            std::memcpy(&id, &data[pos + 4], sizeof(unsigned short));

            auto xorval = data[pos + 6];
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
                if (m_UnknownIDs.empty())
                {
                    m_UnknownIDs.push_back(id);
                }

                auto result1 = std::find(m_UnknownIDs.begin(), m_UnknownIDs.end(), id);
                if (result1 == m_UnknownIDs.end())
                {
                    m_logger->error("UNHANDLED LOG DATA: id={}, length={}", id, length - 12);
                    m_UnknownIDs.push_back(id);
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
        // m_logger->info(m_logger->name() + " Destructing.");
    }
} // namespace tello_protocol
