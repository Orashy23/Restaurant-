#include "Restaurant.h"
#include "Order.h"
#include "Action.h"

Restaurant::Restaurant()
{}

void Restaurant::AddtoPendingList(Order* pOrd)
{
    string type = pOrd->getType();
    if (type == "ODN")
    {
        PENDODN.enqueue(pOrd);
    }

    else if (type == "ODG")
    {
        PENDODG.enqueue(pOrd);
    }
    else if (type == "OT")
    {
        Pend_OT.enqueue(pOrd);
    }
    else if (type == "OVN")
    {
        Pend_OVN.enqueue(pOrd);
    }
    else if (type == "OVC")
    {
        Pend_OVC_List.enqueue(pOrd);
    }
    else if (type == "OVG")
    {
        float priorty = pOrd->getPriority();
        Pend_OVG.enqueue(pOrd, priorty);
    }
}

void Restaurant::ExecuteActions(int currentTimestep)
{
    Action* pact;
    while (Request_Actions.peek(pact) && pact->getTime() == currentTimestep)
    {
        Request_Actions.dequeue(pact);
        pact->act(this);
        delete pact; // Clean up memory after execution
	}
}
void Restaurant::CancelOrder(int id)
{
	Order* pOrd;
    if (Pend_OVC_List.Cancel_Order(id, pOrd)) {
        Cancelled_orders.enqueue(pOrd);
        return;
    }
    else if (Cooking_Orders.CancelOrder(id, pOrd)) {
        Cancelled_orders.enqueue(pOrd);
        return;
    }
    else if (Ready_OV_List.CancelOrder(id, pOrd)) {
        Cancelled_orders.enqueue(pOrd);
        return;
	}
    
}


Restaurant::~Restaurant()
{
	Order* pOrd;
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
	while (Pend_OVG.dequeue(pOrd))
		delete pOrd;
	while (Cooking_Orders.dequeue(pOrd))
		delete pOrd;
	while (Ready_OV_List.dequeue(pOrd))
		delete pOrd;
	while (Cancelled_orders.dequeue(pOrd))
		delete pOrd;
	while (Finished_Orders.pop(pOrd))
		delete pOrd;
    while(Cooking_Orders.dequeue(pOrd))
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