// HelloWorldLoop.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <windows.h>
#include <iostream>



int main()
{
	while (1) {
		std::cout << "Hello world!" << std::endl;
		Sleep(5000);
	}
    return 0;
}

