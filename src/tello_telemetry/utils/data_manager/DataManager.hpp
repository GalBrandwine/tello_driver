#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include "utils/tello_observer/IDataMgrSubject.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <sstream>

#include "ILogHeaderMsgDataManager.hpp"
#include "ILogDataMsgDataManager.hpp"
#include "IConnAckMsgDataManager.hpp"

namespace tello_protocol
{
    struct ConnectionInformation
    {
        bool IsConnected = false;
    };

    /**
     * @brief Store LogHeaderMsg information
     * 
     */
    struct LogHeaderInformation
    {
        std::vector<unsigned char> BuildDate, DJILogVersion, LogId;
    };

    /**
     * @brief Store and maintain data from Observers.
     * Apon each new data from a observer:
     * * Update these new data (if new)
     * * Notify all attached subscribers to this new data.
     * 
     */
    class DataManager
        : public IDataMgrSubject,
          public ILogHeaderMsgDataManager,
          public ILogDataMsgDataManager,
          public IConnAckMsgDataManager
    {
    public:
        /**
         * @brief Overriding IConnAckMsgDataManager.
         * This function will be called by the ConnReqAck observer, which is attached to TelloTelemetry.
         * 
         */
        void SetConnReqAck() override;

        /**
         * @brief Overriding ILogDataMsgDataManager;
         * Get new Position and Movement data out of LogData
         * 
         * @param data std::shared_ptr<ILogDataGetter> filled with preprocessed log information.
         */
        void SetLogData(const std::shared_ptr<ILogDataGetter> log_data_processor) override;

        /**
         * @section Overriding ILogHeaderMsgDataManager interface.
         * 
         * 
         */
        void SetLogID(const unsigned short id) override;
        void SetBuildDate(const std::vector<unsigned char> &build_date) override;
        void SetDJILogVersion(const std::vector<unsigned char> &log_version) override;

        void Notify(const OBSERVERS) override;

        void Attach(const OBSERVERS observer_type, IObserver *observer) override;
        void Attach(const OBSERVERS observer_type, IPositionVelocityObserver *observer) override;
        // void Attach(const OBSERVERS observer_type, IConnectionEstablishedObserver *observer) override;

        /**
         * @brief Check it connection to drone extablished
         * 
         * @return true 
         * @return false 
         */
        bool IsConnReqAckReceived() { return m_connection_information.IsConnected; };
        DataManager(std::shared_ptr<spdlog::logger> logger, spdlog::level::level_enum lvl = spdlog::level::info);
        ~DataManager();

    private:
        void notify_log_req_received(IObserver *observer);
        void notify_dji_log_version(IObserver *observer);

        /**
         * @brief Expects IPositionVelocityObserver.
         * This is an example of implementing an observer that is an extention of IObserver.
         * 
         * @throw Exception if Wrong observers attached.
         * @param[out] observer - an attched IPositionVelocityObserver.
         */
        void notify_position_velocity(IObserver *observer);
        void howManyObservers(const OBSERVERS observer_type);

        /**
         * @section Helper structs.
         * 
         */
        ConnectionInformation m_connection_information;

        LogHeaderInformation m_log_header_information;
        PoseVelData m_posVel;
        std::unordered_map<OBSERVERS, std::list<IObserver *>> m_attached_dict;
        std::shared_ptr<spdlog::logger> m_logger;
    };
} // namespace tello_protocol
