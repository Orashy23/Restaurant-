#include "Restaurant.h"
#include "Order.h"
#include "Action.h"
#include "Chef.h"
#include "Table.h"
#include "Scooter.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
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
}


Restaurant::~Restaurant()
{
	Order* pOrd;
	int pri;
	while (Pend_ODN.dequeue(pOrd))
		delete pOrd;
	while (Pend_ODG.dequeue(pOrd))
		delete pOrd;
	while (Pend_OT.dequeue(pOrd))
		delete pOrd;
	while (Pend_OVN.dequeue(pOrd))
		delete pOrd;
	while (Pend_OVC_List.dequeue(pOrd))
		delete pOrd;
	while (Pend_OVG.dequeue(pOrd, pri))
		delete pOrd;
	while (Cooking_Orders.dequeue(pOrd,pri))
		delete pOrd;
	while (Ready_OV_List.dequeue(pOrd))
		delete pOrd;
	while (Cancelled_orders.dequeue(pOrd))
		delete pOrd;
	while (Finished_Orders.pop(pOrd))
		delete pOrd;
    while(Cooking_Orders.dequeue(pOrd , pri ))
		delete pOrd;
	while (Ready_OV_List.dequeue(pOrd))
		delete pOrd;
	while (RDY_OD.dequeue(pOrd))
		delete pOrd;
	while (RDY_OT.dequeue(pOrd))
		delete pOrd;
	while (Cancelled_orders.dequeue(pOrd))
		delete pOrd;
	while (Finished_Orders.pop(pOrd))
		delete pOrd;
}