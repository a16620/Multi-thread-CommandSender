#include "MainFrame.h"

void RunWorker(CActor* act)
{
	act->run();
}

void MainFrame::Run(int key)
{
	for (int i = 0; i < 3; i++)
		w[i] = thread(RunWorker, components[i]);
	
	while (!GetAsyncKeyState(key));
}

MainFrame::MainFrame()
{
	components[0] = new CCommandProcessor();
	components[1] = new CServerActor(components[0]);
	components[2] = new CServerListener(components[1], 6000);
}


MainFrame::~MainFrame()
{
	for (int i = 0; i < 3; i++)
	{
		components[i]->terminate();
		w[i].join();
		delete components[i];
	}
}
