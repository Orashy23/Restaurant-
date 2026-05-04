#include "Restaurant.h"
#include "Order.h"
#include "Action.h"
#include "Chef.h"
#include "Table.h"
#include "Scooter.h"
#include "UI.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
//123

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

void Restaurant::randomSimulate()
{
    srand((unsigned int)time(0));

    for (int i = 1; i <= 10; i++) Free_CS.enqueue(new Chef(i, true, 2));
    for (int i = 11; i <= 25; i++) Free_CN.enqueue(new Chef(i, false, 3));

    int tid = 1;
    for (int i = 0; i < 10; i++) Free_Tables.addTable(new Table(tid++, 4));
    for (int i = 0; i < 10; i++) Free_Tables.addTable(new Table(tid++, 6));
    for (int i = 0; i < 10; i++) Free_Tables.addTable(new Table(tid++, 8));

    for (int i = 1; i <= 15; i++)
        Free_Scooters.enqueue(new Scooter(i, 50, 5, 3), i);

    // Generate 500 orders
    string types[] = { "ODG", "ODN", "OT", "OVN", "OVC", "OVG" };
    int totalOrders = 500;
    for (int i = 1; i <= totalOrders; i++) {
        string type = types[rand() % 6];
        Order* pOrd = new Order(i, type, rand() % 200 + 50, rand() % 5 + 1, 0);
        if (type == "ODG" || type == "ODN")
            pOrd->setDineInInfo(rand() % 4 + 2, rand() % 10 + 3, rand() % 2 == 0);
        else if (type == "OVC" || type == "OVG" || type == "OVN")
            pOrd->setDistance(rand() % 900 + 100);
        AddtoPendingList(pOrd);
    }


    LinkedQueue<Chef*>    Busy_Chefs;
    LinkedQueue<Scooter*> InUse_Scooters;
    int done = 0, ts = 1;

    while (done < totalOrders) {
        for (int i = 0; i < 30; i++) {
            Order* pOrd = nullptr; bool got = false;
            int lc = rand() % 6;
            for (int a = 0; a < 6 && !got; a++) {
                int l = (lc+a)%6, p;
                if      (l==0 && !Pend_ODG.isEmpty())      { Pend_ODG.dequeue(pOrd);      got=true; }
                else if (l==1 && !Pend_ODN.isEmpty())      { Pend_ODN.dequeue(pOrd);      got=true; }
                else if (l==2 && !Pend_OT.isEmpty())       { Pend_OT.dequeue(pOrd);       got=true; }
                else if (l==3 && !Pend_OVN.isEmpty())      { Pend_OVN.dequeue(pOrd);      got=true; }
                else if (l==4 && !Pend_OVC_List.isEmpty()) { Pend_OVC_List.dequeue(pOrd); got=true; }
                else if (l==5 && !Pend_OVG.isEmpty())      { Pend_OVG.dequeue(pOrd,p);   got=true; }
            }
            if (!got) break;
            Chef* pChef = nullptr;
            if (rand()%2==0 && !Free_CS.isEmpty()) Free_CS.dequeue(pChef);
            else if (!Free_CN.isEmpty())           Free_CN.dequeue(pChef);
            else if (!Free_CS.isEmpty())           Free_CS.dequeue(pChef);
            if (!pChef) { AddtoPendingList(pOrd); break; }
            pChef->setIsFree(false);
            Busy_Chefs.enqueue(pChef);
            Cooking_Orders.enqueue(pOrd, pOrd->getSize());
        }

        for (int i = 0; i < 15; i++) {
            if (rand()%100 >= 75) continue;
            if (Cooking_Orders.isEmpty()) break;
            Order* pOrd; int p;
            Cooking_Orders.dequeue(pOrd, p);
            Chef* pChef;
            if (Busy_Chefs.dequeue(pChef)) {
                pChef->setIsFree(true);
                if (pChef->getIsSpecial()) Free_CS.enqueue(pChef);
                else Free_CN.enqueue(pChef);
            }
            string t = pOrd->getType();
            if (t=="ODG"||t=="ODN") RDY_OD.enqueue(pOrd);
            else if (t=="OT")       RDY_OT.enqueue(pOrd);
            else                    Ready_OV_List.enqueue(pOrd);
        }

        for (int i = 0; i < 10; i++) {
            Order* pOrd = nullptr; bool got = false;
            int lc = rand() % 3;
            for (int a = 0; a < 3 && !got; a++) {
                int l = (lc+a)%3;
                if      (l==0 && !RDY_OD.isEmpty())        { RDY_OD.dequeue(pOrd);        got=true; }
                else if (l==1 && !RDY_OT.isEmpty())        { RDY_OT.dequeue(pOrd);        got=true; }
                else if (l==2 && !Ready_OV_List.isEmpty()) { Ready_OV_List.dequeue(pOrd); got=true; }
            }
            if (!got) break;
            string t = pOrd->getType();
            if (t=="OT") { Finished_Orders.push(pOrd); done++; }
            else if (t=="OVN"||t=="OVC"||t=="OVG") {
                Scooter* ps; int sp;
                if (!Free_Scooters.isEmpty()) {
                    Free_Scooters.dequeue(ps,sp); ps->increaseOderCounter(); InUse_Scooters.enqueue(ps);
                }
                InServ_Orders.enqueue(pOrd, rand()%20+1);
            }
            else {
                int seats = pOrd->getSeats(); if (seats==0) seats=2;
                Table* pt = Free_Tables.getBest(seats);
                if (pt) Busy_No_Share.addTable(pt);
                InServ_Orders.enqueue(pOrd, rand()%20+1);
            }
        }

        { int id=rand()%totalOrders+1; Order* po; if(Pend_OVC_List.Cancel_Order(id,po)){Cancelled_orders.enqueue(po);done++;} }
        { int id=rand()%totalOrders+1; Order* po; if(Ready_OV_List.CancelOrder(id,po)){Cancelled_orders.enqueue(po);done++;} }
        {
            int id=rand()%totalOrders+1; Order* po;
            if(Cooking_Orders.CancelOrder(id,po)){
                Chef* pc;
                if(Busy_Chefs.dequeue(pc)){pc->setIsFree(true); if(pc->getIsSpecial())Free_CS.enqueue(pc); else Free_CN.enqueue(pc);}
                Cancelled_orders.enqueue(po); done++;
            }
        }

        if (rand()%100<25 && !InServ_Orders.isEmpty()) {
            Order* po; int p; InServ_Orders.dequeue(po,p);
            string t = po->getType();
            if (t=="OVN"||t=="OVC"||t=="OVG") {
                Scooter* ps; if(InUse_Scooters.dequeue(ps)) Back_Scooters.enqueue(ps,ps->getSpeed());
            } else if (t=="ODG"||t=="ODN") {
                Table* pt; int tp;
                if(!Busy_No_Share.isEmpty()){Busy_No_Share.dequeue(pt,tp); Free_Tables.addTable(pt);}
            }
            Finished_Orders.push(po); done++;
        }

        if (rand()%100<50 && !Back_Scooters.isEmpty()) {
            Scooter* ps; int p; Back_Scooters.dequeue(ps,p);
            if(ps->needsMaintenance()) Maint_Scooters.enqueue(ps);
            else Free_Scooters.enqueue(ps, ps->getSpeed());
        }

        if (rand()%100<50 && !Maint_Scooters.isEmpty()) {
            Scooter* ps; Maint_Scooters.dequeue(ps);
            ps->AfterMaintenance(); Free_Scooters.enqueue(ps, ps->getSpeed());
        }

        UI pUI;
        pUI.printCurrentTimestep(ts, this);
        pUI.waitForClick();
        ts++;
    }
    cout << "\n=== Done in " << ts-1 << " timesteps. Finished:" << Finished_Orders.getcount()
         << " Cancelled:" << Cancelled_orders.getcount() << " ===\n";
}


