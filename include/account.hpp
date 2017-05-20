#pragma once

#include <cstddef>

enum {
	ROLE_USER,
	ROLE_ADMIN
};

// Инициализация окна и авторизация или регистрация
bool auth();

// Создание аккаунта вручную
std::size_t createAccount();

// Изменение полей аккаунта
std::size_t editAccount();

// Отображение всех аккаунтов
std::size_t viewAccounts();

// Запись из вектора в файл
void writeAccounts();
