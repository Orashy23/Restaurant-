#pragma once
#include "Action.h"

class CancelAction : public Action
{
	int orderID; // ID of the order to be cancelled
public:
    CancelAction(int time, int id)
        : Action(time), orderID(id) {
    }

    virtual void act(Restaurant* pRes)
    {
        pRes->CancelOrder(orderID);
    }

    virtual void print() const
    {
        cout << "(X, " << getTime() << ", " << orderID << ")";
    }
};

