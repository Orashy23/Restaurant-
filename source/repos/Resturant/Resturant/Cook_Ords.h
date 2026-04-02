#pragma once
#include "priQueue.h"
#include "Order.h"

// Derived from PriQueue to support CancelOrder(ID) during cooking
class Cook_Ords : public priQueue<Order*>
{
public:
    Cook_Ords() {}

    // Function to search and remove a specific order by ID
    bool CancelOrder(int id , Order* &cancelledOrder)
    {
        priQueue<Order*> tempQueue;
        Order* tempOrder;
        int pri;
        bool found = false;
        while (this->dequeue(tempOrder,pri)) {
            if (tempOrder->getID() == id) {
                found = true;
                cancelledOrder = tempOrder;
            } else {
                tempQueue.enqueue(tempOrder, tempOrder->getPriority());
            }
        }

        while (tempQueue.dequeue(tempOrder,pri)) {
            this->enqueue(tempOrder, tempOrder->getPriority());
        }

        return found;
    }
};