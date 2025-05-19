#include <FS.h>
#include <LittleFS.h>
#include <iostream>
#include <string>
#include <string_view>
#include "filesystem.hpp"
//IMPLEMENTATIONS ARE UNSAFE AND MAY CRASH SYSTEM IF FILES GET TOO BIG

namespace filesystem {
    void saveToFile(std::string filename, std::string contents) {
        LittleFS.begin();
        std::string path = "/" + filename;
        File file = LittleFS.open(path.c_str(), "w");
        if (!file) {
            LittleFS.end();
            return;
        }

        file.write(contents.c_str());
        LittleFS.end();
    }

    std::string getFileContents(std::string filename) {
        LittleFS.begin();
        std::string path = "/" + filename;
        File file = LittleFS.open(path.c_str(), "r");
        if (!file) {
            LittleFS.end();
            return "";
        }

        char buffer[1024];
        std::string_view view;
        while (file.available()) {
            int bytesRead = file.readBytes(buffer, 1024);
            view = std::string_view(buffer, bytesRead);
        }

        std::string contents{view};
        file.close();
        LittleFS.end();
        return contents;
    }

}