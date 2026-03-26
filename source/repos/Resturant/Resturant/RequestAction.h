//#pragma once
//#include "Order.h"
//#include "Chef.h"
//#include "Scooter.h"
//#include "LinkedQueue.h"
//#include "priQueue.h"
//#include "Action.h"
//
//
//class RequestAction : public Action
//{
//private:
//// to be wridden from the file;
//	string type;
//	int ID;
//	int size;
//	int price;
//
////Dine in only
//	int duration;
//	bool canShare;
//	int seats;
//// Delevery only
//	int distance;
//public:
//	RequestAction(int t,string typ,int sz ,int prc, int dur, int dist , bool share, int seatC ,int id) : Action(t)
//	{
//
//		type = typ;
//		ID = id;
//		size = sz;
//		price = prc;
//		duration = dur;
//		canShare = share;
//		seats = seatC;
//		distance = dist;
//	}
//	void act(Restaurant* pRes);
//	bool isDineIn() { return type == "DineIn"; }
//	bool isTakeAway() { return type == "TakeAway"; }
//	void setType(string t) { type = t; }
//	void setDineInInfo(int dur, bool share, int seatC);
//	void setDistance(int d) { distance = d; }
//	~RequestAction() {};
//};
//
//
//// /// //// //// //// ////////// CPP //////////////////
//
//
//void RequestAction::act(Restaurant* pRes)
//{
//	Order* order = new Order(ID, type, price, size, getTime());
//	order->setDineInInfo(seats, duration, canShare);
//	order->setDistance(distance);
//	pRes->addOrder(order);
//	if(type == "DineIn")
//		pRes->addDineInOrder(order);
//	
//	else if(type == "TakeAway")
//		pRes->addTakeAwayOrder(order);
//
//
//}
//
//
//
//
//
//
//
// //  TO be completed by Gharbawy
