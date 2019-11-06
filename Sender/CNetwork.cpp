#include "CNetwork.h"

TcpCommunicator::TcpCommunicator(SOCKET socket) : m_socket(socket)
{
	if (socket == INVALID_SOCKET)
		throw SocketException(WSAGetLastError());
}

TcpCommunicator::~TcpCommunicator()
{
	Close();
}

Buffer<char>* TcpCommunicator::Receive()
{
	long len = 0, t = 0;
	do {
		int t_;
		t_ = recv(m_socket, ((char*)&len) + t, 4 - t, 0);
		if (t_ == -1)
		{
			throw SocketException(WSAGetLastError());
		}
		else
			t += t_;
	} while (t < 4);

	len = ntohl(len);
	Buffer<char>* buffer = new Buffer<char>(len);
	t = 0;
	do {
		int t_;
		t_ = recv(m_socket, ((char*)buffer->getBuffer()) + t, len - t, 0);
		if (t_ == -1)
		{
			delete buffer;
			throw SocketException(WSAGetLastError());
		}
		else
			t += t_;
	} while (t < len);

	return buffer;
}

int TcpCommunicator::Send(Buffer<char>* buffer)
{
	int sz = buffer->size();
	if (sz == 0)
	{
		throw SocketException(WSAGetLastError());
	}
	long _sz = htonl(sz);
	if (send(m_socket, (char*)&_sz, 4, 0) != 4)
		throw SocketException(WSAGetLastError());
	int r = 0;
	do
	{
		int r_ = send(m_socket, buffer->getBuffer() + r, sz - r, 0);
		if (r_ == -1)
			throw SocketException(WSAGetLastError());
		r += r_;
	} while (r < sz);

	return 0;
}

void TcpCommunicator::Shutdown(int how)
{
	shutdown(m_socket, how);
}

void TcpCommunicator::Close()
{
	if (m_socket == INVALID_SOCKET)
		return;
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
}

TcpListener::TcpListener(SOCKET socket) : m_socket(socket)
{
	ZeroMemory(&m_addr, sizeof(sockaddr_in));
	m_addr.sin_family = AF_INET;
}

TcpListener::~TcpListener()
{
	Close();
}

void TcpListener::SetAddress(const ULONG & address, int port)
{
	m_addr.sin_addr.s_addr = htonl(address);
	m_addr.sin_port = htons(port);
}

void TcpListener::Listen(int backlog)
{
	listen(m_socket, backlog);
}

void TcpListener::Bind()
{
	bind(m_socket, (sockaddr*)&m_addr, sizeof(sockaddr_in));
}

SOCKET TcpListener::Accept()
{
	sockaddr_in addr;
	int sz = sizeof(sockaddr_in);
	return accept(m_socket, (sockaddr*)&addr, &sz);
}

void TcpListener::Shutdown(int how)
{
	shutdown(m_socket, how);
	Close();
}

void TcpListener::Close()
{
	closesocket(m_socket);
}
