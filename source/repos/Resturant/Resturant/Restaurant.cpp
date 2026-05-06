#include "Restaurant.h"
#include "Order.h"
#include "Action.h"
#include "Chef.h"
#include "Table.h"
#include "Scooter.h"
#include "UI.h"
#include "RequestAction.h"
#include "CancelAction.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

Restaurant::Restaurant()
{}

void Restaurant::AddtoPendingList(Order* pOrd)
{
    string type = pOrd->getType();
    if (type == "COMBO") Pend_COMBO.enqueue(pOrd);
    else if (type == "ODN")        Pend_ODN.enqueue(pOrd);
    else if (type == "ODG")   Pend_ODG.enqueue(pOrd);
    else if (type == "OT")    Pend_OT.enqueue(pOrd);
    else if (type == "OVN")   Pend_OVN.enqueue(pOrd);
    else if (type == "OVC")   Pend_OVC_List.enqueue(pOrd);
    else if (type == "OVG") { 
        int priority = pOrd->getPrice() + (pOrd->getSize() * 10) - (pOrd->getDistance() / 5);
        pOrd->setPriority(priority);
        Pend_OVG.enqueue(pOrd, pOrd->getPriority()); 
    }
}

void Restaurant::setorder(Order* pOrd, Chef* chef, int time) {
    pOrd->setChef(chef);
    pOrd->setTA(time);
    int cooktime = ceil((float)pOrd->getSize() / chef->getSpeed());
    pOrd->setTR(cooktime + time);
    Cooking_Orders.enqueue(pOrd, -pOrd->getTR());
    chef->setIsFree(false);
}

void Restaurant::assignpendingtochef(int currentTimestep) {
    Chef* chef;
    Order* pOrd;
    int priority;
    while (!Pend_ODG.isEmpty() && !Free_CS.isEmpty()) {
        Free_CS.dequeue(chef);
        Pend_ODG.dequeue(pOrd);
        setorder(pOrd, chef, currentTimestep);
    }
    while (!Pend_ODN.isEmpty()&& (!Free_CN.isEmpty() || !Free_CS.isEmpty())) {
        Pend_ODN.dequeue(pOrd);
        if (!Free_CN.isEmpty()) {
            Free_CN.dequeue(chef);
        }
        else if (!Free_CS.isEmpty()) {
            Free_CS.dequeue(chef);
        }
        else {
            break;
        }
        setorder(pOrd, chef, currentTimestep);

    }
    while (!Pend_OT.isEmpty() && !Free_CN.isEmpty()) {
        Free_CN.dequeue(chef);
        Pend_OT.dequeue(pOrd);
        setorder(pOrd, chef, currentTimestep);
    }
    while (!Pend_OVG.isEmpty() && !Free_CS.isEmpty()) {
        Free_CS.dequeue(chef);
        Pend_OVG.dequeue(pOrd, priority);
        setorder(pOrd, chef, currentTimestep);
    }
    while (!Pend_OVC_List.isEmpty() && (!Free_CN.isEmpty() || !Free_CS.isEmpty())) {
        Pend_OVC_List.dequeue(pOrd);
        if (!Free_CN.isEmpty()) {
            Free_CN.dequeue(chef);
        }
        else if (!Free_CS.isEmpty()) {
            Free_CS.dequeue(chef);
        }
        else {
            break;
        }
        setorder(pOrd, chef, currentTimestep);
    }
    while (!Pend_OVN.isEmpty() && !Free_CN.isEmpty()) {
        Free_CN.dequeue(chef);
        Pend_OVN.dequeue(pOrd);
        setorder(pOrd, chef, currentTimestep);
    }
}



void Restaurant::ExecuteActions(int currentTimestep)
{
    Action* pact;
    while (Request_Actions.peek(pact) && pact->getTime() == currentTimestep) {
        Request_Actions.dequeue(pact);
        pact->act(this);
        delete pact;
    }
    while (Cancel_Actions.peek(pact) && pact->getTime() == currentTimestep) {
        Cancel_Actions.dequeue(pact);
        pact->act(this);
        delete pact;
    }
}

