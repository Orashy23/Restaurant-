#pragma once
#include "priQueue.h"
#include "Order.h"

// Derived from PriQueue to support CancelOrder(ID) during cooking
class Cook_Ords : public priQueue<Order*>
{
public:
    Cook_Ords() {}

    // Function to search and remove a specific order by ID
   // Search and remove a specific order by ID while it is being cooked
    bool CancelOrder(int id, Order*& cancelledOrder) {
        priQueue<Order*> tempQueue;
        Order* tempOrder;
        int pri;
        bool found = false;

        // Use a temporary queue to find the targeted ID
        while (this->dequeue(tempOrder, pri)) {
            if (tempOrder->getID() == id) {
                found = true;
                cancelledOrder = tempOrder;
                // Note: We don't break here to ensure we process the whole queue if needed,
                // though IDs are unique so we could break for optimization.
            }
            else {
                tempQueue.enqueue(tempOrder, pri);
            }
        }

        // Restore the cooking queue without the cancelled order
        while (tempQueue.dequeue(tempOrder, pri)) {
            this->enqueue(tempOrder, pri);
        }
        return found;
    }
};