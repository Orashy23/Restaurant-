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
	else if ()
	{
		cout << "/////////////////////\n" << "Entering silent mode\n";
	}
	
	return mode;
}

void UI::getFileNames(string& inFile, string& outFile)
{



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
	cout << "===============================" << endl;
	cout << "Actions List\n";

	cout << "\n========= Pending Orders =========\n";
	// pRest->getPend_ODN()->print();
	// pRest->getPend_OVG()->print(); 
	// pRest->getPend_OVC()->print();


	// 3. Available Chefs
	cout << "\n========= Available Chefs =========\n";

	

	// 4. Cooking Orders
	cout << "\n========= Cooking Orders =========\n";

	// 5. Ready Orders
	cout << "\n========= Ready Orders =========\n";

	// 6. Available Scooters
	cout << "\n========= Available Scooters =========\n";

	// 7. Available Tables
	cout << "\n========= Available Tables =========\n";

	// 8. In-Service Orders
	cout << "\n========= In-Service Orders =========\n";

	// 9. In-Maintenance & Returning Scooters
	cout << "\n========= Scooters (Maintenance & Back) =========\n";

	// 10. Finished & Cancelled Orders
	cout << "\n========= Finished & Cancelled Orders =========\n";

	cout << "=========================================\n";
}

