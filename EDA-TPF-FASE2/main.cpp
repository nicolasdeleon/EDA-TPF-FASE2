#include <iostream>
#include "eventHandling.h"
#include "genericFSM.h"
#include "interfaseEventGenerator.h"
#include "Simulation.h"
#include <stdlib.h>


enum implStates: stateTypes {MainMenu, CreatingNode, ManageConnections, MakeTsx, dummyState};

using namespace std;
class FSMImplementation : public genericFSM
{
	
	
	private:
		
	#define TX(x) (static_cast<void (genericFSM::* )(genericEvent *)>(&FSMImplementation::x)) //casteo a funcion, por visual
		const fsmCell fsmTable[4][5] = {
			//  E_Draw							    E_CreateNode                       E_Back                             E_MngNodeCnx							E_MakeTsx													
			{  	{MainMenu,TX(printMainMenu)},		{CreatingNode,TX(dummyfunc)},	   {MainMenu,TX(dummyfunc)},	      {ManageConnections,TX(dummyfunc)},	{MakeTsx,TX(dummyfunc)}},  //MainMenu
			{	{CreatingNode,TX(printMakingNode)},	{ManageConnections,TX(dummyfunc)}, {MainMenu,TX(dummyfunc)},		  {CreatingNode,TX(dummyfunc)},		    {CreatingNode,TX(dummyfunc)}},  //CreatingNode
			{	{ManageConnections,TX(printManageCnx)},	{dummyState,TX(dummyfunc)},	   {MainMenu,TX(dummyfunc)}, {ManageConnections,TX(dummyfunc)},    {ManageConnections,TX(dummyfunc)}},  //ManageConnections 
			{	{MakeTsx,TX(printMakeTsx)},	        {MainMenu,TX(dummyfunc)},	       {MainMenu,TX(dummyfunc)},		      {MakeTsx,TX(dummyfunc)},		        {MakeTsx,TX(dummyfunc)}}	  //MakeTsx
			};
	
	//The action routines for the FSM
	//These actions should not generate fsmEvents
	
	void dummyfunc(genericEvent * ev)
	{
		return;
	}

	void printMainMenu(genericEvent* ev) {
		guiEvGen->printMainMenu();
		return;
	}

	void printMakingNode(genericEvent* ev) {
		guiEvGen->printMakingNode();
		return;
	}

	void printManageCnx(genericEvent* ev) {
		guiEvGen->printManageConnections();
		return;
	}

	void printMakeTsx(genericEvent* ev) {
		guiEvGen->printMakeTsx();
		return;
	}

	interfaseEventGenerator* guiEvGen;
	Simulation* sim;

	public:
	FSMImplementation(): genericFSM(&fsmTable[0][0],5,4,MainMenu){}
	void referenceGuiEvGen(interfaseEventGenerator * guiEvGenerator) {
		guiEvGen = guiEvGenerator;
	}
	void referenceNodes(Simulation* simulation) {
		sim = simulation;
	}
};


int main(int argc, char** argv) 
{
	Simulation sim;
	FSMImplementation fsm;
	interfaseEventGenerator guiEvGen;	//generador de UN tipo de eventos
	mainEventGenerator eventGen;	//generador de eventos de TODO el programa
	
	//netwEventGenerator netwEvGen;
	fsm.referenceGuiEvGen(&guiEvGen);
	fsm.referenceNodes(&sim);
	eventGen.attach(&guiEvGen);	//registro fuente de eventos


	srand(time(NULL));
	
	
	
	
	//esta parte revisar despues para cargar el json creo que el problema es el formato del json que cree habria que pedirle a nico o lucas un json que funcione para crearlo.

	json creador;
	//esta parte de importar al json habria que hacerla desde la gui como en el tp1 que te daba la direccion de la carpeta y te cargaba el json que querias

	string str = "json_creacion.json";
	
	//Abro el archivo y lo asigno a mi variable miembro blocks (de tipo json)
	ifstream blocks_file(str, ifstream::binary);

	blocks_file >> creador;
	if (!creador.empty()) 
	{	
		for (string node : creador["full-nodes"]) {
			
			sim.addNode("127.0.0.1", stoi(node), NodeType::FULL);
		}
		for (string node : creador["spv"]) {

			sim.addNode("127.0.0.1", stoi(node), NodeType::SPV);
		}
		for (auto node : sim.Nodes) {
			vector<string> addresses = creador["full-nodes"];
			node->giveAvailableNodes(addresses);
		}
	}


	// terminar las configs con un startNodes()
	sim.startNodes();


	guiEvGen.linkSimulation(&sim);
	/* Esto serian configuraciones pre iniciar el programa */

	

	//aqui llamo a mi nodo central para que empiece a hacer la funcion algoritmoparticular que esta en full node.
	//asumo que el nodo 1 (el del puerto 10) es el nodo que invoca este algoritmo.
	
	//toda esta primer parte es para elegir un nodo en el arreglo que sea full ver si es necesario despues en la implementacion de esto.
	
	/*
	int Num_nodo = -1;
	for (int i=0; i<sim.Nodes.size();i++)
	{
		if (sim.Nodes[i]->getType() == NodeType::FULL)
		{
			Num_nodo = i;
			i = sim.Nodes.size(); //para que salga del for, lo se re cabeza pero no importa tanto aca eso es mas bien de prueba
		}
	}

	//controlo que exista algun nodo full cargado
	if (Num_nodo != -1)
	{
		//pongo el mismo nodo como pingeado en la lista de pingeados
		NodeInfo node;

		
		//pongo los pingeados en el vector de pingeados
		for (int i = 0; i < 4; i++)
		{
			if (sim.Nodes[i]->getType() == NodeType::FULL && i != Num_nodo )
			{
				node.ip = sim.Nodes[i]->getIP();
				node.puerto = sim.Nodes[i]->getPort();
				FullNode* nodoTemp = dynamic_cast<FullNode*>(sim.Nodes[Num_nodo]);
				(*nodoTemp).pingedNodes.push_back(node);
			}
		}
		((FullNode*)(sim.Nodes[Num_nodo]))->algoritmoParticular();
		//ahora que tengo la lista con los pingednodes cargados empiezo a hacer el algoritmo propiamente dicho
	}
	else
	{
		cout << "no encontre ningun nodo full ocurrio un problema" << endl;
	}
	*/
	


	bool quit = false;
	bool showOnce = true;
	
	do
	{
		genericEvent * ev = eventGen.getNextEvent();

		/*
		if (showOnce && sim.getRequestAnswer("127.0.0.1:80").size() != 0) {
			cout << "Answer from 8080: " << sim.getRequestAnswer("127.0.0.1:80") << std::endl;
			showOnce = false;
		}*/
		if (ev != nullptr) 
		{
			if (ev->getType() == E_Quit)
			{
				quit = true;
			}
			else {
				sim.doNodePolls();
				fsm.cycle(ev);
			}
			delete ev;
		}
	} 
	while (!quit);
	
	//system("pause");
	return 0;
}