// This function is called at the end of each timestep to assign free scooters to ready delivery orders.

void Restaurant::assignReadyToService(int currentTimestep)
{
    Scooter* pScooter;
    Order* pOrd;

    // ==========================================
    // PRIORITY 1: Assign Cold Orders (OVC) First
    // ==========================================
    while (!RDY_OVC.isEmpty() && !Free_Scooters.isEmpty())
    {
        RDY_OVC.dequeue(pOrd);
        Free_Scooters.dequeue(pScooter);

        // 1. Calculate Delivery Times
        // We use ceil() to round up in case distance/speed is a decimal
        int t_serv = ceil((float)pOrd->getDistance() / pScooter->getSpeed());
        int t_f = currentTimestep + t_serv;

        pOrd->setFinishTime(t_f);

        // 2. Update Scooter Stats
        pScooter->addDistance(pOrd->getDistance());
        pScooter->setReturnTime(t_f + t_serv); // Time to drive there + time to drive back

        // 3. The Handoff
        pOrd->setScooter(pScooter);

        // 4. Move to In-Service
        InServ_Orders.enqueue(pOrd);
    }

    // ==========================================
    // PRIORITY 2: Assign Remaining Delivery Orders
    // ==========================================
    while (!Ready_OV.isEmpty() && !Free_Scooters.isEmpty())
    {
        Ready_OV.dequeue(pOrd);
        Free_Scooters.dequeue(pScooter);

        // Exact same math as above
        int t_serv = ceil((float)pOrd->getDistance() / pScooter->getSpeed());
        int t_f = currentTimestep + t_serv;

        pOrd->setFinishTime(t_f);

        pScooter->addDistance(pOrd->getDistance());
        pScooter->setReturnTime(t_f + t_serv);

        pOrd->setScooter(pScooter);
        InServ_Orders.enqueue(pOrd);
    }

    // ==========================================
    // PRIORITY 3: Assign Dine-in Orders (OD)
    // ==========================================
    while (!RDY_OD.isEmpty())
    {
        RDY_OD.peek(pOrd);
        int reqSeats = pOrd->getSeats(); // Make sure Order has getSeats()

        // 1. Try to find a shared table first
        Table* assignedTable = Busy_Sharable.getBest(reqSeats);

        // 2. If no shared table fits, try a completely free table
        if (assignedTable == nullptr) {
            assignedTable = Free_Tables.getBest(reqSeats);
        }

        // 3. Did we find ANY table?
        if (assignedTable != nullptr)
        {
            RDY_OD.dequeue(pOrd);

            // Calculate Service Time (Placeholder math: adjust based on your project rules!)
            int t_serv = reqSeats * 2;
            pOrd->setFinishTime(currentTimestep + t_serv);

            // Update table seats
            assignedTable->setOccupiedSeats(assignedTable->getOccupiedSeats() + reqSeats);

            // Move the table to the correct Busy queue (Assuming you have a way to check if it's full)
            // For now, we will push it to Busy_No_Share if we assume it's full
            Busy_No_Share.addTable(assignedTable);

            // The Handoff
            pOrd->setTable(assignedTable); // Make sure Order has setTable(Table* t)
            InServ_Orders.enqueue(pOrd, pOrd->getFinishTime()); // Push with priority
        }
        else
        {
            break; // No tables left! Order waits until next timestep.
        }
    }

    // ==========================================
    // PRIORITY 4: Assign Takeaway Orders (OT)
    // ==========================================
    while (!RDY_OT.isEmpty())
    {
        RDY_OT.dequeue(pOrd);

        int t_serv = 2; // Placeholder: Takeaways just need packing time
        pOrd->setFinishTime(currentTimestep + t_serv);

        InServ_Orders.enqueue(pOrd, pOrd->getFinishTime());
    }

}


