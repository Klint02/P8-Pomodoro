#include <iostream>
#include "Arduino.h"
#include <ESP8266WebServer.h>
#include <IPAddress.h>
#include <Wire.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include "src/filesystem/filesystem.hpp"
#include "src/RTCService/RTCService.hpp"
#include "src/LoggingService/LoggingService.hpp"
#include "src/NetworkService/NetworkService.hpp"



auto RTC_service = RTC::RTCService();
auto logging_service = logging::LoggingService(RTC_service);
auto central_logger = new logging::LoggingWrapper("Central Service", logging_service);
auto network_service = new network::networkService(logging_service, RTC_service);
void setup() {
    Serial.begin(115200);
    std::cout << std::endl;
    central_logger->log("PomoOS starting up", logging::levels::INFO);

    network_service->initWiFi();
    network_service->initRest();
    central_logger->log("PomoOS is now running", logging::levels::INFO);
}

void loop() {
    network_service->handleRestClient();
}