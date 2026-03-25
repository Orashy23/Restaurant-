#pragma once
class Scooter
{
private:
	int ID;
	int speed;
	int maintananceTime;
	int maintanceOrder;
	int ordercounter;
public:
	Scooter(int Id, int speed, int maintananceTime, int mainOrder);
	int getID();
	int getSpeed();
	int getMaintananceTime();
	int getMaintananceOrder();
	int getOrderCounter();
	void setMaintananceOrder(int mainOrder);
	void setID(int Id);
	void setSpeed(int speed);
	void setMaintananceTime(int maintananceTime);
	void increaseOderCounter();
	void print();
	bool needsMaintenance();
	void AfterMaintenance();
	friend ostream& operator<<(ostream& out, const Scooter* s);
};

