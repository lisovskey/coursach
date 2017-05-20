#pragma once

#include <cstddef>

// Отрисовка меню с номерами клавиш
void drawMenu(const std::size_t, ...);

// Отрисовка заголовков таблицы
void drawTitles(const std::size_t, ...);

// Отрисовка ожидания нажатия любой клавиши
void drawPressAnyKey();

// Отрисовка строки посередине
void drawCentered(const std::string, const std::size_t);

// Отрисовка строки до середины
void drawPreCentered(const std::string, const std::size_t);

// Отрисовка страницы помощи
void drawHelp(bool);
