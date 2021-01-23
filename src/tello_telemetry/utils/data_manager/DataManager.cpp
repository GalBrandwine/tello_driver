#include "DataManager.hpp"
namespace tello_protocol
{
    void DataManager::SetLogData(const std::shared_ptr<ILogDataGetter> log_data_processor)
    {
        /**
         * @todo Upon SetLogData, also Notify all attached to CONN_ACK_MSG.
         * Sometimes conenection to the drone might be already extablished to the drone wont sent CONN_ACK_MSG.
         * We need to trigger it: SetConnAcked();
         * 
         */

        // std::string msg(__PRETTY_FUNCTION__);
        // m_logger->info(msg + " Called");

        PoseVelData posVel;
        bool is_success = log_data_processor->GetLogMvo().GetPosVelIfUpdated(posVel);
        if (is_success)
        {
            m_logger->info("pose {} {} {}", posVel.pose.x, posVel.pose.y, posVel.pose.z);
        }
        else
        {
            // m_logger->info("Waiting for pos data.");
        }

        // Notify(ACK_LOG_HEADER);
    }

    void DataManager::SetLogID(const unsigned short id)
    {
        m_logger->info(__PRETTY_FUNCTION__);
        std::memcpy(&m_log_header_information.log_id, &id, sizeof(unsigned short));
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

    void DataManager::notify_log_req_received(IObserver *observer)
    {
        observer->Update(m_log_header_information.log_id);
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
            default:
                break;
            }
        }
    }

    void DataManager::Attach(const OBSERVERS observer_type, IObserver *observer)
    {
        m_attached_dict[observer_type].push_back(observer);
    }

    void DataManager::howManyObservers(const OBSERVERS observer_type)
    {
        m_logger->debug("There are " + std::to_string(m_attached_dict[observer_type].size()) + " observers attached to: " + observer_name(observer_type) + ".\n");
    }

    DataManager::DataManager(std::shared_ptr<spdlog::logger> logger, spdlog::level::level_enum lvl)
        : m_logger(logger)
    {
        m_log_header_information.log_id.reserve(sizeof(unsigned short));
    }

    DataManager::~DataManager()
    {
    }
} // namespace tello_protocol
