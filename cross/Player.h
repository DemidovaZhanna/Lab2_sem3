#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

using namespace std;

class Game;

class Player
{
public:
	Player(char symbol) : _symbol(symbol) {}
	virtual int move() = 0;
	virtual void set_game(Game* game) = 0;
	virtual char show_symbol() {return _symbol;}

private:
	char _symbol;
};

#endif // PLAYER_H
