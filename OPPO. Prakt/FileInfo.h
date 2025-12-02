#pragma once
#include <string>
#include <regex>
#include <iostream>
#include <ctime>

class FileInfo {
public:
    FileInfo(const std::string& line);

    std::string getName() const;
    size_t getSizeKB() const;

    friend std::ostream& operator<<(std::ostream& stream, const FileInfo& c);

private:
    void StringToObjs(const std::string& line);
    bool isValidDate(int year, int month, int day) const;

    std::string sName;
    tm date{};
    int64_t iSize = 0;
};
