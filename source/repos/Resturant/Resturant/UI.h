#pragma once
#pragma once
#include <iostream>
#include <string>
using namespace std;

class Restaurant; // Forward declaration

class UI
{
public:
    UI();
    ~UI();

    // --- 1. Startup Functions ---
    // Reads the mode (Interactive or Silent) from the user
    int getMode();

    // Reads the file names from the user
    void getFileNames(string& inFile, string& outFile);

    // --- 2. Printing Functions ---
    // Prints simple messages (like "Simulation Starts in Silent mode")
    void printMsg(string msg);

    // Pauses and waits for user input ("PRESS ANY KEY TO MOVE TO NEXT STEP!")
    void waitForClick();

    // This is the main function that prints the huge Interactive Mode screen!
    // As per the PDF, you must pass the lists to this function so it can call their print() methods.
    // (Note: I am just passing the Restaurant pointer here so the UI can access all lists easily, 
    // but you can also pass the lists individually if your professor strictly prefers that).
    void printCurrentTimestep(int timestep, Restaurant* pRest);
};