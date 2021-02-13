#ifndef COMPUTER_H
#define COMPUTER_H

#include <vector>
#include <stdexcept>
#include <list>
#include <time.h>

#include "Player.h"
#include "Game.h"

static const int INF = 100;

class Computer: public Player
{
public:
	Computer(char symbol);
	void set_game(Game* game);
	int move ();

	int MiniMax();
	int MinMove();
	int MaxMove();

	int evaluate_position();
	int current_estimate();
	void generate_moves(list<size_t> &move_list);	//генерация всех возможных ходов
	void adding_to_list(list<size_t> &move_list, int xx, int yy);
	virtual ~Computer() {} //не реализован

private:
	char sign;
	int _size_field;
	vector<char> _occupied_field;
	size_t num;
	Game* _game;
};

Computer::Computer(char symbol) :
	Player(symbol)
{
	num = 0;
}

void Computer::set_game(Game* game)
{
	_game = game;
	_size_field = _game->_size_field;
	_occupied_field.resize(size_t(_size_field*_size_field));
	_occupied_field = _game->_occupied_field;
}

int Computer::move()
{
	return MiniMax();
}

int Computer::MiniMax()
{
	int best_val = -INF;
	int index = 0;
	list<size_t> move_list;
	int *best_moves = new int[size_t(_size_field*_size_field)];
	generate_moves(move_list);

	while (!move_list.empty())
	{
		_occupied_field[move_list.front()] = this->show_symbol();
		sign = this->show_symbol();

		int val = MinMove();
		if (val > best_val)
		{
			best_val = val;
			index = 0;
			best_moves[index] = 1 + int(move_list.front());
		}
		else if (val == best_val)
			best_moves[++index] = 1 + int(move_list.front());
		//cout << "\nminimax: " << 1 + move_list.front() << "(" << val << ")";
		_occupied_field[move_list.front()] = 0;
		move_list.pop_front();
	}
	srand((unsigned int)time(nullptr));
	if (index > 0)
		index = rand() % index;
	return best_moves[index] - 1;
}

int Computer::MinMove()
{
	int pos_value = evaluate_position();
	if (pos_value != -1)
		return pos_value;

	size_t depth = 0;
	if (_size_field == 3)
		depth = 9;
	if (_size_field == 4)
		depth = 4;
	if (_size_field > 4)
		depth = 3;

	if (num == depth) {
		int valid = 0;
		auto max = _game->maximum_length(this->show_symbol(), _occupied_field);
		if (this->show_symbol() == 'X')
			valid = max.first - max.second;
		if (this->show_symbol() == 'O')
			valid = max.second - max.first;
		return valid;
	}

	num++;
	int best_val = +INF;
	list<size_t> move_list;
	generate_moves(move_list);

	while (!move_list.empty())
	{
		this->show_symbol() == 'X'? sign = 'O' : sign = 'X';
		_occupied_field[move_list.front()] = sign;
		int val = MaxMove();

		if (val < best_val)
			best_val = val;

		_occupied_field[move_list.front()] = 0;
		move_list.pop_front();
	}
	num--;
	return best_val;
}

int Computer::MaxMove()
{
	int pos_value = evaluate_position();
	if (pos_value != -1)
		return pos_value;

	size_t depth = 0;
	if (_size_field == 3)
		depth = 9;
	if (_size_field == 4)
		depth = 4;
	if (_size_field > 4)
		depth = 3;

	if (num == depth) {
		int valid = 0;
		auto max = _game->maximum_length(this->show_symbol(), _occupied_field);
		if (this->show_symbol() == 'X')
			valid = max.first - max.second;
		if (this->show_symbol() == 'O')
			valid = max.second - max.first;
		return valid;
	}

	num++;
	int best_val = -INF;

	list<size_t> move_list;
	generate_moves(move_list);

	while (!move_list.empty())
	{
		this->show_symbol() == 'X'? sign = 'X' : sign = 'O';
		_occupied_field[move_list.front()] = sign;

		int val = MinMove();

		if (val > best_val)
			best_val = val;

		_occupied_field[move_list.front()] = 0;
		move_list.pop_front();
	}
	num--;
	return best_val;
}

int Computer::evaluate_position()
{
	_game->check_game_state(sign, _occupied_field);
	if ((state == XWIN || state == OWIN || state == DRAW))
	{
		if ((state == XWIN && this->show_symbol() == 'X') || (state == OWIN && this->show_symbol() == 'O'))
			return INF;
		else
			if ((state == XWIN && this->show_symbol() == 'O') || (state == OWIN && this->show_symbol() == 'X'))
				return -INF;
		else
			if (state == DRAW) return 0;
	}
	return -1;
}


void Computer::generate_moves(list<size_t> &move_list)
{
	int x = 0,
		y = 0,
		count = 0,
		num = 0;

	for (int i = 0; i < _size_field; i++)
		for (int j = 0; j < _size_field; j++)
			if (_occupied_field[size_t(i*_size_field+j)] != 0)
			{
				x += i;
				y += j;
				count++;
			}
			else if (_occupied_field[size_t(i*_size_field+j)] == 0)
				num++;
	if (num == int(_size_field*_size_field))
		count = 1;
	x /= count;
	y /= count;

	int shiftI = min(x, _size_field - x),
		shiftJ = min(y, _size_field - y),
		shift = min(shiftI, shiftJ);


	if (_occupied_field[size_t(x*_size_field+y)] == 0)
		move_list.push_back(size_t(x*_size_field+y));


	for (int r = 1; r <= _size_field-shift; r++)
	{
		int xx = x-r, yy = y+r;

		for(; xx < x+r; ++xx)
			adding_to_list(move_list, xx, yy);

		for (; yy > y-r; --yy)
			adding_to_list(move_list, xx, yy);

		for (xx = x+r; xx > x - r; --xx)
			adding_to_list(move_list, xx, yy);

		for (yy = y - r; yy < y + r; ++yy)
			adding_to_list(move_list, xx, yy);
	}
}

void Computer::adding_to_list(list<size_t> &move_list, int xx, int yy)
{
	if (xx >= 0 && yy >= 0 && xx <_size_field && yy < _size_field && _occupied_field[size_t(xx*_size_field+yy)] == 0)
		move_list.push_back(size_t(xx*_size_field + yy));
}


#endif // COMPUTER_H
