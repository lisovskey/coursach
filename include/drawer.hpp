#pragma once

#include <cstddef>

void drawMenu(const std::size_t, ...);
void drawTitles(const std::size_t, ...);
void drawPressAnyKey();
void drawCentered(const std::string, const std::size_t);
void drawPreCentered(const std::string, const std::size_t);
void drawHelp(bool);
