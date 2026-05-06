#pragma once
#include <iostream>
using namespace std;
class Scooter
{
private:
	int ID;
	int speed;
	int maintananceTime;
	int maintanceOrder;
	int ordercounter;
	int totalDistance;       // Tracks cumulative distance to prioritize shortest-traveled scooters
	int tripsSinceMaint;     // Tracks how many deliveries it has done since last maintenance
	int returnTime;          // The timestep when this scooter arrives back at the restaurant
	int maintFinishTime;     // The timestep when it leaves the mechanic
	int totalBusyTime;
	int failureprob;
	bool isbroken = false;



public:
	Scooter(int Id, int speed, int maintananceTime, int mainOrder);
	int getID();
	int getSpeed();
	int getMaintananceTime();
	int getMaintananceOrder();
	int getOrderCounter();

	int getTotalDistance();
	int getReturnTime();
	int getMaintFinishTime();

	void setMaintananceOrder(int mainOrder);
	void setID(int Id);
	void setSpeed(int speed);
	void setMaintananceTime(int maintananceTime);

	void setTotalDistance(int distance);
	void setReturnTime(int time);
	void setMaintFinishTime(int time);

	void increaseOderCounter();
	void addDistance(int dist);
	void incrementTrips();
	void print();
	bool needsMaintenance();
	bool checkfailure();
	void AfterMaintenance();

	int getTotalBusyTime() const { return totalBusyTime; }
	void setTotalBusyTime(int time) { totalBusyTime = time; }

	

	friend ostream& operator<<(ostream& out, const Scooter* s);
};

