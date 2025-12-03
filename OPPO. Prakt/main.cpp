#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "FileInfo.h"
#include <stdint.h>
#include <string>
#include <regex>
#include <sstream>
using namespace std;

int main() {
    setlocale(LC_ALL, "Rus");
    ifstream ist("File.txt");
    if (!ist.is_open()) {
        cerr << "Не удалось открыть файл File.txt\n";
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

    int choice;

    cout << "1 — Добавить строку в файл\n";
    cout << "2 — Показать все данные\n";
    cout << "3 — Показать данные, отсортированные по алфавиту\n";
    cout << "4 — Показать файлы в диапазоне КБ\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice)
    {
    case 1: {
        regex string_pattern(R"(^[\w\s]+\.[a-zA-Z0-9]+$)");
        string userFile;

        while (true) {
            cout << "Введите название файла (название + формат): ";
            getline(cin, userFile);

            if (regex_match(userFile, string_pattern)) {
                break;
            }
            else
                cout << "Ошибка! Неверный формат названия файла!" << endl;
        }

        regex date_regex(R"(\b\d+\.\d+\.\d+\b)");
        string userDate;

        while (true) {
            cout << "Введите дату в формате ГГГГ.ММ.ДД: ";
            cin >> userDate;

            vector<string> parts;
            stringstream ss(userDate);
            string part;

            while (getline(ss, part, '.'))
                parts.push_back(part);

            if (stoi(parts[1]) > 12 || stoi(parts[2]) > 31)
                cout << "Ошибка: неверный формат даты! Попробуйте снова." << endl;

            else if (regex_match(userDate, date_regex)) {
                break;
            }
            else {
                cout << "Ошибка: неверный формат даты! Попробуйте снова." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        regex size_regex(R"(\b\d+\b)");
        string fileSize;
        int Razmer;

        while (true) {
            cout << "Введите размер файла (целое число байт): ";
            cin >> fileSize;

            if (regex_match(fileSize, size_regex)) {
                try {
                    Razmer = stoi(fileSize);
                    break;
                }
                catch (const exception& e) {
                    cout << "Ошибка: неверный формат числа! Попробуйте снова" << endl;
                }
            }
            else {
                cout << "Ошибка! Размер должен быть целым и неотрицательным!" << endl;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        ofstream outfile("File.txt", ios::app);
        if (!outfile) {
            cerr << "Не удалось открыть файл для записи!" << endl;
            return 1;
        }

        outfile << endl;
        outfile << "\"" << userFile << "\"" " " << userDate << " " << Razmer;
        outfile.close();

        cout << "Данные успешно добавлены в файл!" << endl;
    }
          break;

    case 2:             // Вывод всех данных в консоль
        cout << "\nВсе данные:\n\n";
        for (const FileInfo& f : files)
            cout << f;
        break;

    case 3:             // Сортировка по имени файла
        cout << "\nСортировка по алфавиту:\n\n";
        {
            vector<FileInfo> sorted = files;
            sort(sorted.begin(), sorted.end(),
                [](const FileInfo& a, const FileInfo& b) {
                    return a.getName() < b.getName();
                });
            for (const FileInfo& f : sorted)
                cout << f;
        }
        break;

    case 4:             // Сортировка по КБ
        size_t minKB, maxKB;
        cout << "Введите диапазон размера файла (в КБ): ";
        cin >> minKB >> maxKB;

        cout << "\nФайлы в диапазоне " << minKB << "–" << maxKB << " КБ:\n\n";
        for (const FileInfo& f : files)
        {
            size_t sizeKB = f.getSizeKB();
            if (sizeKB >= minKB && sizeKB <= maxKB)
                cout << f;
        }
        break;

    default:
        cout << "Неизвестная команда.\n";
        break;
    }

    return 0;
}