#pragma once
#include<thread>
#include<mutex>
#include<queue>
#include<atomic>
#include<list>
#include "CNetwork.h"
#include"CDataType.h"
using namespace std;
class CActor
{
private:
	static int lastId;
protected:
	const int m_id;
	atomic<bool> m_keepRun;
	mutex m_lock;
	condition_variable m_cv;

public:
	virtual void push_message(void* msg) = 0;
	virtual void run() = 0;
	void terminate();
	CActor();
	virtual ~CActor();
private:
	virtual void onTerminate() {};
};

class CCommandProcessor : public CActor
{
private:
	queue<Buffer<char>*> m_queue;

public:
	CCommandProcessor();
	~CCommandProcessor();
	void run();
	void push_message(void* msg);
};

class CServerActor : public CActor
{
private:
	queue<TcpCommunicator*> m_queue;
	list<TcpCommunicator*> m_list;

	CActor* next;
public:
	void run();
	void push_message(void* msg);
	CServerActor(CActor* n);
	~CServerActor();
};

class CServerListener : public CActor
{
private:
	CActor* next;
	TcpListener listener;

public:
	void run();
	void push_message(void* msg);
	CServerListener(CActor* n, int port);
	~CServerListener();
private:
	virtual void onTerminate();
};