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

    while (getline(ist, line)) {
        try {
            files.emplace_back(line);
        }
        catch (const std::exception& e) {
            cerr << "Ошибка при обработке строки: " << e.what() << endl;
        }
    }
    ist.close();

    // Сортировка по имени файла
    sort(files.begin(), files.end(), [](const FileInfo& a, const FileInfo& b) {
        return a.getName() < b.getName();
        });

    // Сортировка по КБ
    long long minKB, maxKB;
    cout << "Введите диапазон размера файла (в КБ): ";
    cin >> minKB >> maxKB;

    cout << "\nФайлы в диапазоне " << minKB << "–" << maxKB << " КБ:\n\n";
    for (size_t i = 0; i < files.size(); ++i)
    {
        const FileInfo& f = files[i];
        size_t sizeKB = f.getSizeKB();
        if (sizeKB >= minKB && sizeKB <= maxKB)
            cout << f;
    }

    return 0;
}

// Создать Git
// Мне: сделать меню с выбором
// Напарнику: сделать добавление строки

// Венгерская нотация
// Что-то про int32 вместо long long
// size_t вместо long long в FileInfo.h
// Добавить инфу про строку в консоли