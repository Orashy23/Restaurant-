#pragma once
#include "LinkedQueue.h"
#include "Order.h"

// Derived from LinkedQueue to support CancelOrder(ID) in ready state
class RDY_OV : public LinkedQueue<Order*>
{
public:
    RDY_OV() {}

    // Function to search and remove a specific order by ID
    bool CancelOrder(int id , Order* &cancelledOrder)
    {
		LinkedQueue<Order*> tempQueue;
		Order* tempOrder;
		bool found = false;
		while (this->dequeue(tempOrder)) {
			if (tempOrder->getID() == id) {
				found = true;
				cancelledOrder = tempOrder;
			} else {
				tempQueue.enqueue(tempOrder);
			}
		}

		while (tempQueue.dequeue(tempOrder)) {
			this->enqueue(tempOrder);
		}

		return found;
    }
};