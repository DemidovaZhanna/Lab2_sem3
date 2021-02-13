#ifndef INTERFACE_H
#define INTERFACE_H

#include "Person.h"
#include "Computer.h"

void interface()
{
	int size_field = 0;

	size_t choise = 0;
	char symbol = '\0';

	Person person('O');
	Computer comp('X');

	Person person2('X');
	Computer comp2('O');

	while(100) {

		cout << "Select field size(3 - 5): ";
		cin >> size_field;

		while (choise == 0 || choise > 4)
		{
			cout << "Select the game type: 1. computer-computer, 2. computer-person, 3. person-computer, 4. person-person ";
			cin >> choise;
		}

		if (choise == 2 || choise == 3)
		{
			do {
				cout << "Select symbol: X or O: ";
				cin >> symbol;
			} while (symbol != 'X' && symbol != 'O');
		}

		Game *game = nullptr;

		if (choise == 1)
			game = new Game(size_field, &comp, &comp2);
		if (choise == 2 && symbol == 'X')
			game = new Game(size_field, &comp2, &person2);
		if (choise == 2 && symbol == 'O')
			game = new Game(size_field, &comp, &person);
		if (choise == 3 && symbol == 'X')
			game = new Game(size_field, &person2, &comp2);
		if (choise == 3 && symbol == 'O')
			game = new Game(size_field, &person, &comp);
		if (choise == 4)
			game = new Game(size_field, &person, &person2);

		game->display_board();
		game->get_move();

		choise = 0;
		cout << "\n\n";
	}
}

#endif // INTERFACE_H
