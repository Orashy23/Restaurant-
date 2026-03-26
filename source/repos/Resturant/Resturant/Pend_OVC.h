#pragma once
#include "LinkedQueue.h"
#include "Order.h"
class Pend_OVC : public LinkedQueue<Order*> 
{

public:

	bool Cancel_Order( int cancelledID , Order* &cancelledOrder ) {

		LinkedQueue<Order*> tempQueue;
		Order* tempOrder;
		bool found = false;
		
		while (this->dequeue(tempOrder)) {
		
			if (tempOrder->getID() == cancelledID) {

				found = true;
				tempOrder = cancelledOrder;
			}
			else {

				tempQueue.enqueue(tempOrder);

			}
		}

		while (tempQueue.dequeue(tempOrder)) {

			this->enqueue(tempOrder);
		}


		return found;
	}

};