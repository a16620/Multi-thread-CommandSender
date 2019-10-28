#include <stdio.h>
#include "MainFrame.h"

int main()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
	MainFrame main;
	main.Run(VK_ESCAPE);
	WSACleanup();
	return 0;
}