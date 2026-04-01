#pragma once
#include <LinkedQueue.h>
#include <priQueue.h>
#include <Stack.h>


class Order;
class Chef;
class Table;
class Scooter;
class Action;

class Restaurant
{
	////////////// Orrders /////////////

	LinkedQueue<Action*> ActionQueue; //Queue of all the actions that will be read from the file
	LinkedQueue<Order*> Pend_ODG; //Queue of pending grilled orders 
	LinkedQueue<Order*> Pend_ODN; //Queue of pending Normal orders
	LinkedQueue<Order*> Pend_OT; //Queue of pending Take_Away orders
	LinkedQueue<Order*> Pend_OVN; //Queue of pending Normal delivery orders
	priQueue<Order*> Pend_OVG; //Queue of pending Grilled delivery orders
	//LinkedQueueCOLD<Order*>Pend_ OVC; //Queue of pending Cold delivery orders //TO DO !!!

	/////////////// Chefs /////////////
	LinkedQueue<Chef*> Free_CS; //Queue of available special chefs
	LinkedQueue<Chef*> Free_CN; //Queue of available normal chefs

	/////////////// SPECIAL DATA ////////////
	LinkedQueue<Order*> Cancelled_Orders; //Queue of cancelled orders
	Stack<Order*> Finished_Orders; //Queue of finished orders




	/////////////// Tables /////////////





};