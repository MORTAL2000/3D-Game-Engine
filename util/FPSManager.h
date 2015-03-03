#ifndef FPS_MANAGER_H
#define FPS_MANAGER_H

#include "../core/Commons.h"

class FPSManager
{
public:
	static FPSManager& getInstance();

	void init();
	double getDeltaTime();
	double tick();
private:
	double m_lastTime;
	double m_currentTime;
	FPSManager();
};

#endif
