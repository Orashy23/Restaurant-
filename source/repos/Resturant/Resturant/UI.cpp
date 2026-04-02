#include "UI.h"
#include <iostream>
#include "Restaurant.h"
using namespace std;

UI::UI()
{
}

int UI::getMode()
{
	//Getting mode from the user
	int mode;
	cout << "Please select the mode of operation:\n" << "1. Interactive Mode\n" << "2. Silent Mode\n";
	cin >> mode;

	if (mode != 1 && mode != 2)
	{
		cout << "Invalid mode selected. Defaulting to Interactive Mode.\n";
		mode = 1;
	}

	if (mode==1)
	{
		cout << "/////////////////////\n" << "Entering interactive mode\n";
	}
	else if (mode==2)
	{
		cout << "/////////////////////\n" << "Entering silent mode\n";
	}
	
	return mode;
}

void UI::getFileNames(string& inFile, string& outFile)
{
	cout << "Enter the name of the input file (e.g., test1.txt): ";
	cin >> inFile;

	cout << "Enter the name of the output file (e.g., output1.txt): ";
	cin >> outFile;


}

void UI::printMsg(string msg)
{
		cout << msg << endl;
}

void UI::waitForClick()
{



}

void UI::printCurrentTimestep(int timestep, Restaurant* pRest)
{
	cout << "Current Timestep: " << timestep << endl;
	cout << "=========================================\n";

	// 1. Actions List
	cout << "========= Actions List =========\n";
	cout << pRest->getRequestActions()->getcount() + pRest->getCancelActions()->getcount() << " actions remaining: ";
	pRest->getRequestActions()->print();
	cout << "\n";

	// 2. Pending Orders
	cout << "\n========= Pending Orders =========\n";
	cout << pRest->getPend_ODN()->getcount() << " ODN: "; pRest->getPend_ODN()->print(); cout << "\n";
	cout << pRest->getPend_ODG()->getcount() << " ODG: "; pRest->getPend_ODG()->print(); cout << "\n";
	cout << pRest->getPend_OT()->getcount() << " OT: ";  pRest->getPend_OT()->print(); cout << "\n";
	cout << pRest->getPend_OVN()->getcount() << " OVN: "; pRest->getPend_OVN()->print(); cout << "\n";
	cout << pRest->getPend_OVG()->getCount() << " OVG: "; pRest->getPend_OVG()->print(); cout << "\n";
	cout << pRest->getPend_OVC()->getcount() << " OVC: "; pRest->getPend_OVC()->print(); cout << "\n";
	// 3. Available Chefs
	cout << "\n========= Available Chefs =========\n";
	cout << pRest->getFree_CN()->getcount() << " CN: "; pRest->getFree_CN()->print(); cout << "\n";
	cout << pRest->getFree_CS()->getcount() << " CS: "; pRest->getFree_CS()->print(); cout << "\n";

	// 4. Cooking Orders
	cout << "\n========= Cooking Orders =========\n";
	cout << pRest->getCooking_Orders()->getCount() << " cooking orders: "; pRest->getCooking_Orders()->print(); cout << "\n";

	// 5. Ready Orders
	cout << "\n========= Ready Orders =========\n";
	cout << pRest->getRDY_OD()->getcount() << " OD: "; pRest->getRDY_OD()->print(); cout << "\n";
	cout << pRest->getRDY_OT()->getcount() << " OT: "; pRest->getRDY_OT()->print(); cout << "\n";
	cout << pRest->getReady_OV()->getcount() << " OV: "; pRest->getReady_OV()->print(); cout << "\n";

	// 6. Available Scooters
	cout << "\n========= Available Scooters =========\n";
	cout << pRest->getFree_Scooters()->getCount() << " Scooters: "; pRest->getFree_Scooters()->print(); cout << "\n";

	// 7. Available Tables
	cout << "\n========= Available Tables =========\n";
	cout << pRest->getFree_Tables()->getCount() << " tables: "; pRest->getFree_Tables()->print(); cout << "\n";

	// 8. In-Service Orders
	cout << "\n========= In-Service Orders =========\n";
	cout << pRest->getInServ_Orders()->getCount() << " Orders: "; pRest->getInServ_Orders()->print(); cout << "\n";

	// 9. In-Maintenance & Returning Scooters
	cout << "\n========= Scooters (Maintenance & Back) =========\n";
	cout << pRest->getMaint_Scooters()->getcount() << " In-maintenance: "; pRest->getMaint_Scooters()->print(); cout << "\n";
	cout << pRest->getBack_Scooters()->getCount() << " Back to Restaurant: "; pRest->getBack_Scooters()->print(); cout << "\n";

	// 10. Finished & Cancelled Orders
	cout << "\n========= Finished & Cancelled Orders =========\n";
	cout << pRest->getCancelled_orders()->getcount() << " cancelled: "; pRest->getCancelled_orders()->print(); cout << "\n";
	cout << pRest->getFinished_Orders()->getcount() << " Finished: "; pRest->getFinished_Orders()->print(); cout << "\n";

	cout << "=========================================\n";
}