void Restaurant::CancelOrder(int id)
{
    Order* pOrd;
    if (Pend_OVC_List.Cancel_Order(id, pOrd))    { Cancelled_orders.enqueue(pOrd); return; }
    else if (Cooking_Orders.CancelOrder(id, pOrd)) 
    {
		// If the order is currently being cooked, we also need to free up the chef
		Chef* c = pOrd->getChef();
        if (c) {
            c->setIsFree(true);
            if (c->getIsSpecial())
                Free_CS.enqueue(c);
            else
                Free_CN.enqueue(c);
        }
        Cancelled_orders.enqueue(pOrd); return;
    }
    else if (Ready_OV_List.CancelOrder(id, pOrd))  { Cancelled_orders.enqueue(pOrd); return; }
}

void Restaurant::loadFile(string filename)
{
    int CN, CS, CN_Speed, CS_Speed, S_count, S_speed;
    int Main_Ords, Main_Dur;
    int Table_numbers;
    int Total_Action;


    ifstream input(filename);
    input >> CN >> CS
        >> CN_Speed >> CS_Speed
        >> S_count >> S_speed
        >> Main_Ords >> Main_Dur
        >> Table_numbers;

    int tablesCreated = 0;
    int tableID = 1;
    int Table_Count, Capacity;
	CS_Count = CS;
	CN_Count = CN;
	Scotter_Count = S_count;


    while (tablesCreated < Table_numbers)
    {
        input >> Table_Count >> Capacity;
        for (int i = 0; i < Table_Count; i++)
            Free_Tables.addTable(new Table(tableID++, Capacity));
        tablesCreated += Table_Count;
    }
	input >> TH >> Total_Action;
	char Action_Type;
	int Action_Time, ID, Size, Price, Seats, Duration, CanShareInt, Distance;
	int Action_Count = 0;


    while (Total_Action > Action_Count) {
        input >> Action_Type;
        if (Action_Type == 'X'){
            input >> Action_Time >> ID;
			CancelAction* cancelAct = new CancelAction(Action_Time, ID);
            Cancel_Actions.enqueue(cancelAct);
            Action_Count++;
        }
        else if (Action_Type == 'Q')
        {
            string type;
            input >> type >> Action_Time >> ID >> Size >> Price;
            int Seats = 0, Duration = 0, Distance = 0;
            bool CanShare = false;

            if (type == "ODG" || type == "ODN")
            {
                char canShareChar;
                input >> Seats >> Duration >> canShareChar;
                CanShare = (canShareChar == 'Y');
            }
            else if (type == "OVC" || type == "OVG" || type == "OVN")
            {
                input >> Distance;
            }

            else if (type == "COMBO") {
                int nChefs, nScooters;
                input >> Distance >> nChefs >> nScooters;
                RequestAction* requestAct = new RequestAction(Action_Time, type, ID, Size, Price, 0, 0, false, Distance, nChefs, nScooters);
                Request_Actions.enqueue(requestAct);
                Action_Count++;
                continue;
            }

            RequestAction* requestAct = new RequestAction(Action_Time, type, ID, Size, Price, Seats, Duration, CanShare, Distance);
            Request_Actions.enqueue(requestAct);
            Action_Count++;
        }


    
    }


    for (int i = 1; i <= CN; i++) {
        Free_CN.enqueue(new Chef(i, false, CN_Speed));
    }
    for (int i = 1+CN; i <=CS + CN; i++) {
        Free_CS.enqueue(new Chef(i, true, CS_Speed));
    }
    for (int i = 1; i <= S_count; i++) {
        Free_Scooters.enqueue(new Scooter(i, S_speed, Main_Ords, Main_Dur), 0);
	}
        

    
}


