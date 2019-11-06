#include "CActor.h"

#pragma region ActorBase
int CActor::lastId = 0;

CActor::CActor() : m_id(lastId++), m_keepRun(true)
{
}

CActor::~CActor()
{
}

void CActor::terminate()
{
	onTerminate();
	m_keepRun = false;
	m_cv.notify_one();
}
#pragma endregion
#pragma region CommandActor
CCommandProcessor::CCommandProcessor()
{
}

CCommandProcessor::~CCommandProcessor()
{
	while (!m_queue.empty())
	{
		delete m_queue.front();
		m_queue.pop();
	}
}

void CCommandProcessor::run()
{
	while (m_keepRun)
	{
		unique_lock<mutex> lk(m_lock);
		m_cv.wait(lk, [&] {return !m_queue.empty() || !m_keepRun; });
		
		while (m_keepRun)
		{
			Buffer<char>* b = m_queue.front();
			m_queue.pop();
			lk.unlock();

			system(b->getBuffer());
			delete b;

			lk.lock();
			if (m_queue.empty())
				break;
		}
		lk.unlock();
	}
}

void CCommandProcessor::push_message(void* msg)
{
	{
		lock_guard<mutex> lk(m_lock);
		m_queue.push((Buffer<char>*)msg);
	}
	m_cv.notify_one();
}
#pragma endregion
#pragma region ServerActor
void CServerActor::run()
{
	while (m_keepRun)
	{
		{
			lock_guard<mutex> lk(m_lock);
			while (!m_queue.empty())
			{
				if (m_list.size() >= 5)
					break;
				m_list.push_back(m_queue.front());
				m_queue.pop();
			}
		}

		for (list<TcpCommunicator*>::iterator it = m_list.begin(); it != m_list.end();)
		{
			try {
				Buffer<char>* bf = (*it)->Receive();
				next->push_message(bf);
			}
			catch (SocketException e)
			{
				printf("err: %d\n", e.GetErrorCode());
				if (e.GetErrorCode() != WSAETIMEDOUT)
				{
					printf("disconn: %d\n", e.GetErrorCode());
					delete *it;
					it = m_list.erase(it);
					continue;
				}
			}
			it++;
		}
	}
}

void CServerActor::push_message(void * msg)
{
	lock_guard<mutex> lk(m_lock);
	m_queue.push((TcpCommunicator*)msg);
}
CServerActor::CServerActor(CActor* n) : next(n)
{
}
CServerActor::~CServerActor()
{
	for (list<TcpCommunicator*>::iterator it = m_list.begin(); it != m_list.end();it++)
	{
		delete *it;
	}
	
	while (!m_queue.empty())
	{
		m_queue.front()->Close();
		m_queue.pop();
	}
}
#pragma endregion
#pragma region ServerListener
#define LOGIN_NONPASS 96
#define LOGIN_REQPASS 97
void CServerListener::run()
{
	while (m_keepRun)
	{
		SOCKET s = listener.Accept();
		if (s == INVALID_SOCKET)
			continue;
		TcpCommunicator* con = new TcpCommunicator(s);

		Buffer<char> bf;
		char name[20];
		gethostname(name, 20);
		int namelen = strlen(name)+1;
		bf.reserve(namelen+1);
		bf.getBuffer()[0] = LOGIN_REQPASS;
		memcpy(bf.getBuffer()+1, name, namelen);
		con->Send(&bf);
		bf.release();

		auto_ptr<Buffer<char>> pwbf(con->Receive());

		if (strcmp(m_password, pwbf->getBuffer()) != 0)
		{
			delete con;
			continue;
		}

		next->push_message(con);
	}
}

void CServerListener::push_message(void * msg)
{
}

void CServerListener::onTerminate()
{
	listener.Shutdown(SD_BOTH);
}

CServerListener::CServerListener(CActor* n, int port) : next(n), listener(TcpListener(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)))
{
	m_password = "HARICOM1";

	listener.SetAddress(INADDR_ANY, port);
	listener.Bind();
	listener.Listen(5);
}

CServerListener::~CServerListener()
{
	listener.Close();
}
#pragma endregion