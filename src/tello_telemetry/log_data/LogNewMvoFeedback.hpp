#pragma once
#include "protocol.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "utils/telemetry_data/TelemetryData.hpp"
namespace tello_protocol
{
    /**
     * @brief Supply Position and velocity log:
     * Class for extracting Position and velocity information from the raw bytestream.
     * Based on the [TelloPy](https://github.com/hanyazou/TelloPy/blob/develop-0.7.0/tellopy/_internal/protocol.py#L353) extracting methods.
     * 
     */
    class LogNewMvoFeedback
    {
    public:
        const int GetUpdateCounter() const { return m_count; };
        LogNewMvoFeedback(std::shared_ptr<spdlog::logger>);
        ~LogNewMvoFeedback();

        /**
         * @brief Parse movement data from preprocessed [still raw] data.
         * 
         * @param[in] data preprocessed data, stripped from un-important bytes. Contain raw information about pos and vel.
         * @param[in] count amount of preprocessed data. Update function expects the preprocessed data to be in curtain size.
         */
        void Update(const std::vector<unsigned char> &data, int count = 0);

        /**
         * @brief Get the Pos object
         * 
         * @return const Vec3& 
         */
        const Vec3 &GetPos() const;

        /**
         * @brief Get the Vel object
         * 
         * @return const Vec3& 
         */
        const Vec3 &GetVel() const;


        friend std::ostream &operator<<(std::ostream &os, const LogNewMvoFeedback &dt)
        {
            os << "VEL: " << dt.m_Vel.x << " " << dt.m_Vel.y << " " << dt.m_Vel.z << " POS: " << dt.m_Pos.x << " " << dt.m_Pos.y << " " << dt.m_Pos.z << '\n';
            return os;
        }

    private:
        std::shared_ptr<spdlog::logger> m_logger;
        const std::string m_data;
        int m_count = 0;
        Vec3 m_Pos;
        Vec3 m_Vel;
    };

} // namespace tello_protocol