#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "CNetwork.h"
#include "CDataType.h"
#include <stdio.h>
#include <iostream>

int main()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	char cmd[256] = "startmsg";
	SOCKET snd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(snd, (sockaddr*)&addr, sizeof(sockaddr_in));
	printf("connected!\n");
	TcpCommunicator so(snd);
	char hostname[20];
	Buffer<char>* tmp = so.Receive();
	memcpy(hostname, tmp->getBuffer(), tmp->size());
	delete tmp;
	while (1)
	{
		std::cout << hostname << "> ";
		std::cin >> cmd;
		int l = strlen(cmd) + 1;
		Buffer<char> bf(l);
		memcpy(bf.getBuffer(), cmd, l);
		so.Send(&bf);
		Sleep(1000);
	}
	closesocket(snd);
	WSACleanup();
}