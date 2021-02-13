#ifndef GAME_H
#define GAME_H

#include "IDictionary.h"
#include "Player.h"

#include <vector>
#include <string.h>

using namespace std;
static enum { PLAYING, OWIN, XWIN, DRAW } state;

class Game
{
public:
	Game(int size_field, Player* player1, Player* player2);

	void display_board();
	void get_move();												 //кто ходит в текущий момент
	pair<size_t, size_t> maximum_length (char symbol, vector<char> occupied_field);
	void check_game_state(char symbol, vector<char> occupied_field); //проверка состояния игры

private:
	Player* _player1,
		  * _player2;

public:
	size_t _cplayer_num;
	int _size_field;
	vector<char> _occupied_field;
};

Game::Game(int size_field, Player* player1, Player* player2) :
	_player1(player1),
	_player2(player2),
	_cplayer_num(1),
	_size_field(size_field)
{
	_occupied_field.resize(size_t(_size_field*_size_field));
	for (int i = 0; i < _size_field*_size_field; i++)
		_occupied_field[size_t(i)] = 0;

	state = PLAYING;

	_player1->set_game(this);
	_player2->set_game(this);
}

void Game::display_board()
{
	cout << endl;
	if (_size_field < 16) {				//если больше, то просто вывод номера клетки компьютера
		for (int i = 0; i < _size_field; i++)
		{
			for (int k = 0; k < _size_field; k++)
			{
				if (k > 0)
					cout << " | ";
				if (k == 0)
					cout << " ";

				cout << _occupied_field.at(size_t(i*_size_field+k));
				cout.flush();

			}
			cout << endl;
			if (i < _size_field - 1)
			{
                for (size_t k = 0; k < 3.5*_size_field+1; k++)
					cout << "-";
				cout << endl;
			}
		}
	}
	cout << endl;
}

void Game::get_move()
{
	if (_cplayer_num == 1)
	{
		size_t i;
		_player1->set_game(this);
		do {
			i = size_t(_player1->move());
		} while (_occupied_field[i] != 0);

		_occupied_field[i] = _player1->show_symbol();
		_cplayer_num = 2;
		check_game_state(_player1->show_symbol(), _occupied_field);
	}
	else
	{
		size_t i;
		_player2->set_game(this);
		do {
			i = size_t(_player2->move());
		} while (_occupied_field[i] != 0);

		_occupied_field[i] = _player2->show_symbol();
		_cplayer_num = 1;
		check_game_state(_player2->show_symbol(), _occupied_field);
	}
	display_board();

	if (state == OWIN)
		cout << "winner is 'O'";
	if (state == XWIN)
		cout << "winner is 'X'";
	if (state == DRAW)
		cout << "draw";
	while (state == PLAYING)
		get_move();
}

pair<size_t, size_t> Game::maximum_length(char symbol, vector<char> occupied_field)
{
	size_t maxX = 0,
		   maxO = 0;

	size_t ***count = new size_t**[size_t(_size_field)];
	for (size_t i = 0; i < size_t(_size_field); i++) {
		count[i] = new size_t*[size_t(_size_field)];
		for (size_t j = 0; j < size_t(_size_field); j++) {
			count[i][j] = new size_t[4];
			for (size_t k = 0; k < 4; k++)
				count[i][j][k] = 0;
		}
	}

	for (size_t i = 0; i < size_t(_size_field); i++)
	{
		for (size_t j = 0; j < size_t(_size_field); j++)
		{
			if (occupied_field[i*size_t(_size_field)+j] == symbol)
			{
				for (size_t k = 0; k < 4; k++)
					count[i][j][k] = 1;

				if (j != 0)
					count[i][j][0] += count[i][j-1][0];

				if (i != 0)
					count[i][j][1] += count[i-1][j][1];

				if (i != 0 && j != 0)
					count[i][j][2] += count[i-1][j-1][2];

				if (i != 0 && j != size_t(_size_field)-1)
					count[i][j][3] += count[i-1][j+1][3];
			}

			for (size_t k = 0; k < 4; k++)
			{
				if (count[i][j][k] > maxX && occupied_field[i*size_t(_size_field)+j] == 'X')
					maxX = count[i][j][k];
				else if (count[i][j][k] > maxO && occupied_field[i*size_t(_size_field)+j] == 'O')
					maxO = count[i][j][k];
			}
		}
	}

	for (size_t i = 0; i < size_t(_size_field); i++)
	{
		for (size_t j = 0; j < size_t(_size_field); j++)
			delete[]count[i][j];
		delete[]count[i];
	}
	delete[]count;

	return {maxX, maxO};
}

void Game::check_game_state(char symbol, vector<char> occupied_field)
{	
	state = PLAYING;
	auto max = maximum_length(symbol, occupied_field);

	size_t coun = 0;
	for (int i = 0; i < _size_field; i++)
		for (int j = 0; j < _size_field; j++)
			if (occupied_field[size_t(i*_size_field+j)] == 0)
				coun++;

	for (size_t k = 0; k < 4; k++)
	{
		size_t cons;
		if (_size_field > 3)
			cons = 4;
		else cons = 3;

		if (max.first == cons)
			state = XWIN;
		if (max.second == cons)
			state = OWIN;
	}

	if (coun == 0 && state != XWIN && state != OWIN)
		state = DRAW;
}

#endif // GAME_H

