#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "FileInfo.h"
#include <stdint.h>
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

    switch (choice)
    {
    case 1:             // Добавление новой строки
        // Напарник добавит строку

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