#include "Restaurant.h"
#include "Order.h"

Restaurant::Restaurant()
{}

void Restaurant::AddtoPendingList(Order* pOrd)
{
	string type = pOrd->getType();
	if (type == "ODN") {

		PENDODN.enqueue(pOrd);

	}
	
	else if (type == "ODG") {
		
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



    }

void Restaurant::ExecuteActions(int currentTimestep)
{



}


Restaurant::~Restaurant();