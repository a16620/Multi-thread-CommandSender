#pragma once
#include "CActor.h"

class MainFrame
{
private:
	CActor* components[3];
	thread w[3];
public:
	void Run(int key);

	MainFrame();
	~MainFrame();
};

