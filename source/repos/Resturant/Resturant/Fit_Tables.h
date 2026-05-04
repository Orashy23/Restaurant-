#pragma once
#include "priQueue.h"
#include "Table.h"
#include <iostream>

class Fit_Tables : public priQueue<Table*>
{
public:
    Fit_Tables() {}

    // Adds a table to the queue using negative free seats so the tightest fit is at the front
    void addTable(Table* t) {
        enqueue(t, -(t->getFreeSeats()));
    }

    // Dequeues tables until it finds the tightest fit, then stops searching
    Table* getBest(int requiredSeats) {
        priQueue<Table*> temp;
        Table* best = nullptr;
        Table* t;
        int p;

        while (dequeue(t, p)) {
            // Because of the negative priority trick, the first one that fits IS the best!
            if (t->getFreeSeats() >= requiredSeats) {
                best = t;
                break; // OPTIMIZATION: Stop searching immediately! 
            }
            else {
                // Table is too small. Put it in temp to get it out of the way.
                temp.enqueue(t, p);
            }
        }

        // Put the smaller tables we skipped over back into the main queue
        while (temp.dequeue(t, p)) {
            enqueue(t, p);
        }

        return best;
    }

    // Prints the current state of all tables in this queue
    void print() const {
        priNode<Table*>* current = head;
        while (current != nullptr) {
            int p;
            Table* t = current->getItem(p);
            std::cout << "[T" << t->getID() << ", " << t->getCapacity() << ", " << t->getFreeSeats() << "] ";
            current = current->getNext();
        }
        std::cout << std::endl;
    }

    // Returns the total number of tables currently in this queue
    int getcount() const {
        priNode<Table*>* current = head;
        int count = 0;
        while (current) {
            count++;
            current = current->getNext();
        }
        return count;
    }
};