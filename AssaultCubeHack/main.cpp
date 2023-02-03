#include <iostream>
#include "Process.h"
#include "hack.h"

//#include "Trainer.h"

int main() {
	// Define Target Process And Attach
	Process* tProc = new Process(L"ac_client.exe");
	if (!tProc->status) {
		delete tProc;
		return 0;
	}

	// Init Hack
	Hack* hack = new Hack(tProc);
	hack->setWindow();
	hack->printToWindow();

	// hack Loop
	while (!hack->getExitStatus()) {
		hack->update();
		hack->getKeyState();
		
		if (hack->getUpdateFlag()) {
			hack->printToWindow();
			hack->setUpdateFlag(false);
		}
		
		hack->update();
		hack->executeFeatures();
	}

	// Cleanup on Exit
	delete hack;
	delete tProc;
	return 0;
}





//int main() {
//	// Define Target Process And Attach
//	Process* tProc = new Process(L"ac_client.exe");
//	if (!tProc->status) {
//		delete tProc;
//		return 0;
//	}
//
//	// Create Trainer --- MIGRRATE TO HACK ONCE COMPLETE
//	Trainer* Hack = new Trainer(tProc);
//
//	// Create Display Window
//	Hack->setWindow();
//	Hack->printToWindow();
//
//	// Main Loop
//	while (!Hack->getExitStatus()) {
//
//		Hack->updateAddresses();
//		Hack->getKeyState();
//
//		if (Hack->getUpdateFlag()) {
//			Hack->printToWindow();
//			Hack->setUpdateFlag(false);
//		}
//
//		Hack->executeFeatures();
//
//		Sleep(1);
//	}
//
//	delete Hack;
//	delete tProc;
//	return 0;
//}