#pragma once
#include <string>
#include <iostream>
using namespace std;

class Scooter;
class Table;

class Chef; // Forward declaration

class Order
{
	

private:
	int ordersize, price, seats, ID, duration, distance;
	int priority;
	string type;
	int TQ, TA, TR, TS, TF;
	bool canShare;
	Scooter* assignedScooter;
	Table* assignedTable;
	Chef* assignedChef; //this is the chef that will prepare the order once finished , it will be set to nullptr;
	/* this T* are used to track the time taken through each step of the order cycle 
	 (document page 4 of the project description for visualization) */
public:
	Order(int id, string type, int price, int size, int timerequest);
	int getID();
	int getTQ();
	int getSize();
	int getPrice();
	int getPriority();
	string getType();

	void setTA(int t);
	void setTR(int t);
	void setTS(int t);
	void setTF(int t);
	void setPriority(int p);

	int getTA() const;
	int getTR() const;
	int getTS() const;
	int getTF() const;

	void setScooter(Scooter* s);
	Scooter* getScooter();
	void setTable(Table* t);
	Table* getTable();

	void setDineInInfo(int seats, int duration, bool canShare);
	int getSeats();
	int getDuration();
	bool getCanShare();

	void setDistance(int d);
	int getDistance();

	void print();

	void setChef(Chef* c) { assignedChef = c; }
	Chef* getChef() { return assignedChef; }

	friend ostream& operator<<(ostream& os, Order* order);

};


