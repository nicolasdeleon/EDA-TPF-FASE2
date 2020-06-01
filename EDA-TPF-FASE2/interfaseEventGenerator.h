#pragma once

#include <queue>
#include <allegro5/allegro.h>
#include "eventHandling.h"
#include "Simulation.h"

using namespace std;

enum implEvent : eventTypes { E_Draw, E_CreateNode, E_Back, E_MngNodeCnx, E_Quit};

class cEventDraw : public genericEvent
{
public:
	eventTypes getType(void) { return E_Draw; }
};

class cEventCreateNodeScreen : public genericEvent
{
public:
	eventTypes getType(void) { return E_CreateNode; }
};

class cEventQuit : public genericEvent
{
public:
	eventTypes getType(void) { return E_Quit; }
};

class cEventBack : public genericEvent
{
public:
	eventTypes getType(void) { return E_Back; }
};

class cEventManageConnections : public genericEvent
{
public:
	eventTypes getType(void) { return E_MngNodeCnx; }
};


class interfaseEventGenerator : public eventGenerator
{

	public:
	interfaseEventGenerator();
	~interfaseEventGenerator();
	genericEvent * getEvent(void);
	void printMainMenu(void);
	void printMakingNode(void);
	void printManageConnections(void);
	void linkSimulation(Simulation* sim) {
		mySim = sim;
		currentNodes = mySim->getNodes();
	}

	
	
	private:

	
	queue <genericEvent*> guiEvents;
	ALLEGRO_EVENT_QUEUE* alEventQueue;
	ALLEGRO_DISPLAY* display;
	vector<GenericNode*> currentNodes;
	Simulation* mySim;
	bool imguiInit();
};