void Restaurant::updateInServiceOrders(int currentTimestep)
{
    Order* pOrd;
    int pri; // Variable to hold the priority when dequeuing

    // Get the exact size before looping to prevent an infinite loop
    int currentSize = InServ_Orders.getcount();

    for (int i = 0; i < currentSize; i++)
    {
        // Dequeue the order and its priority
        InServ_Orders.dequeue(pOrd, pri);

        // --- CHECK IF THE ORDER IS DONE ---
        if (pOrd->getFinishTime() == currentTimestep)
        {
            // 1. Move the food to the Finished Stack
            Finished_Orders.push(pOrd);

            string type = pOrd->getType();

            // 2. Handle Delivery Resources (Scooters)
            if (type == "OVN" || type == "OVG" || type == "OVC")
            {
                Scooter* pScooter = pOrd->getScooter();
                if (pScooter != nullptr)
                {
                    // Send the scooter back, using its return time as the priority
                    Back_Scooters.enqueue(pScooter, pScooter->getReturnTime());
                }
            }
            // 3. Handle Dine-in Resources (Tables)
            else if (type == "ODN" || type == "ODG")
            {
                Table* pTable = pOrd->getTable();
                if (pTable != nullptr)
                {
                    // Subtract the people who just finished eating
                    int newOccupied = pTable->getOccupiedSeats() - pOrd->getSeats();
                    pTable->setOccupiedSeats(newOccupied);

                    // Where does the table go now?
                    if (newOccupied <= 0)
                    {
                        pTable->setOccupiedSeats(0); // Safety check
                        Free_Tables.addTable(pTable); // It is completely empty!
                    }
                    else
                    {
                        Busy_Sharable.addTable(pTable); // Still has people, but seats freed up
                    }
                }
            }
            // NOTE: Takeaway ("OT") orders do nothing here. They just grab food and leave!
        }
        else
        {
            // --- NOT DONE YET ---
            // Push it back into the In-Service queue with its finish time as the priority
            InServ_Orders.enqueue(pOrd, pOrd->getFinishTime());
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
