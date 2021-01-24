#include "DataManager.hpp"
namespace tello_protocol
{
    void DataManager::SetConnReqAck()
    {
        m_connection_information.IsConnected = true;
    }

    void DataManager::SetLogData(const std::shared_ptr<ILogDataGetter> log_data_processor)
    {
        /**
         * @brief Upon SetLogData, also Notify all attached to CONN_ACK_MSG.
         * Sometimes conenection to the drone might be already extablished to the drone wont sent CONN_ACK_MSG.
         * We need to trigger it: SetConnAcked();
         * 
         */
        if (!m_connection_information.IsConnected)
        {
            SetConnReqAck();
        }


        if (log_data_processor->GetLogMvo().GetPosVelIfUpdated(m_posVel))
        {
            m_logger->info("pose {} {} {}", m_posVel.pose.x, m_posVel.pose.y, m_posVel.pose.z);
            Notify(POSITION_VELOCITY_LOG);
        }
    }

    void DataManager::SetLogID(const unsigned short id)
    {
        m_logger->info(__PRETTY_FUNCTION__);
        std::memcpy(&m_log_header_information.LogId, &id, sizeof(unsigned short));
        m_logger->info("LogId: {};", id);
        Notify(ACK_LOG_HEADER);
    }

    void DataManager::SetBuildDate(const std::vector<unsigned char> &build_date)
    {
        m_log_header_information.BuildDate = build_date;

        std::stringstream ss;
        for (auto &character : m_log_header_information.BuildDate)
        {
            ss << character;
        }
        m_logger->info("BUILD date: {};", ss.str());
    }

    void DataManager::SetDJILogVersion(const std::vector<unsigned char> &log_version)
    {
        m_log_header_information.DJILogVersion = log_version;
        std::stringstream ss;
        for (auto &character : m_log_header_information.DJILogVersion)
        {
            ss << character;
        }
        m_logger->info("DJI LOG VERSION: {};", ss.str());
        Notify(DJI_LOG_VERSION);
    }

    void DataManager::notify_position_velocity(IObserver *observer)
    {
        try
        {
            dynamic_cast<IPositionVelocityObserver *>(observer)->Update(m_posVel);
        }
        catch (const std::exception &e)
        {
            m_logger->critical("Cought dynamic_cast exception: {}", e.what());
        }
    }

    void DataManager::notify_log_req_received(IObserver *observer)
    {
        observer->Update(m_log_header_information.LogId);
    }

    void DataManager::notify_dji_log_version(IObserver *observer)
    {
        std::vector<unsigned char> data(m_log_header_information.DJILogVersion);
        data.insert(data.end(), m_log_header_information.BuildDate.begin(), m_log_header_information.BuildDate.end());
        observer->Update(data);
    }

    void DataManager::Notify(const OBSERVERS observer_type)
    {
        howManyObservers(observer_type);

        for (auto observer : m_attached_dict[observer_type])
        {
            switch (observer_type)
            {
            case OBSERVERS::ACK_LOG_HEADER:
                notify_log_req_received(observer);
                break;
            case OBSERVERS::DJI_LOG_VERSION:
                notify_dji_log_version(observer);
                break;
            case OBSERVERS::POSITION_VELOCITY_LOG:
                notify_position_velocity(observer);
                break;
            default:
                break;
            }
        }
    }

    void DataManager::Attach(const OBSERVERS observer_type, IObserver *observer)
    {
        m_attached_dict[observer_type].push_back(observer);
    }
    void DataManager::Attach(const OBSERVERS observer_type, IPositionVelocityObserver *observer)
    {
        m_attached_dict[observer_type].push_back(observer);
    }
    // void DataManager::Attach(const OBSERVERS observer_type, IConnectionEstablishedObserver *observer)
    // {
    //     m_attached_dict[observer_type].push_back(observer);
    // }
    void DataManager::howManyObservers(const OBSERVERS observer_type)
    {
        m_logger->debug("There are " + std::to_string(m_attached_dict[observer_type].size()) + " observers attached to: " + observer_name(observer_type) + ".\n");
    }

    DataManager::DataManager(std::shared_ptr<spdlog::logger> logger, spdlog::level::level_enum lvl)
        : m_logger(logger)
    {
        m_log_header_information.LogId.reserve(sizeof(unsigned short));
    }

    DataManager::~DataManager()
    {
    }
} // namespace tello_protocol
