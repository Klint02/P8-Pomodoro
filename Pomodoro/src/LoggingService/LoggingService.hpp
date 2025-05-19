#ifndef LOGGING_SERVICE_HEADER
#define LOGGING_SERVICE_HEADER
namespace logging
{
    enum levels {
        ERROR = 1,
        DEBUG,
        INFO
    };

    class LoggingService
    {
    private:
        RTC::RTCService& RTC_service;
    public:
        LoggingService(RTC::RTCService& RTC_service);
        void log(const std::string service_name, const std::string message, const int level) const;
    };

    class LoggingWrapper
    {
    private:
        std::string service_name = "";
        const LoggingService& logging_service;
    public:
        LoggingWrapper(std::string service_name, const LoggingService& loggins_service);
        void log(std::string message, int level) const;

    };
    
}

#endif