#ifndef FILESYSTEM_HEADER
#define FILESYSTEM_HEADER

namespace filesystem {
    void saveToFile(std::string filename, std::string contents);

    std::string getFileContents(std::string filename);

}

#endif