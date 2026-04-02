#pragma once
#include "priQueue.h"
#include "Table.h"


class Fit_Tables : public priQueue<Table*>
{
public:
    Fit_Tables() {}
    void addTable(Table* t) { 
        enqueue(t, -t->getFreeSeats());


    }
    Table* getBest(int requiredSeats) {
        priQueue<Table*> temp;
        Table* best = nullptr;
        Table* t;
        int p;

        while (dequeue(t, p)) {
            if (t->getFreeSeats() >= requiredSeats && best == nullptr) {
                best = t;
            }
            else {
                temp.enqueue(t, p);
            }
        }

        while (temp.dequeue(t, p)) {
            enqueue(t, p);
        }
		return best;
	}


    void print() const {
        priNode<Table*>* current = head;
        while (current != nullptr) {
            int p;
            Table* t = current->getItem(p);
            cout << "[T" << t->getID() << ", "<< t->getCapacity() << ", "<< t->getFreeSeats() << "] ";
            current = current->getNext();
        }
        cout << endl;
    }
    }
    int getcount() const { 
        priNode<Table*>* current = head;
		int count = 0;
        while(current) {
            count++;
            current = current->getNext();
		}
		return count;
    }

    
    
};