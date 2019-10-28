#pragma once
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "CDataType.h"

class SocketException
{
private:
	int ecode;
public:
	SocketException(int code) : ecode(code) {};
	int GetErrorCode() const { return ecode; }
};

class TcpCommunicator
{
private:
	SOCKET m_socket;
public:
	TcpCommunicator(SOCKET);
	~TcpCommunicator();

	Buffer<char>* Receive();
	int Send(Buffer<char>* buffer);
	void Shutdown(int how);
	void Close();
};

class TcpListener
{
private:
	SOCKET m_socket;
	sockaddr_in m_addr;
public:
	TcpListener(SOCKET);
	~TcpListener();
	void SetAddress(const ULONG& address, int port);
	void Listen(int backlog);
	void Bind();
	SOCKET Accept();
	void Shutdown(int how);
	void Close();
};