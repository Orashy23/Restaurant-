#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ArrayStack.h"

// Derived Classes Headers
#include "Pend_OVC.h"    
#include "Cook_Ords.h"   
#include "RDY_OV.h"      
#include "Fit_Tables.h"  

class Order;
class Chef;
class Table;
class Scooter;
class Action;

class Restaurant
{
private:
    ////////////// 1. Action Lists (2) /////////////
    LinkedQueue<Action*> Request_Actions; // Q actions from file
    LinkedQueue<Action*> Cancel_Actions;  // X actions from file

    ////////////// 2. Pending Orders (6) /////////////
    LinkedQueue<Order*> Pend_ODG; // Pending Dine-in Grilled
    LinkedQueue<Order*> Pend_ODN; // Pending Dine-in Normal
    LinkedQueue<Order*> Pend_OT;  // Pending Takeaway
    LinkedQueue<Order*> Pend_OVN; // Pending Delivery Normal
    priQueue<Order*> Pend_OVG;    // Pending Delivery Grilled (Priority)
    Pend_OVC Pend_OVC_List;       // Derived: supports CancelOrder(ID)

    ////////////// 3. Available Chefs (2) /////////////
    LinkedQueue<Chef*> Free_CS; // Available Special Chefs
    LinkedQueue<Chef*> Free_CN; // Available Normal Chefs

    ////////////// 4. Tables (3) /////////////
    Fit_Tables Free_Tables;     // Derived: supports getBest()
    Fit_Tables Busy_Sharable;   // Derived: supports getBest()
    Fit_Tables Busy_No_Share;   // Derived: supports getBest()

    ////////////// 5. In-Execution / Served (2) /////////////
    Cook_Ords Cooking_Orders;       // Derived: supports CancelOrder(ID)
    priQueue<Order*> InServ_Orders; // Priority: based on service duration

    ////////////// 6. Ready Orders (3) /////////////
    LinkedQueue<Order*> RDY_OD; // Ready Dine-in orders
    LinkedQueue<Order*> RDY_OT; // Ready Takeaway orders
    RDY_OV Ready_OV_List;       // Derived: supports CancelOrder(ID)

    ////////////// 7. Scooters (3) /////////////
    priQueue<Scooter*> Free_Scooters; // Priority: shortest distance
    priQueue<Scooter*> Back_Scooters; // Priority: return distance
    LinkedQueue<Scooter*> Maint_Scooters; // Scooters in maintenance

    ////////////// 8. Finished / Cancelled (2) /////////////
    LinkedQueue<Order*> Cancelled_orders; // List of all cancelled orders
    ArrayStack<Order*> Finished_Orders;   // All completed orders

public:
    Restaurant();
    ~Restaurant();

    // Core Functions
    void AddtoPendingList(Order* pOrd);
    void ExecuteActions(int currentTimestep);
	void CancelOrder(int id);
	// simulation function
    void randomSimulate();
};