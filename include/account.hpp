#pragma once

enum {
	ROLE_USER,
	ROLE_ADMIN
};

bool auth();
std::size_t createAccount();
std::size_t editAccount();
std::size_t viewAccounts();
void writeAccounts();
