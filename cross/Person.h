#ifndef PERSON_H
#define PERSON_H

#include <vector>
#include <stdexcept>
#include <list>
#include <time.h>

#include "Player.h"
#include "Game.h"

using namespace std;

class Person: public Player
{
public:
	Person (char simbol);

	void set_game(Game* _game);
	int input();
	int move();

	virtual ~Person() {} //не реализован

private:	
	int _size_field;
	vector<char> _occupied_field;
};



Person::Person(char symbol):
	Player(symbol)
{}

void Person::set_game(Game* _game)
{
	_size_field = _game->_size_field;
	_occupied_field.resize(_size_field*_size_field);
	_occupied_field = _game->_occupied_field;
}

int Person::input()
{
	int inp[2];

	do {
		cout << "Enter your move: ";
		for (size_t i = 0; i < 2; i++)
				cin >> inp[i];
		cout << endl;
	} while (inp[0] > _size_field - 1 || inp[1] > _size_field - 1);

	int motion = inp[0]*_size_field+inp[1];
	return motion;
}

int Person::move()
{
	int inp = input();
	return inp;
}


#endif // PERSON_H