void Restaurant::assignTakeawayOrders(int currentTimestep)
{
    LinkedQueue<Order*> notReadyYet;
    Order* pOrd;

    while (RDY_OT.dequeue(pOrd))
    {
        // Must have waited at least 1 full timestep since becoming ready
        if (currentTimestep >= pOrd->getTR() + 1)
        {
            pOrd->setTS(currentTimestep);
            pOrd->setTF(currentTimestep);
            Finished_Orders.push(pOrd);
        }
        else
        {
            notReadyYet.enqueue(pOrd);
        }
    }

    // Put orders that aren't ready yet back in the queue
    while (notReadyYet.dequeue(pOrd))
        RDY_OT.enqueue(pOrd);
}

// ─────────────────────────────────────────────────────────────
// Dine-in: try sharing first, then free tables. Best-fit always.
// ─────────────────────────────────────────────────────────────
void Restaurant::assignDineInOrders(int currentTimestep)
{
    LinkedQueue<Order*> unassigned;
    Order* pOrd;

    while (RDY_OD.dequeue(pOrd))
    {
        int seats = pOrd->getSeats();
        Table* pt = nullptr;

        //Priority 1: try an already-occupied sharable table
        pt = Busy_Sharable.getBest(seats);
        if (pt)
        {
            // Reduce free seats on that table
            pt->setFreeSeats(pt->getFreeSeats() - seats);

            // If seats remain, it can still accept more customers
            if (pt->getFreeSeats() > 0)
                Busy_Sharable.addTable(pt);
            else
                Busy_No_Share.addTable(pt); // full now, no more sharing
        }
        else
        {
            // Priority 2: use a brand-new free table
            pt = Free_Tables.getBest(seats);
            if (pt)
            {
                pt->setFreeSeats(pt->getFreeSeats() - seats);

                // Where the table goes depends on the customer's sharing preference
                if (pOrd->getCanShare() && pt->getFreeSeats() > 0)
                    Busy_Sharable.addTable(pt); // others can still join
                else
                    Busy_No_Share.addTable(pt); // no sharing allowed or table is full
            }
        }

        if (pt) // successfully assigned
        {
            pOrd->setTable(pt);
            pOrd->setTS(currentTimestep);
            pOrd->setTF(currentTimestep + pOrd->getDuration());
            // Negative TF as priority → earliest finishing order at the front
            InServ_Orders.enqueue(pOrd, -(pOrd->getTF()));
        }
        else
        {
            unassigned.enqueue(pOrd); // no table found, try again next timestep
        }
    }

    while (unassigned.dequeue(pOrd))
        RDY_OD.enqueue(pOrd);
}


 



// Helper function to handle assigning a single delivery order to a scooter
void Restaurant::assignOneDeliveryOrder(Order* ord, int currentTimestep)
{
    Scooter* ps;
    int sp;

    Free_Scooters.dequeue(ps, sp);

    int tserv = (int)ceil((float)ord->getDistance() / ps->getSpeed());

    ord->setTS(currentTimestep);
    ord->setTF(currentTimestep + tserv);

    ps->addDistance(ord->getDistance());
    ps->increaseOderCounter();   // Tracks when maintenance is needed
    ord->setScooter(ps);

    int returnArrival = ord->getTF() + (int)ceil((float)ord->getDistance() / ps->getSpeed());
    ps->setReturnTime(returnArrival);

    InServ_Orders.enqueue(ord, -(ord->getTF()));
}


