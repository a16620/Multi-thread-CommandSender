#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "CNetwork.h"
#include "CDataType.h"
#include <stdio.h>
#include <iostream>
#include <memory>
#define LOGIN_NONPASS 96
#define LOGIN_REQPASS 97
int main()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
	char targetip[32];
	std::cout << "target ip: ";
	std::cin >> targetip;
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	addr.sin_addr.s_addr = inet_addr(targetip);
	char cmd[256] = "startmsg";
	SOCKET snd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(snd, (sockaddr*)&addr, sizeof(sockaddr_in));
	std::cout << "connected:";
	TcpCommunicator so(snd);
	char hostname[32];
	try {
		Buffer<char>* tmp = so.Receive();
		char mode = tmp->getBuffer()[0];
		memcpy(hostname, tmp->getBuffer() + 1, strlen(tmp->getBuffer() + 1) + 1);
		std::cout << hostname << '\n';
		delete tmp;
		if (mode == LOGIN_REQPASS)
		{
			char pw[32];
			std::cout << "password: ";
			std::cin >> pw;
			int l = strlen(pw);
			Buffer<char> bf(l+1);
			memcpy(bf.getBuffer(), pw, l+1);
			so.Send(&bf);
		}
		
	}
	catch (SocketException e)
	{
		std::cerr << "Socket Exception Occur";
		so.Close();
		WSACleanup();
		return -1;
	}
	
	while (1)
	{
		try {
			std::cout << hostname << "> ";
			std::cin >> cmd;
			int l = strlen(cmd) + 1;
			Buffer<char> bf(l);
			memcpy(bf.getBuffer(), cmd, l);
			so.Send(&bf);
			Sleep(1000);
		}
		catch (...)
		{
			std::cerr << "Socket Exception Occur";
			break;
		}
	}
	so.Close();
	WSACleanup();
}