#pragma once
#include <iostream>
#include "Restaurant.h"
using namespace std;

class Action
{
	private:
		int time;
public:
	Action(int t) { time = t; };
	virtual void act(Restaurant* pRes) = 0; //pure virtual function makes this class abstract
	int getTime() {return time;} 
	~Action() {};
};

// test


