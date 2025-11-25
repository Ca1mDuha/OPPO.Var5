#include "FileInfo.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>

FileInfo::FileInfo(const std::string& line) {
    StringToObjs(line);
}

void FileInfo::StringToObjs(const std::string& line) {
    std::regex name_reg("\"([^\"]+)\"");
    std::regex date_reg("(\\d{4})\\.(\\d{2})\\.(\\d{2})");
    std::regex size_reg("(-?\\d+)");
    std::smatch match;

    // Поиск имени файла
    if (std::regex_search(line, match, name_reg)) {
        name = match[1];
    }
    else {
        throw std::runtime_error("имя файла не найдено");
    }

    // Поиск даты
    int year = 0, month = 0, day = 0;
    if (std::regex_search(line, match, date_reg)) {
        year = stoi(match[1]);
        month = stoi(match[2]);
        day = stoi(match[3]);
        if (!isValidDate(year, month, day))
            throw std::runtime_error("некорректная дата");
        date.tm_year = year - 1900;
        date.tm_mon = month - 1;
        date.tm_mday = day;
    }
    else {
        throw std::runtime_error("дата не найдена");
    }

    // Поиск размера
    size = 0;
    bool found = false;
    auto begin = std::sregex_iterator(line.begin(), line.end(), size_reg);
    auto end = std::sregex_iterator();
    for (auto i = begin; i != end; ++i) {
        int n = stoi((*i)[1]);
        if (n != year && n != month && n != day) {
            size = n;
            found = true;
        }
    }
    if (!found) 
        throw std::runtime_error("размер не найден");
    if (size < 0) 
        throw std::runtime_error("отрицательный размер файла");
}

// Проверка корректности даты
bool FileInfo::isValidDate(int year, int month, int day) const {
    if (month < 1 || month > 12 || day < 1) return false;

    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (leap) 
        daysInMonth[1] = 29;

    return day <= daysInMonth[month - 1];
}

std::string FileInfo::getName() const {
    return name;
}

size_t FileInfo::getSizeKB() const {
    return size / 1024;
}

std::ostream& operator<<(std::ostream& stream, const FileInfo& c) {
    stream << "Файл: \"" << c.name << "\"\n";
    stream << "Дата создания: " << (c.date.tm_year + 1900) << "."
        << (c.date.tm_mon + 1) << "." << c.date.tm_mday << "\n";
    stream << "Размер: " << c.size << " байт (" << c.size / 1024 << " КБ)\n";
    stream << "--------------------------------------\n";
    return stream;
}
