﻿#define _SILENCE_ALL_CXX23_DEPRECATION_ARNINGS 
#define _CRT_SECURE_NO_WARNINGS

#include <locale.h>

#include "header/Server.h"

int main() {
	setlocale(LC_ALL, "en_US.UTF-8");

	Server server(9000);

	server.run();

	return 0;
}
