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

namespace tello_protocol
{
    /**
     * @brief Store LogHeaderMsg information
     * 
     */
    struct LogHeaderInformation
    {
        std::vector<unsigned char> BuildDate, DJILogVersion,log_id;
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
          public ILogDataMsgDataManager
    {
    public:
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

        DataManager(std::shared_ptr<spdlog::logger> logger, spdlog::level::level_enum lvl = spdlog::level::info);
        ~DataManager();

    private:
        void notify_log_req_received(IObserver *observer);
        void notify_dji_log_version(IObserver *observer);
        void howManyObservers(const OBSERVERS observer_type);

        LogHeaderInformation m_log_header_information;
        std::unordered_map<OBSERVERS, std::list<IObserver *>> m_attached_dict;
        std::shared_ptr<spdlog::logger> m_logger;
    };
} // namespace tello_protocol
