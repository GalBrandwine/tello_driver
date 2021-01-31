#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include "utils/tello_observer/IDataMgrSubject.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <sstream>
#include <type_traits>

#include "ILogHeaderMsgDataManager.hpp"
#include "ILogDataMsgDataManager.hpp"
#include "IConnAckMsgDataManager.hpp"
#include "IWifiMsgDataManager.hpp"
#include "IAltLimitMsgDataManager.hpp"
#include "ILogDataConnectionInformationSupply.hpp"
#include "IFlightDataMsgDataManager.hpp"
namespace tello_protocol
{

    /**
     * @brief Store and maintain data from Observers.
     * Apon each new data from an observer do:
     * * Update these new data (if new)
     * * Notify all attached subscribers to this new data.
     * 
     */
    class DataManager
        : public IDataMgrSubject,
          public ILogHeaderMsgDataManager,
          public ILogDataMsgDataManager,
          public IConnAckMsgDataManager,
          public IWifiMsgDataManager,
          public IAltLimitMsgDataManager,
          public ILogDataConnectionInformationSupply,
          public IFlightDataMsgDataManager
    {
    public:
        /**
         * @brief Set the Alt Limit Received as a response to the request tello_protocol::GET_ALT_LIMIT_CMD
         * 
         * @param alt_limit - requested limit, sent back from drone as an approval.
         */
        void SetAltLimit(unsigned char alt_limit) override;

        /**
         * @brief Set the Flight Data object
         * 
         * @param flight_data_processor 
         */
        void SetFlightData(const std::shared_ptr<IFlightDataGetter> flight_data_processor) override;

        /**
         * @brief Set the WifiMsg data.
         * Overriding IWifiMsgDataManager, so that TelloWifiMsgObserver (which attached to TelloTellemetry) could supply new WifiMsgs.
         * 
         * @param wifi_strength
         */
        void SetWifiMsg(const unsigned char &wifi_strength) override;

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
        void Attach(const OBSERVERS observer_type, IFlightDataObserver *observer) override;
        void Attach(const OBSERVERS observer_type, IImuAttitudeObserver *observer) override;

        // void Attach(const OBSERVERS observer_type, IConnectionEstablishedObserver *observer) override;

        /**
         * @brief Check it connection to drone extablished
         * 
         * @return true 
         * @return false 
         */
        bool IsConnReqAckReceived() { return m_connection_information.IsConnected; };

        /**
         * @brief Get the Connection Information object
         * 
         * @return const tello_protocol::ConnectionInformation& 
         */
        ConnectionInformation &GetConnectionInformation() override;

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

        /**
         * @brief Expects observer to has interface of IFlightDataObserver
         * 
         * @throw Exception if Wrong observers attached.
         * @param[out] observer - an attched IFlightDataObserver.
         */
        void notify_flight_data_received(IObserver *observer);

        /**
         * @brief Expects observer to has interface of IFlightDataObserver
         * 
         * @param[out] observer - an attched IFlightDataObserver.
         */
        void notify_imu_attitude_received(IObserver *observer);
        void howManyObservers(const OBSERVERS observer_type);

        /**
         * @section Helper structs.
         * 
         */
        ConnectionInformation m_connection_information;

        LogHeaderInformation m_log_header_information;
        ImuAttitudeData m_imuAtti;
        PoseVelData m_posVel;
        FlightDataStruct m_flightData;
        std::unordered_map<OBSERVERS, std::list<IObserver *>> m_attached_dict;
        std::shared_ptr<spdlog::logger> m_logger;
    };
} // namespace tello_protocol
