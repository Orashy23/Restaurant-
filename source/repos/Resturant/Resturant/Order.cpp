#include "Order.h"
#include<iostream>
using namespace std;
#include <string>
Order::Order(int id, string type, int price, int size, int timerequest)
{
	this->ID = id;
	this->type = type;
	this->price = price;
	this->ordersize = size;
	this->TQ = timerequest;
	this->priority = 0;
	this->TA = 0;
	this->TR = 0;
	this->TS = 0;
	this->TF = 0;
	this->assignedScooter = nullptr;
	this->assignedTable = nullptr;

	this->numChefsNeeded = 1;
	this->numScootersNeeded = 1;
	this->numChefsAssigned = 0;
	this->numScootersAssigned = 0;

	for (int i = 0; i < 4; i++) {
		assignedChefs[i] = nullptr;
	}
	for (int i = 0; i < 20; i++) {
		assignedScooters[i] = nullptr;
	}

}

int Order::getID()
{
	return ID;
}
int Order::getTQ()
{
	return TQ;
}
int Order::getSize()
{
	return ordersize;
}
int Order::getPrice()
{
	return price;
}
int Order::getPriority()
{
	return priority;
}
string Order::getType()
{
	return type;
}
int Order::getSeats()
{
	return seats;
}
int Order::getDuration()
{
	return duration;
}
bool Order::getCanShare()
{
	return canShare;
}
void Order::setTA(int t)
{
	TA = t;
}
void Order::setTR(int t)
{
	TR = t;
}
void Order::setTS(int t)
{
	TS = t;
}
void Order::setTF(int t)
{
	TF = t;
}
void Order::setDineInInfo(int seats, int duration, bool canShare)
{
	this->seats = seats;
	this->duration = duration;
	this->canShare = canShare;
}
void Order::setDistance(int d)
{
	this->distance = d;
}
int Order::getDistance()
{
	return distance;
}
void Order::setPriority(int p)
{
	this->priority = p;
}
void Order::print()
{
	cout << "ID: " << ID << " Type: " << type << " Price: " << price << " Size: " << ordersize;

	if (type == "ODG" || type == "ODN") // dine in
	{
		cout << " Seats: " << seats << " Duration: " << duration;
	}
	else if (type == "OVC" || type == "OVG" || type == "OVN")  // delivery 
	{
		cout << " Distance: " << distance;
	}
	cout << endl;
}

ostream& operator<<(ostream& os, Order* order)
{
	if (order) {
		os << "ID: " << order->ID << " Type: " << order->type << " Price: " << order->price << " Size: " << order->ordersize;

		if (order->type == "ODG" || order->type == "ODN") // dine in
		{
			os << " Seats: " << order->seats << " Duration: " << order->duration;
		}
		else if (order->type == "OVC" || order->type == "OVG" || order->type == "OVN")  // delivery 
		{
			os << " Distance: " << order->distance;
		}
		os << endl;
	}
	return os;
}

int Order::getTA() const { return TA; }
int Order::getTR() const { return TR; }
int Order::getTS() const { return TS; }
int Order::getTF() const { return TF; }

void    Order::setScooter(Scooter* s) { assignedScooter = s; }
Scooter* Order::getScooter() { return assignedScooter; }
void    Order::setTable(Table* t) { assignedTable = t; }
Table* Order::getTable() { return assignedTable; }
