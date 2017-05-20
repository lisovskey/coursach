#pragma once

#include <cstddef>

// Считывание студентов и настроек с дефолтными значениями
void readData();

// Добавление нового студента
std::size_t addStudent();

// Поиск студента по определенному полю
std::size_t findStudent();

// Изменение полей студента
void editStudent();

// Отображение всех студентов
void viewStudents();

// Отображение всех студентов в определенном порядке
void sortStudents();

// Настройки значений
void settings();

// Запись из вектора в файл
void writeStudents();

// Запись настроек в файл
void writeConfig();
