#include "Table.h"
#include<iostream>
using namespace std;
#include <string>
Table::Table(int Id, int capacity)
{
	this->ID = Id;
	this->capacity = capacity;
	this->freeSeats = capacity;
}
int Table::getID()
{
	return ID;
}
int Table::getCapacity()
{
	return capacity;
}	
int Table::getFreeSeats()
{
	return freeSeats;
}

void Table::print()
{
	cout << "Table ID: " << ID
		<< " Capacity: " << capacity
		<< " Free Seats: " << freeSeats << endl;
}

ostream& operator<<(ostream& os, Table* table)
{
	if (table) {
		os << "Table ID: " << table->ID
			<< " Capacity: " << table->capacity
			<< " Free Seats: " << table->freeSeats << endl;
	}
	return os;
}