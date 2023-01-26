#include <iostream>
#include "Process.h"
#include "Trainer.h"

int main() {
	// Define Target Process And Attach
	Process* tProc = new Process(L"ac_client.exe");
	if (!tProc->status) {
		delete tProc;
		return 0;
	}

	// Create Trainer (Hack)
	Trainer* Hack = new Trainer(tProc);

	// Create Display Window
	Hack->setWindow();

	// Main Loop
	while (!Hack->getExitStatus()) {
		// Updates
		Hack->updateAddresses();
		//Hack->updateGameData();
		Hack->getKeyState();
		Hack->printToWindow();

		// Features - Probably add function for aimbot
		Hack->executeFeatures();
	}

	delete Hack;
	delete tProc;
	return 0;
}