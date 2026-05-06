#include "Scooter.h"
#include<iostream>
using namespace std;
#include <string>
#include "Chef.h"



Scooter::Scooter(int Id, int speed, int maintananceTime, int mainorder)
{
	this->ID = Id;
	this->speed = speed;
	this->maintananceTime = maintananceTime;
	this->maintanceOrder = mainorder;
	this->ordercounter = 0;
	this->tripsSinceMaint = 0;
	this->totalBusyTime = 0;
	totalDistance = 0;
	returnTime = 0;
	maintFinishTime = 0;
}

int Scooter::getID()
{
	return ID;
}
int Scooter::getSpeed()
{
	return speed;
}
int Scooter::getMaintananceTime()
{
	return maintananceTime;
}
int Scooter::getMaintananceOrder()
{
	return maintanceOrder;
}
int Scooter::getOrderCounter()
{
	return ordercounter;
}
void Scooter::setID(int Id)
{
	this->ID = Id;
}

void Scooter::setSpeed(int speed)
{
	this->speed = speed;
}


void Scooter::setMaintananceTime(int maintananceTime)
{
	this->maintananceTime = maintananceTime;
}
void Scooter::setMaintananceOrder(int mainOrder)
{
	this->maintanceOrder = mainOrder;
}
void Scooter::increaseOderCounter()
{
	ordercounter++;
}
bool Scooter::needsMaintenance()
{
	if (ordercounter >= maintanceOrder)
		return true;
	else { return false; }
}
void Scooter::AfterMaintenance()
{
	ordercounter = 0;
}
void Scooter::print()
{
	cout << "Scooter ID: " << ID
		<< " Speed: " << speed
		<< " Orders Served: " << ordercounter;
}

bool Scooter::checkfailure() {
	int r = rand() % 100;
	return r < breakdownprobability;
}

int Scooter::getTotalDistance()
{
	return totalDistance;
}

void Scooter::setTotalDistance(int dist) {
	totalDistance = dist;
}

void Scooter::addDistance(int dist) {
	totalDistance += dist;
}



void Scooter::incrementTrips() {
	tripsSinceMaint++;
}

int Scooter::getReturnTime() {
	return returnTime;
}

void Scooter::setReturnTime(int time) {
	returnTime = time;
}

int Scooter::getMaintFinishTime() {
	return maintFinishTime;
}

void Scooter::setMaintFinishTime(int time) {
	maintFinishTime = time;
}


ostream& operator<<(ostream& out, const Scooter* s)
{
	out << "S" << s->ID;
	return out;
}

