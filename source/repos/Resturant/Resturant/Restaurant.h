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

	int CS_Count;
	int CN_Count;
	int Scotter_Count;
    int totalTimesteps;

    //bonus:
    int TH; // Threshold for overwait
    priQueue<Order*> Overwait_OVG; // New bonus list


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

	void updateCookingOrders(int currentTimestep); // Move orders 
	void updateScooters(int currentTimestep); // Move scooters 
    bool simulationDone();

	void updateStatisticsCounters(); // Update any counters needed for final report generation 

	void promoteOverwaitOrders(int currentTimestep);
    priQueue<Order*>* getOverwaitOVG() { return &Overwait_OVG; };
  

public:

    
    void simulate();

    Restaurant();
    ~Restaurant();

    // Core Functions
    void AddtoPendingList(Order* pOrd);
    void assignpendingtochef(int currentTimestep);
    void setorder(Order* pOrd, Chef* chef, int time);
    void ExecuteActions(int currentTimestep);
	void CancelOrder(int id);
    // ==========================================
    // --- Getters for UI Printing ---
    // ==========================================

    // 1. Actions Lists
    LinkedQueue<Action*>* getRequestActions() { return &Request_Actions; }
    LinkedQueue<Action*>* getCancelActions() { return &Cancel_Actions; }

    // 2. Pending Orders
    LinkedQueue<Order*>* getPend_ODG() { return &Pend_ODG; }
    LinkedQueue<Order*>* getPend_ODN() { return &Pend_ODN; }
    LinkedQueue<Order*>* getPend_OT() { return &Pend_OT; }
    LinkedQueue<Order*>* getPend_OVN() { return &Pend_OVN; }
    priQueue<Order*>* getPend_OVG() { return &Pend_OVG; }
    Pend_OVC* getPend_OVC() { return &Pend_OVC_List; }

    // 3. Available Chefs
    LinkedQueue<Chef*>* getFree_CS() { return &Free_CS; }
    LinkedQueue<Chef*>* getFree_CN() { return &Free_CN; }

    // 4. Tables
    Fit_Tables* getFree_Tables() { return &Free_Tables; }
    Fit_Tables* getBusy_Sharable() { return &Busy_Sharable; }
    Fit_Tables* getBusy_No_Share() { return &Busy_No_Share; }

    // 5. In-Execution / Served
    Cook_Ords* getCooking_Orders() { return &Cooking_Orders; }
    priQueue<Order*>* getInServ_Orders() { return &InServ_Orders; }

    // 6. Ready Orders
    LinkedQueue<Order*>* getRDY_OD() { return &RDY_OD; }
    LinkedQueue<Order*>* getRDY_OT() { return &RDY_OT; }
    RDY_OV* getReady_OV() { return &Ready_OV_List; }

    // 7. Scooters
    priQueue<Scooter*>* getFree_Scooters() { return &Free_Scooters; }
    priQueue<Scooter*>* getBack_Scooters() { return &Back_Scooters; }
    LinkedQueue<Scooter*>* getMaint_Scooters() { return &Maint_Scooters; }

    // 8. Finished / Cancelled
    LinkedQueue<Order*>* getCancelled_orders() { return &Cancelled_orders; }
    ArrayStack<Order*>* getFinished_Orders() { return &Finished_Orders; }


    void assignTakeawayOrders(int currentTimestep);
    void assignDineInOrders(int currentTimestep);
    void assignDeliveryOrders(int currentTimestep);
    void updateInServiceOrders(int currentTimestep);


    void loadFile(string filename);
    void writeOutput(string filename);
};