void Restaurant::assignDeliveryOrders(int currentTimestep)
{
    Order* pOrd;

    // 1. Assign Overwait OVG orders FIRST (Highest Priority)
    while (!Overwait_OVG.isEmpty() && !Free_Scooters.isEmpty())
    {
        int pri;
        Overwait_OVG.dequeue(pOrd, pri);
        assignOneDeliveryOrder(pOrd, currentTimestep);
    }

    // 2. Split the single ready-OV list into cold vs. everything else
    LinkedQueue<Order*> ovcOrders;
    LinkedQueue<Order*> otherOrders;

    while (Ready_OV_List.dequeue(pOrd))
    {
        if (pOrd->getType() == "OVC")
            ovcOrders.enqueue(pOrd);
        else
            otherOrders.enqueue(pOrd);
    }

    // 3. Assign OVC second
    while (!ovcOrders.isEmpty() && !Free_Scooters.isEmpty())
    {
        ovcOrders.dequeue(pOrd);
        assignOneDeliveryOrder(pOrd, currentTimestep);
    }

    // 4. Then assign normal OVN / OVG last
    while (!otherOrders.isEmpty() && !Free_Scooters.isEmpty())
    {
        otherOrders.dequeue(pOrd);
        assignOneDeliveryOrder(pOrd, currentTimestep);
    }

    // Put any unassigned orders back into the ready list
    while (ovcOrders.dequeue(pOrd))   Ready_OV_List.enqueue(pOrd);
    while (otherOrders.dequeue(pOrd)) Ready_OV_List.enqueue(pOrd);
}


void Restaurant::updateInServiceOrders(int currentTimestep)
{
    int size = InServ_Orders.getCount();

    for (int i = 0; i < size; i++)
    {
        Order* pOrd; int pri;
        InServ_Orders.dequeue(pOrd, pri);

        if (pOrd->getTF() == currentTimestep)
        {
            string type = pOrd->getType();

            if (type == "OVN" || type == "OVG" || type == "OVC")
            {
                Scooter* ps = pOrd->getScooter();
                int returnDist = pOrd->getDistance();

                // Shorter return distance = arrives back sooner = higher priority
                // priQueue is max-first, so priority = -returnDist
                Back_Scooters.enqueue(ps, -returnDist);
            }
            else if (type == "ODG" || type == "ODN")
            {
                Table* pt = pOrd->getTable();
                if (pt)
                {
                    // Return the seats this order was using
                    pt->setFreeSeats(pt->getFreeSeats() + pOrd->getSeats());

                    if (pt->getFreeSeats() == pt->getCapacity())
                        Free_Tables.addTable(pt);   // fully empty → back to free pool
                    else
                        Busy_Sharable.addTable(pt); // still partially occupied
                }
            }
            // OT orders are finished directly in assignTakeawayOrders, nothing extra needed here

            Finished_Orders.push(pOrd);
        }
        else
        {
            // Not done yet — put it back
            InServ_Orders.enqueue(pOrd, pri);
        }
    }
}

// Check if all queues are empty // if the simulation is done or not
bool Restaurant::simulationDone() {
   
    return

        //bonus: by the great ghrbr:
        Overwait_OVG.isEmpty() &&

        Request_Actions.isEmpty() &&
        Cancel_Actions.isEmpty() &&

        Pend_ODG.isEmpty() &&
        Pend_ODN.isEmpty() &&
        Pend_OT.isEmpty() &&
        Pend_OVN.isEmpty() &&
        Pend_OVG.isEmpty() &&
        Pend_OVC_List.isEmpty() &&

        Cooking_Orders.isEmpty() &&

        RDY_OD.isEmpty() &&
        RDY_OT.isEmpty() &&
        Ready_OV_List.isEmpty() &&

        InServ_Orders.isEmpty() &&

        Back_Scooters.isEmpty() &&
        Maint_Scooters.isEmpty();
}

