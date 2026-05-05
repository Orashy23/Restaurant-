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
using namespace std;

Restaurant::Restaurant()
{}

void Restaurant::AddtoPendingList(Order* pOrd)
{
    string type = pOrd->getType();
    if (type == "ODN")        Pend_ODN.enqueue(pOrd);
    else if (type == "ODG")   Pend_ODG.enqueue(pOrd);
    else if (type == "OT")    Pend_OT.enqueue(pOrd);
    else if (type == "OVN")   Pend_OVN.enqueue(pOrd);
    else if (type == "OVC")   Pend_OVC_List.enqueue(pOrd);
    else if (type == "OVG")   Pend_OVG.enqueue(pOrd, pOrd->getPriority());
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
    else if (Cooking_Orders.CancelOrder(id, pOrd)) { Cancelled_orders.enqueue(pOrd); return; }
    else if (Ready_OV_List.CancelOrder(id, pOrd))  { Cancelled_orders.enqueue(pOrd); return; }
}

void Restaurant::loadFile(string filename)
{
    int CN, CS, CN_Speed, CS_Speed, S_count, S_speed;
    int Main_Ords, Main_Dur;
    int Table_numbers;
    int TH, Total_Action;

    
	






    ifstream input(filename);
    input >> CN >> CS
        >> CN_Speed >> CS_Speed
        >> S_count >> S_speed
        >> Main_Ords >> Main_Dur
        >> Table_numbers;

    int tablesCreated = 0;
    int tableID = 1;
    int Table_Count, Capacity;

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

// ─────────────────────────────────────────────────────────────
// Takeaway: OT orders wait exactly 1 timestep after TR, then done.
// ─────────────────────────────────────────────────────────────
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

        // --- Priority 1: try an already-occupied sharable table ---
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
            // --- Priority 2: use a brand-new free table ---
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

// ─────────────────────────────────────────────────────────────
// Delivery: OVC gets priority, then all other OV types.
// ─────────────────────────────────────────────────────────────
void Restaurant::assignDeliveryOrders(int currentTimestep)
{
    // Split the single ready-OV list into cold vs. everything else
    LinkedQueue<Order*> ovcOrders;
    LinkedQueue<Order*> otherOrders;
    Order* pOrd;

    while (Ready_OV_List.dequeue(pOrd))
    {
        if (pOrd->getType() == "OVC")
            ovcOrders.enqueue(pOrd);
        else
            otherOrders.enqueue(pOrd);
    }

    // Helper lambda to assign one order to one scooter
    auto assignOne = [&](Order* ord)
        {
            Scooter* ps; int sp;
            Free_Scooters.dequeue(ps, sp);

            int tserv = (int)ceil((float)ord->getDistance() / ps->getSpeed());
            ord->setTS(currentTimestep);
            ord->setTF(currentTimestep + tserv);

            ps->addDistance(ord->getDistance());
            ps->increaseOderCounter();   // tracks maintenance threshold
            ord->setScooter(ps);

            // Negative TF → earliest finisher is at the front of InServ_Orders
            InServ_Orders.enqueue(ord, -(ord->getTF()));
        };

    // Assign OVC first
    while (!ovcOrders.isEmpty() && !Free_Scooters.isEmpty())
    {
        ovcOrders.dequeue(pOrd);
        assignOne(pOrd);
    }

    // Then assign OVN / OVG
    while (!otherOrders.isEmpty() && !Free_Scooters.isEmpty())
    {
        otherOrders.dequeue(pOrd);
        assignOne(pOrd);
    }

    // Put any unassigned orders back into the ready list
    while (ovcOrders.dequeue(pOrd))   Ready_OV_List.enqueue(pOrd);
    while (otherOrders.dequeue(pOrd)) Ready_OV_List.enqueue(pOrd);
}

// ─────────────────────────────────────────────────────────────
// Check every in-service order — finish those whose TF has arrived.
// ─────────────────────────────────────────────────────────────
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

            // --- Delivery order finished ---
            if (type == "OVN" || type == "OVG" || type == "OVC")
            {
                Scooter* ps = pOrd->getScooter();
                int returnDist = pOrd->getDistance();

                // Shorter return distance = arrives back sooner = higher priority
                // priQueue is max-first, so priority = -returnDist
                Back_Scooters.enqueue(ps, -returnDist);
            }
            // --- Dine-in order finished: release the table ---
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
