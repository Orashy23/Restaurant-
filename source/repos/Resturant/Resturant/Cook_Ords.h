#pragma once
#include "priQueue.h"
#include "Order.h"

// Derived from PriQueue to support CancelOrder(ID) during cooking
class Cook_Ords : public priQueue<Order*>
{
public:
    Cook_Ords() {}

    // Function to search and remove a specific order by ID
    bool CancelOrder(int id)
    {
        // TO DO: Implement search and removal logic using priority nodes
        return false;
    }
};