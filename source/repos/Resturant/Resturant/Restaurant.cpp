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
