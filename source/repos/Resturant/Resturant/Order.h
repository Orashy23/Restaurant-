#pragma once
#include <string>
using namespace std;
class Order
{
private:
	int ordersize, price, seats, ID, duration, distance;
	int priority;
	string type;
	int TQ, TA, TR, TS, TF;
	bool canShare;
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

	void setDineInInfo(int seats, int duration, bool canShare);
	int getSeats();
	int getDuration();
	bool getCanShare();

	void setDistance(int d);
	int getDistance();

	void print();

	friend ostream& operator<<(ostream& os, Order* order);

};


