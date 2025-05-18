#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <Arduino.h>
#include <IPAddress.h>
#include <algorithm>
#include <Arduino_JSON.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include "../TaskControlService/TaskControlService.hpp"
#include "../RTCService/RTCService.hpp"
#include "../LoggingService/LoggingService.hpp"
#include "NetworkService.hpp"

namespace network {
    std::string IPToString(IPAddress address) {
        std::ostringstream message_oss;

        message_oss << std::to_string(static_cast<u_int8_t>(address[0])) << "." << std::to_string(static_cast<u_int8_t>(address[1])) << "." << std::to_string(static_cast<u_int8_t>(address[2])) << "." << std::to_string(static_cast<u_int8_t>(address[3]));

        return message_oss.str();
    }

    networkService::networkService(const logging::LoggingService& logging_service, RTC::RTCService& RTC_service) : RTC_service(RTC_service) {
        this->server = new ESP8266WebServer(80);
        this->current_ip = new IPAddress(0,0,0,0);
        this->local_ip = new IPAddress(192,168,1,2);
        this->gateway = new IPAddress(192,168,1,1);
        this->subnet = new IPAddress(255,255,255,0);
        this->logger = new logging::LoggingWrapper("Network Service", logging_service);
        //FOR TESTING
    }

    void networkService::initWiFi() {
        if (wifi_network_status == networkStatuses::running) {
            logger->log("Attempted start of wifi, but it is already running", logging::levels::ERROR);
            return;
        }

        logger->log("Starting WiFi", logging::levels::INFO);
        //Determine what networks are present
        WiFi.mode(WIFI_STA);
        
        logger->log("Scanning for availible networks", logging::levels::INFO);
        auto wifi_scan_count = WiFi.scanNetworks();
        
        if (wifi_scan_count < 1 || wifi_networks.size() < 1) {
            logger->log("No known networks found. Starting softAP", logging::levels::ERROR);
            
            logger->log("Setting soft-AP configuration ... ", logging::levels::INFO);
            if (WiFi.softAP("Pomodoro Net") && WiFi.softAPConfig(*local_ip, *gateway, *subnet)) {
                logger->log("Ready", logging::levels::INFO);
            } else {
                logger->log("Failed, stopping WiFi radio", logging::levels::ERROR);
                stopWiFi();
                return;
            }

            logger->log("Soft-AP IP address = " + IPToString(*local_ip), logging::levels::INFO);
            wifi_network_status = networkStatuses::running;
        } else {
            //TODO (nkc): Wifi connection is currently not implemented
            logger->log("NOT IMPLEMENTED", logging::levels::ERROR);
            return;

            for (int i = 0; i < wifi_scan_count; i++) {

            }

            WiFi.begin("", "");
            Serial.println("");

            // Wait for connection
            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                Serial.print(".");
            }

            Serial.println("");
            Serial.print("Connected to ");
            Serial.println("");
            Serial.print("IP address: ");
            //this->current_ip* = WiFi.localIP();
            Serial.println();
        }
        


    }

    void networkService::stopWiFi() {
        logger->log("Stopping WiFi", logging::levels::INFO);
        WiFi.mode(WIFI_OFF);
        WiFi.forceSleepBegin();
        yield();
        wifi_network_status = networkStatuses::sleeping;
        logger->log("WiFi stopped", logging::levels::INFO);

    }

    void networkService::initRest() {
        if (wifi_network_status != networkStatuses::running || rest_server_status == networkStatuses::running) {
            logger->log("Attempted start of REST server, but server is already running or WiFi is not", logging::levels::ERROR);
            
            return;
        }
        logger->log("Starting local REST server ", logging::levels::INFO);
        // Set server routing
        restServerRouting();
        // Set not found response
        server->onNotFound([this]() {handleNotFound();});
        // Start server
        server->begin();
        logger->log("Local REST server started ", logging::levels::INFO);
        rest_server_status = networkStatuses::running;
    }

    void networkService::restServerRouting() {
        server->on("/", HTTP_GET, [this]() {
            server->send(200, F("text/html"),
                F("Welcome to the REST Web Server"));
        });

        server->on("/status", HTTP_GET, [this]() {
            server->send(200, F("text/html"), F("UP"));
        });

        server->on("/setclock", HTTP_GET, [this]() {
            if (server->hasArg("time") && server->hasArg("date")) {
                RTC_service.setClock(server->arg("date").c_str(), server->arg("time").c_str());
                std::string message = "Time is set to " + RTC_service.getTime();
                logger->log(message, logging::levels::INFO);
                server->send(200, F("text/html"), message.c_str());
            } else {
                server->send(400, F("text/html"), F("Expected query parameters \"date\" & \"time\""));
            }
        });

        server->on("/recieve", HTTP_GET, [this]() {
            logger->log("babababab", logging::levels::DEBUG);
            server->send(200, F("application/json"), F(""));
        });

        server->on("/send", HTTP_POST, [this]() {
            logger->log("Starting import of app tasks", logging::levels::INFO);
            JSONVar tasks = JSON.parse(server->arg("plain"));
            
            if (JSON.typeof(tasks) == "undefined") {
                logger->log("Parsing input failed!", logging::levels::ERROR);

                server->send(400, F("text/html"), F("Failed to parse JSON body"));
                return;
            }
            logger->log("Making tasks", logging::levels::INFO);
            
            std::vector<TSC::Task> task_vector;

            for (int i = 0; i < tasks.length(); i++) {
                task_vector.push_back(TSC::Task(tasks[i]));
            }

            for (auto& task : task_vector) {
                std::cout << task << std::endl;
            }

            server->send(200, F("text/html"), F("done"));
        });
    }

    void networkService::handleNotFound() {
        // Manage not found URL
        String message = "File Not Found\n\n";
        message += "URI: ";
        message += server->uri();
        message += "\nMethod: ";
        message += (server->method() == HTTP_GET) ? "GET" : "POST";
        message += "\nArguments: ";
        message += server->args();
        message += "\n";
        for (uint8_t i = 0; i < server->args(); i++) {
            message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
        }
        server->send(404, "text/plain", message);
    }

    void networkService::handleRestClient() {
        server->handleClient();
    }
     
    int networkService::addNetwork(wifiNetworkSettings setting) {
        
        //TODO (nkc): Should also save to a config file
        auto it = std::find(this->wifi_networks.begin(), this->wifi_networks.end(), setting);
        if (it == wifi_networks.end()) {
            this->wifi_networks.push_back(setting);
            return networkStatuses::success;
        } else {
            *it = setting;
            return networkStatuses::already_present;
        }
        return networkStatuses::error;
        
    }

    int networkService::removeNetwork(wifiNetworkSettings setting) {
        //TODO (nkc): Should also save to a config file
        auto it = std::find(this->wifi_networks.begin(), this->wifi_networks.end(), setting);
        if (it == wifi_networks.end()) {
            return networkStatuses::does_not_exist;
        } else {
            this->wifi_networks.erase(it);
            return networkStatuses::success;
        }
        return networkStatuses::error;
    }

}