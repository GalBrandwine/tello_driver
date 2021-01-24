#pragma once

#include <memory>
#include <chrono>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "utils/TelloSocket.hpp"
#include "TelloTelemetry.hpp"
#include "TelloCommander.hpp"
#include "utils/data_manager/DataManager.hpp"

#include "TelloConnAckMsgObserver.hpp"
#include "TelloLogDataMsgObserver.hpp"
#include "TelloLogHeaderMsgObserver.hpp"
#include "TelloStickCommandsObserver.hpp"
#include "TelloAckLogHeaderIdSenderObserver.hpp"
#include "TelloDriverConfig.hpp"

#include <cstdlib>
#include <iostream>
#include "asio.hpp"
#include <thread>

/**
 * @file TelloDriver.hpp
 * @author your name (gal080592@gmail.com)
 * @brief The unofficial SDK Tello driver.
 * @date 2021-01-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

using namespace std::chrono_literals;
using asio::ip::udp;

#define CONNECTION_TIMEOUT 50
class TelloDriver
{
public:
    /**
    * @brief Movements section
    **/
    void Backward(int amount);
    void Forward(int amount);
    void Takeoff();
    void Land();
    const float GetAttLimit();
    const short GetAltLimit();
    const short GetWifiStrength();
    const tello_protocol::Vec3 GetPos();
    std::shared_ptr<spdlog::logger> GetLogger() { return m_BaseLogger; };
    TelloDriver(spdlog::level::level_enum lvl = spdlog::level::info);
    ~TelloDriver();
    tello_protocol::TelloCommander &GetTelloCommander();
    tello_protocol::TelloTelemetry &GetTelloTelemetry();

    void Attach(OBSERVERS oberver_type, IObserver *observer);
    void Connect();
    bool WaitForConnection(int);

private:
    tello_protocol::DataManager m_DataManager;

    /*************************************
     * 
     * @section Incomming data observers
     * 
    *************************************/

    /**
     * @brief TelloLogHeaderMsgObserver attached to telloTelemetry via ISubject interface.
     * It being callback upon new data arival to TelloTelemtry. 
     * Then parse this new data, and insert it to TelloDataManager through ILogHeaderMsgDataManager interface.
     */
    std::shared_ptr<IObserver> m_TelloLogHeaderMsgObserver;

    /**
     * @brief TelloLogDataMsgObserver attached to TelloTelemetry via ISubject interface.
     * It being callback upon new data arival to TelloTelemtry. 
     * 
     * 1. Upon callback, it creates LogData opbject, then pass raw_data into this LogData.
     * 2. Pass this LogData (filled with processed log data E.G position, velocity etc) into TelloDataManager:
     * via SetLogData() that is available due to ILogDataMsgDataManager interface.
     * 
     */
    std::shared_ptr<IObserver> m_TelloLogDataMsgObserver;

    /**
     * @brief TelloConnAckMsgObserver attached to TelloTelemetry via ISubject interface.
     * Upon callback, this obeserver will check if raw_data contain \'conn_ack\'. 
     * If so this observer will call DataManager::SetConnReqAck()
     * 
     */
    std::shared_ptr<IObserver> m_TelloConnAckMsgObserver;

    /*************************************
     * 
     * @section Outgoing commands observers
     * 
    *************************************/

    /**
     * @brief TelloAckLogHeaderIdSenderObserver attached to TelloDataManager via IDataMgrSubject interface.
     * TelloDataManager trigger a callback upon receiving new AckLogHeaderMsg, send ack commands according to attached OBSERVER::ACK_MSG.
     */
    std::shared_ptr<IObserver> m_TelloAckLogHeaderIdSenderObserver;

    /**
     * @brief TelloStickCommandsObserver attached to telloTelemetry via ISubject interface.
     * It being callback upon new data arival to TelloTelemtry. And send stick commands
     * 
     * @note this callback acts as a heartbeat back to the drone, if not sent the drone will stand still and blink on ORANGE.
     */
    std::shared_ptr<IObserver> m_TelloStickCommandsObserver;

    tello_protocol::TelloCommander m_TelloCommander;
    tello_protocol::TelloTelemetry m_TelloTelemetry;
    std::shared_ptr<spdlog::logger> m_BaseLogger;
    bool m_KeepRunning = true;
    std::thread m_StickCommandsSendingThread;
};
