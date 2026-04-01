#pragma once
#include "priQueue.h"
#include "Table.h"

// Derived class to support best-fit selection
class Fit_Tables : public priQueue<Table*>
{
public:
    Fit_Tables() {}

    // Returns the best table that fits the required seats
    Table* getBest(int requiredSeats)
    {
        // TO DO: Implement best-fit dequeue logic
        return nullptr;
    }
};