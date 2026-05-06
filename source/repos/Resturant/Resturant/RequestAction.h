#pragma once
#include "Action.h"
#include "Restaurant.h"
#include "Order.h"
#include <string>

class RequestAction : public Action
{
private:
    int ID;
    string type;
    int size;
    int price;
//dine in
    int seats, duration;
    bool canShare;

 //delivery
    int distance;

    int numChefs;     // Combo
    int numScooters;  // Combo

public:
    
    RequestAction(int time, string typ, int id, int sz, int prc, int sts, int dur, bool shr, int dist, int nChefs = 1, int nScooters = 1)
        : Action(time), type(typ), ID(id), size(sz), price(prc), seats(sts), duration(dur), canShare(shr), distance(dist), numChefs(nChefs), numScooters(nScooters)
    {
    }

  
    virtual void act(Restaurant* pRes) 
    {
        //new order
        Order* pOrd = new Order(ID, type, price, size, getTime());

        
        if (type == "ODG" || type == "ODN")
        {
            pOrd->setDineInInfo(seats, duration, canShare);
        }
        else if (type == "OVC" || type == "OVG" || type == "OVN")
        {
            pOrd->setDistance(distance);
        }
//resturant control.
        pRes->AddtoPendingList(pOrd);
     // COMBO
        if (type == "COMBO") {
            pOrd->setDistance(distance);
            pOrd->setNumChefsNeeded(numChefs);
            pOrd->setNumScootersNeeded(numScooters);
        }
    }

    virtual void print() const
    {
        cout << "[" << type << ", " << getTime() << ", " << ID << "]";
    }

    virtual ~RequestAction() {}
};