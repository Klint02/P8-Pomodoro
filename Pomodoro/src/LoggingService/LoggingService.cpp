#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include "../RTCService/RTCService.hpp"
#include "LoggingService.hpp"

namespace logging {
    LoggingService::LoggingService(RTC::RTCService& RTC_service) : RTC_service(RTC_service) {
        std::cout << "LoggingService" << std::endl;
    }
    void LoggingService::log(const std::string service_name, const std::string message, const int level) const {
        std::string log_level = "";
        switch (level)
        {
        case levels::INFO:
            log_level = "INFO";
            break;

        case levels::DEBUG:
            log_level = "DEBUG";
            break;
        
        case levels::ERROR:
            log_level = "ERROR";
            break;

        default:
            log_level = "UNKNOWN";
            break;
        }
        std::cout << "[" << RTC_service.getTime() << "][" << log_level << "]["  << service_name << "]: " << message << std::endl;
    }


    LoggingWrapper::LoggingWrapper(std::string service_name, const LoggingService& logging_service) : service_name(service_name), logging_service(logging_service) {
        std::cout << "LoggerWrapper created for " << service_name << std::endl;
    }

    void LoggingWrapper::log(const std::string message, const int level) const {
        logging_service.log(service_name, message, level);
    }
}