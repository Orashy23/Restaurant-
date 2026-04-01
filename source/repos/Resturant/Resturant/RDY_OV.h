#pragma once
#include "LinkedQueue.h"
#include "Order.h"

// Derived from LinkedQueue to support CancelOrder(ID) in ready state
class RDY_OV : public LinkedQueue<Order*>
{
public:
    RDY_OV() {}

    // Function to search and remove a specific order by ID
    bool CancelOrder(int id)
    {
        // TO DO: Implement search and removal logic using queue nodes
        return false;
    }
};