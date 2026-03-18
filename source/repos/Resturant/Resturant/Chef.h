#pragma once
class Chef
{
private:
	int ID;
	bool is_special, is_free;
	int speed;

public:
	Chef(int Id, bool is_special, int speed);
	bool getIsSpecial();
	int getSpeed();
	int getID();
	bool getIsFree();
	void setIsFree(bool status);
	void setID(int Id);
	void setIsSpecial(bool is_special);
	void setSpeed(int speed);
	void print();
};

