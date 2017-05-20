#pragma once

#include <cstddef>
#include <string>

// ������ �������������� �����
std::size_t getPositiveNumber();

// ������ ��������� ��������
double getMoney();

// ������ ������� �� 10
std::size_t getMark();

// ������ �������� ��������
bool getBoolean();

// ������ ������ � ������� ������������ ��������
std::string getPass(const std::size_t);
