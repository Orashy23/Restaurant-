#pragma once
class Table
{
private:
	int ID;
	int capacity;
	int freeSeats;
public:
	Table(int Id, int capacity);
	int getID();
	int getCapacity();
	int getFreeSeats();
	void setID(int Id);
	void setCapacity(int capacity);
	void setFreeSeats(int freeSeats);
	void print();
	friend ostream& operator<<(ostream& os, Order* order);

};


