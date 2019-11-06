#include <stdio.h>
#include "MainFrame.h"
void Stealth();

int main()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
	Stealth();
	MainFrame main;
	main.Run(VK_ESCAPE);
	WSACleanup();
	return 0;
}

void Stealth()
{
	char title[516] = "";

	HWND hwnd;

	//hwnd = FindWindowA(NULL/*"ConsoleWindowClass"*/, title);
	//if (hwnd != NULL)
	//{
	//	return false;
	//}

	AllocConsole();
	//SetConsoleTitleA(title);
	GetConsoleTitleA(title, sizeof(title));
	hwnd = FindWindowA(NULL/*"ConsoleWindowClass"*/, title);
	ShowWindow(hwnd, 0);
}