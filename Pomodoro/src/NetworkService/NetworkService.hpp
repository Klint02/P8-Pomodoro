#ifndef NETWORK_SERVICES
#define NETWORK_SERVICES
namespace network {
    struct wifiNetworkSettings {
        std::string ssid;
        std::string password;

        bool operator==(const wifiNetworkSettings& other) const {
            //We only check for ssid as password may change for wifi networks
            return ssid == other.ssid;
        }
    };

    enum networkStatuses {
        error = -1,
        sleeping,
        running,
        success,
        already_present,
        does_not_exist
    };

    std::string IPToString(IPAddress address);

    class networkService {
        public:
            networkService(const logging::LoggingService& logging_service, RTC::RTCService& RTC_service, TCS::TaskControlService& TCS);            
            void handleRestClient();
            void initWiFi();
            void stopWiFi();
            void initRest();
            int addNetwork(wifiNetworkSettings setting);
            int removeNetwork(wifiNetworkSettings setting);
            std::string getSSID();
            std::string getIP();
            logging::LoggingWrapper* logger = nullptr;


        private:
            RTC::RTCService& RTC_service;
            TCS::TaskControlService& TCS;
            std::vector<wifiNetworkSettings> wifi_networks = {};
            int rest_server_status = networkStatuses::sleeping;
            int wifi_network_status = networkStatuses::sleeping;
            IPAddress* current_ip = nullptr; 
            IPAddress* local_ip = nullptr; 
            IPAddress* gateway = nullptr; 
            IPAddress* subnet = nullptr; 
            std::string SSID = "Pomodoro Net";
            ESP8266WebServer* server = nullptr;

            void restServerRouting();

            void handleNotFound();

            std::string searchWiFiSpace(std::vector<wifiNetworkSettings> wifinetworks);

    };
}


#endif