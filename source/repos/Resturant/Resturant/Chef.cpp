#include "Chef.h"
#include<iostream>
using namespace std;
#include <string>
Chef::Chef(int Id, bool is_special, int speed)
{
	this->ID = Id;
	this->is_special = is_special;
	this->speed = speed;
	this->is_free = true;
}
bool Chef::getIsSpecial()
{
	return is_special;
}
int Chef::getSpeed()
{
	return speed;
}
int Chef::getID()
{
	return ID;
}
bool Chef::getIsFree()
{
	return is_free;
}
void Chef::setIsFree(bool status)
{
	is_free = status;
}


void Chef::print()
{
	cout << "Chef: " << (is_special? "Special" : "Normal") << ", Speed: " << speed 
	<< " Status: " << (is_free ? "Free" : "Busy") << endl;
}

ostream& operator<<(ostream& out, const Chef* c)
{
out << "Chef: " << (c->is_special ? "Special" : "Normal") << ", Speed: " << c->speed
<< " Status: " << (c->is_free ? "Free" : "Busy") << endl;
return out;
}