void Restaurant::updateStatisticsCounters() {

    Order* pOrd;
    int pri;
    priQueue<Order*> tempCookQueue;

    while (Cooking_Orders.dequeue(pOrd, pri)) {
        Chef* assignedChef = pOrd->getChef();
        if (assignedChef) {
            assignedChef->setTotalBusyTime(assignedChef->getTotalBusyTime() + 1);
        }
        tempCookQueue.enqueue(pOrd, pri);
    }
    // Restore the cooking queue
    while (tempCookQueue.dequeue(pOrd, pri)) {
        Cooking_Orders.enqueue(pOrd, pri);
    }


    priQueue<Order*> tempInServQueue;
    while (InServ_Orders.dequeue(pOrd, pri)) {
        // Only delivery orders have scooters
        if (pOrd->getType() == "OVC" || pOrd->getType() == "OVG" || pOrd->getType() == "OVN") {
            Scooter* assignedScooter = pOrd->getScooter();
            if (assignedScooter) {
                assignedScooter->setTotalBusyTime(assignedScooter->getTotalBusyTime() + 1);
            }
        }
        tempInServQueue.enqueue(pOrd, pri);
    }
    // Restore the in-service queue
    while (tempInServQueue.dequeue(pOrd, pri)) {
        InServ_Orders.enqueue(pOrd, pri);
    }


    Scooter* pScooter;
    priQueue<Scooter*> tempBackQueue;
    while (Back_Scooters.dequeue(pScooter, pri)) {
        pScooter->setTotalBusyTime(pScooter->getTotalBusyTime() + 1);
        tempBackQueue.enqueue(pScooter, pri);
    }
    // Restore the back scooters queue
    while (tempBackQueue.dequeue(pScooter, pri)) {
        Back_Scooters.enqueue(pScooter, pri);
    }
}

void Restaurant::promoteOverwaitOrders(int currentTimestep) {
    Order* pOrd;
    LinkedQueue<Order*> tempQueue;

    // Scan through all Ready Delivery orders
    while (Ready_OV_List.dequeue(pOrd)) {
        // Check if it is an OVG order AND if it has exceeded the threshold (TH)
        if (pOrd->getType() == "OVG" && (currentTimestep - pOrd->getTR() > TH)) {

            // Calculate priority: higher (currentTimestep - TQ) means served first
            int priority = currentTimestep - pOrd->getTQ();

            // Move it to the new overwait list
            Overwait_OVG.enqueue(pOrd, priority);
        }
        else {
            // Not overwait, keep it in the normal ready queue
            tempQueue.enqueue(pOrd);
        }
    }

    // Restore the regular ready list
    while (tempQueue.dequeue(pOrd)) {
        Ready_OV_List.enqueue(pOrd);
    }
}

void Restaurant::simulate() {
    UI ui;
    string inFile, outFile;

    // Initial configuration and file loading
    ui.getFileNames(inFile, outFile);
    int mode = ui.getMode();
    loadFile(inFile);

    if (mode == 2) ui.printMsg("Simulation Starts in Silent mode...");

    int currentTimestep = 1;

    // Primary simulation loop
    while (!simulationDone()) {
        // Process requests and cancellations for the current timestep
        ExecuteActions(currentTimestep);

        // Update resource availability before new assignments
        updateScooters(currentTimestep);
        assignpendingtochef(currentTimestep);

		promoteOverwaitOrders(currentTimestep); // Bonus feature: promote overwaiting OVG orders

        updateCookingOrders(currentTimestep);

        // Execute stage 2 assignments
        assignTakeawayOrders(currentTimestep);
        assignDineInOrders(currentTimestep);
        assignDeliveryOrders(currentTimestep);

        // Finalize orders reaching completion time
        updateInServiceOrders(currentTimestep);

        // Increment busy time counters for utilization statistics
        updateStatisticsCounters();

        // Handle interface output
        if (mode == 1) {
            ui.printCurrentTimestep(currentTimestep, this);
            ui.waitForClick();
        }

        currentTimestep++;
    }
    //comm
     
    totalTimesteps = currentTimestep - 1;
    writeOutput(outFile);
    if (mode == 2) ui.printMsg("Simulation ends, Output file created.");
}


