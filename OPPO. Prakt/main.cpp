﻿#include "FileInfo.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <string>
#include <regex>
#include <sstream>
#include <functional>
#include <map>
using namespace std;

struct MenuItem {
    std::string discr;
    std::function<void(vector<FileInfo>&)> func;
};

// Добавление новой строки
void addToFile(vector<FileInfo>&) {
    regex namePattern(R"(^[\w\s]+\.[a-zA-Z0-9]+$)");
    string userFile;

    while (true) {
        cout << "Введите название файла (название + формат): ";
        getline(cin, userFile);

        if (regex_match(userFile, namePattern))
            break;

        cout << "Ошибка! Неверный формат названия файла!" << endl;
    }

    regex datePattern(R"(\b\d{4}\.\d{2}\.\d{2}\b)");
    string userDate;

    while (true) {
        cout << "Введите дату в формате ГГГГ.ММ.ДД: ";
        getline(cin, userDate);

        vector<string> parts;
        stringstream ss(userDate);
        string part;

        while (getline(ss, part, '.'))
            parts.push_back(part);

        if (parts.size() == 3 && stoi(parts[1]) <= 12 && stoi(parts[2]) <= 31
            && regex_match(userDate, datePattern)) {
            break;
        }

        cout << "Ошибка: неверный формат даты! Попробуйте снова." << endl;
    }

    regex sizePattern(R"(^\d+$)");
    string fileSize;
    int Size_Bytes;

    while (true) {
        cout << "Введите размер файла (в байтах): ";
        getline(cin, fileSize);

        if (regex_match(fileSize, sizePattern)) {
            Size_Bytes = stoi(fileSize);
            break;
        }

        cout << "Ошибка: размер должен быть положительным целым числом!" << endl;
    }

    ofstream out("File.txt", ios::app);
    if (!out) {
        cerr << "Не удалось открыть файл для записи." << endl;
        return;
    }

    out << endl << "\"" << userFile << "\" " << userDate << " " << Size_Bytes;
    out.close();

    cout << "Данные успешно добавлены." << endl;
}


// Вывод всех данных
void showAll(vector<FileInfo>& files) {
    cout << "\nВсе данные:" << endl;
    for (const FileInfo& f : files)
        cout << f;
}


// Сортировка по имени файла
void showSorted(vector<FileInfo>& files) {
    vector<FileInfo> sorted = files;

    sort(sorted.begin(), sorted.end(),
        [](const FileInfo& a, const FileInfo& b) {
            return a.getName() < b.getName();
        });

    cout << "\nДанные, отсортированные по алфавиту:" << endl;

    for (const FileInfo& f : sorted)
        cout << f;
}


// Сортировка по КБ
void showBySizeRange(vector<FileInfo>& files) {
    size_t minKB, maxKB;
    cout << "Введите диапазон (в КБ): ";
    cin >> minKB >> maxKB;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nФайлы в диапазоне " << minKB << "–" << maxKB << " КБ:" << endl;

    for (const FileInfo& f : files) {
        size_t sizeKB = f.getSizeKB();
        if (sizeKB >= minKB && sizeKB <= maxKB)
            cout << f;
    }
}

int main() {
    setlocale(LC_ALL, "Rus");
    ifstream ist("File.txt");
    if (!ist.is_open()) {
        cerr << "Не удалось открыть файл File.txt" << endl;
        return 1;
    }

    vector<FileInfo> files;
    string line;
    int lineNumber = 0;

    while (getline(ist, line)) {
        lineNumber++;
        try {
            files.emplace_back(line);
        }
        catch (const std::exception& e) {
            cerr << "Ошибка при обработке строки #"
                << lineNumber << ": " << e.what() << endl;
        }
    }
    ist.close();

    map<int, MenuItem> menuMap = {
        {1, {"Добавить строку в файл", addToFile}},
        {2, {"Показать все данные", showAll}},
        {3, {"Показать данные, отсортированные по алфавиту", showSorted}},
        {4, {"Показать файлы в диапазоне КБ", showBySizeRange}}
    };

    for (auto& [id, item] : menuMap)
        cout << id << " — " << item.discr << endl;

    cout << "Ваш выбор: ";
    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    try {
        menuMap.at(choice).func(files);
    }
    catch (...) {
        cout << "Неизвестная команда." << endl;
    }

    return 0;
}