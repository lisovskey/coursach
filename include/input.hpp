#pragma once

#include <cstddef>
#include <string>

// Запрос положительного числа
std::size_t getPositiveNumber();

// Запрос денежного значения
double getMoney();

// Запрос отметки до 10
std::size_t getMark();

// Запрос булевого значения
bool getBoolean();

// Запрос пароля с заменой отображаемых символов
std::string getPass(const std::size_t);
