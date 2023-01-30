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

	// Create Trainer
	Trainer* Hack = new Trainer(tProc);

	// Create Display Window
	Hack->setWindow();
	Hack->printToWindow();

	// Main Loop
	while (!Hack->getExitStatus()) {

		Hack->updateAddresses();
		Hack->getKeyState();

		if (Hack->getUpdateFlag()) {
			Hack->printToWindow();
			Hack->setUpdateFlag(false);
		}

		Hack->executeFeatures();

		Sleep(1);
	}

	delete Hack;
	delete tProc;
	return 0;
}