// Move finished cooking orders to ready lists and free chefs
void Restaurant::updateCookingOrders(int currentTimestep) {
    priQueue<Order*> tempQueue;
    Order* pOrd;
    int pri;

    while (Cooking_Orders.dequeue(pOrd, pri)) {
        if (currentTimestep >= pOrd->getTR()) { // Order is ready
            Chef* pChef = pOrd->getChef();
            pChef->setIsFree(true);

            // Return chef to correct free list
            if (pChef->getIsSpecial()) {
                Free_CS.enqueue(pChef);
            }
            else {
                Free_CN.enqueue(pChef);
            }

            // Route order to correct ready list
            string type = pOrd->getType();
            if (type == "ODG" || type == "ODN") {
                RDY_OD.enqueue(pOrd);
            }
            else if (type == "OT") {
                RDY_OT.enqueue(pOrd);
            }
            else {
                Ready_OV_List.enqueue(pOrd);
            }
        }
        else {
            // Still cooking
            tempQueue.enqueue(pOrd, pri);
        }
    }

    // Restore cooking queue
    while (tempQueue.dequeue(pOrd, pri)) {
        Cooking_Orders.enqueue(pOrd, pri);
    }
}

// Manage returning and maintenance scooters
void Restaurant::updateScooters(int currentTimestep) {
    priQueue<Scooter*> tempBack;
    Scooter* pScooter;
    int pri;

    // Process returning scooters
    while (Back_Scooters.dequeue(pScooter, pri)) {
        if (currentTimestep >= pScooter->getReturnTime()) {
            if (pScooter->needsMaintenance()) {
                pScooter->setMaintFinishTime(currentTimestep + pScooter->getMaintananceTime());
                Maint_Scooters.enqueue(pScooter);
            }
            else {
                // Enqueue with negative total distance for shortest-traveled priority
                Free_Scooters.enqueue(pScooter, -(pScooter->getTotalDistance()));
            }
        }
        else {
            // Still returning
            tempBack.enqueue(pScooter, pri);
        }
    }

    // Restore back scooters queue
    while (tempBack.dequeue(pScooter, pri)) {
        Back_Scooters.enqueue(pScooter, pri);
    }

    LinkedQueue<Scooter*> tempMaint;

    // Process maintenance scooters
    while (Maint_Scooters.dequeue(pScooter)) {
        if (currentTimestep >= pScooter->getMaintFinishTime()) {
            pScooter->AfterMaintenance(); // Resets order counter
            Free_Scooters.enqueue(pScooter, -(pScooter->getTotalDistance()));
        }
        else {
            // Still in maintenance
            tempMaint.enqueue(pScooter);
        }
    }

    // Restore maintenance queue
    while (tempMaint.dequeue(pScooter)) {
        Maint_Scooters.enqueue(pScooter);
    }
}
void Restaurant::writeOutput(string filename)
{
    priQueue<Order*> Descending_TF;
    Order* ord;
    while (Finished_Orders.pop(ord))
    {
        Descending_TF.enqueue(ord, ord->getTF());

    }
    ofstream output(filename);
    int pri;
    int Total_Order = 0;
    int ODG = 0, ODN = 0, OT = 0, OVN = 0, OVC = 0, OVG = 0;
    int Total_CS = 0;
    int Total_CN = 0;
    int Scotters = 0;
    int Finished = 0;
    int Cancelled = 0;
    int Avg_Ti = 0;
    int Avg_Tc = 0;
    int Avg_Tw = 0;
    int Avg_Tserv = 0;

    while (Descending_TF.dequeue(ord, pri)) {
        int TQ = ord->getTQ();
        int TA = ord->getTA();
        int TS = ord->getTS();
        int TR = ord->getTR();
        int TF = ord->getTF();
        int Id = ord->getID();
        int Ti = (TA - TQ) + (TS - TR);
        int Tc = TR - TA;
        int Tw = Ti + Tc;
        int Tserv = TF - TS;


        output << TF << " " << Id << " " << TQ << " " << TA << " " << TR << " " << TS << " " << Ti << " " << Tc
            << " " << Tw << " " << Tserv << "\n";


        Finished++;

        string type = ord->getType();
        if (type == "ODG") ODG++;
        else if (type == "ODN") {
            ODN++;
        }
        else if (type == "OT") {
            OT++;
        }
        else if (type == "OVN") {
            OVN++;
        }
        else if (type == "OVC") {
            OVC++;
        }
        else if (type == "OVG") {
            OVG++;
        }

        Avg_Ti += Ti;
        Avg_Tc += Tc;
        Avg_Tw += Tw;
        Avg_Tserv += Tserv;

    }
    Cancelled = Cancelled_orders.getcount();
    Total_Order = Finished + Cancelled;
    Avg_Ti = Avg_Ti / Finished;
    Avg_Tc = Avg_Tc / Finished;
    Avg_Tw = Avg_Tw / Finished;
    Avg_Tserv = Avg_Tserv / Finished;

    output << "Total Orders: " << Total_Order << "\n";
    output << "ODG: " << ODG << ", ODN: " << ODN << ", OT: " << OT << ", OVN: " << OVN
        << ", OVC: " << OVC << ", OVG: " << OVG << "\n";
    output << "Total Chefs: " << CN_Count + CS_Count << "\n";
    output << "CS: " << CS_Count << " CN: " << CN_Count << "\n";
    output << "Total Scooters: " << Scotter_Count << "\n";
    output << "Finished Orders: " << Finished << " (" << (Finished * 100 / Total_Order) << "%)\n";
    output << "Cancelled Orders: " << Cancelled << " (" << (Cancelled * 100 / Total_Order) << "%)\n";
    output << "Average Ti: " << Avg_Ti << "\n";
    output << "Average Tc: " << Avg_Tc << "\n";
    output << "Average Tw: " << Avg_Tw << "\n";
    output << "Average Tserv: " << Avg_Tserv << "\n";
    int totalChefBusy = 0;
    Chef* pChef;
    LinkedQueue<Chef*> tempCN, tempCS;
    while (Free_CN.dequeue(pChef)) { 
        totalChefBusy += pChef->getTotalBusyTime();
        tempCN.enqueue(pChef);
    }
    while (Free_CS.dequeue(pChef)) {
        totalChefBusy += pChef->getTotalBusyTime(); 
        tempCS.enqueue(pChef);
    }
    while (tempCN.dequeue(pChef)) {
        Free_CN.enqueue(pChef);
    }
    while (tempCS.dequeue(pChef)) {
        Free_CS.enqueue(pChef);
    }
    float chefUtil = 0;
    if (totalTimesteps > 0)
        chefUtil = totalChefBusy * 100.0f / ((CN_Count + CS_Count) * totalTimesteps);
    output << "Chef Utilization: " << chefUtil << "%\n";

    int totalScooterBusy = 0;
    Scooter* pScoot; int sp;
    priQueue<Scooter*> tempScoot;
    while (Free_Scooters.dequeue(pScoot, sp)) {
        totalScooterBusy += pScoot->getTotalBusyTime(); tempScoot.enqueue(pScoot, sp); 
    }
    while (tempScoot.dequeue(pScoot, sp)) {
        Free_Scooters.enqueue(pScoot, sp);
    }
    float scootUtil = 0;
    if (totalTimesteps > 0)
        scootUtil = totalScooterBusy * 100.0f / (Scotter_Count * totalTimesteps);
    output << "Scooter Utilization: " << scootUtil << "%\n";
}












Restaurant::~Restaurant()
{
    Order* pOrd; int pri;
    while (Pend_ODN.dequeue(pOrd))            delete pOrd;
    while (Pend_ODG.dequeue(pOrd))            delete pOrd;
    while (Pend_OT.dequeue(pOrd))             delete pOrd;
    while (Pend_OVN.dequeue(pOrd))            delete pOrd;
    while (Pend_OVC_List.dequeue(pOrd))       delete pOrd;
    while (Pend_OVG.dequeue(pOrd, pri))       delete pOrd;
    while (Cooking_Orders.dequeue(pOrd, pri)) delete pOrd;
    while (RDY_OD.dequeue(pOrd))              delete pOrd;
    while (RDY_OT.dequeue(pOrd))              delete pOrd;
    while (Ready_OV_List.dequeue(pOrd))       delete pOrd;
    while (Cancelled_orders.dequeue(pOrd))    delete pOrd;
    while (Finished_Orders.pop(pOrd))         delete pOrd;
}
