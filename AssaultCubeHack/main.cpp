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

	Hack->TESTFUNC();

	// Create Display Window
	Hack->setWindow();
	//Hack->printToWindow();

	// Main Loop
	while (!Hack->getExitStatus()) {

		Hack->updateAddresses();
		Hack->getKeyState();

		if (Hack->getUpdateFlag()) {
			Hack->printToWindow();
			Hack->setUpdateFlag(false);
		}

		Hack->executeFeatures();

		Sleep(5);
	}

	delete Hack;
	delete tProc;
	return 0;
}