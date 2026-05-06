#pragma once
#include <iostream>
using namespace std;
class Chef
{
private:
	int ID ;
	bool is_special, is_free;
	int speed;
    int totalBusyTime;

public:
	Chef(int Id, bool is_special, int speed);
	bool getIsSpecial();
	int getSpeed();
	int getID() const;
	bool getIsFree();
	void setIsFree(bool status);
	void setID(int Id);
	void setIsSpecial(bool is_special);
	void setSpeed(int speed);
	void print();
	int getTotalBusyTime() const { return totalBusyTime; }
	void setTotalBusyTime(int time) { totalBusyTime = time; }
	friend ostream& operator<<(ostream& out, const Chef* c);
};

