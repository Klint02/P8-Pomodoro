#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "RTCService.hpp"

// CONNECTIONS:
// DS1302 CLK/SCLK --> 12
// DS1302 DAT/IO --> 13
// DS1302 RST/CE --> 15
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

namespace RTC
{
    RTCService::RTCService(/* args */)
    {
        myWire = new ThreeWire(13, 12, 15); // IO, SCLK, CE
        Rtc = new RtcDS1302<ThreeWire>(*myWire);

        Rtc->Begin();

        compiled_time = RtcDateTime(__DATE__, __TIME__);
        std::cout << getTime(compiled_time) << std::endl;
        Serial.println();

        if (!Rtc->IsDateTimeValid())
        {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            Serial.println("RTC lost confidence in the DateTime!");
            Rtc->SetDateTime(compiled_time);
        }

        if (Rtc->GetIsWriteProtected())
        {
            Serial.println("RTC was write protected, enabling writing now");
            Rtc->SetIsWriteProtected(false);
        }

        if (!Rtc->GetIsRunning())
        {
            Serial.println("RTC was not actively running, starting now");
            Rtc->SetIsRunning(true);
        }

        actual_time = Rtc->GetDateTime();
        if (actual_time < compiled_time)
        {
            Serial.println("RTC is older than compile time!  (Updating DateTime)");
            Rtc->SetDateTime(compiled_time);
        }
        else if (actual_time > compiled_time)
        {
            Serial.println("RTC is newer than compile time. (this is expected)");
        }
        else if (actual_time == compiled_time)
        {
            Serial.println("RTC is the same as compile time! (not expected but all is fine)");
        }
    }

    std::string RTCService::getTime(RtcDateTime &actual_time)
    {
        // THIS IS ONLY FOR CONVERTING TIME TO A STRING
        // TO GET AN UPDATED TIME USE getTime();
        std::ostringstream time_oss;
        time_oss << std::to_string(static_cast<u_int16_t>(actual_time.Year()))
                 << "/" << std::setw(2) << std::setfill('0') << std::to_string(static_cast<u_int8_t>(actual_time.Month()))
                 << "/" << std::setw(2) << std::setfill('0') << std::to_string(static_cast<u_int8_t>(actual_time.Day()))
                 << " " << std::setw(2) << std::setfill('0') << std::to_string(static_cast<u_int8_t>(actual_time.Hour()))
                 << ":" << std::setw(2) << std::setfill('0') << std::to_string(static_cast<u_int8_t>(actual_time.Minute()))
                 << ":" << std::setw(2) << std::setfill('0') << std::to_string(static_cast<u_int8_t>(actual_time.Second()));
        return time_oss.str();
    }

    std::string RTCService::getTime()
    {
        actual_time = Rtc->GetDateTime();
        return getTime(actual_time);
    }

    // int RTCService::getCurrentHour()
    // {
    //     actual_time = Rtc->GetDateTime();

    //     return actual_time.hour;
    // }

    void RTCService::setClock(std::string date, std::string time)
    {
        RtcDateTime new_date_time = RtcDateTime(date.c_str(), time.c_str());
        std::cout << "setting time to " << date << time << std::endl;
        Rtc->SetDateTime(new_date_time);
    }

}