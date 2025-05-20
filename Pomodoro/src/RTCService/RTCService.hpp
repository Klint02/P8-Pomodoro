#ifndef RTC_SERVICE
#define RTC_SERVICE
namespace RTC {

    enum Weekday
    {
        Monday = 0,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday,
    };
    
    class RTCService
    {
    private:
        RtcDateTime compiled_time;
        RtcDateTime actual_time;
        ThreeWire* myWire = nullptr; // IO, SCLK, CE
        RtcDS1302<ThreeWire>* Rtc = nullptr;

    public:
        RTCService(/* args */);
        std::string getTime();
        std::string getTime(RtcDateTime& actual_time);
        void setClock(std::string date, std::string time);
        //int getCurrentHour();
    };
    
}


#endif