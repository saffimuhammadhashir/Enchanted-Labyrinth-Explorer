#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>
#include<time.h>
#include <vector>
using namespace sf;
using namespace std;
class Enemy;
class EnemyList;
class Treeasurelist;
class Treasure;

class Maze {
	Texture walls_texture_horizontal;
	Texture walls_texture_vertical;
	RectangleShape** blocks;
	RectangleShape** foods;
	Color* playerColors;
	int verticalLineGap;
	int horizontalLineGap;

public:
	int n;
	int m;
	Sprite** walls_sprite;
	int** valid;
	int*** coords;

	Maze() {
		srand(time(0));
		n = 12;
		m = 23;
		blocks = new RectangleShape * [m];
		foods = new RectangleShape * [n];
		playerColors = new Color[4];
		playerColors[0] = Color::Red;
		playerColors[1] = Color::Blue;
		playerColors[2] = Color::Green;
		playerColors[3] = Color::Yellow;
		verticalLineGap = 1000 / 15;
		horizontalLineGap = 920 / 15;

		for (int i = 0; i < m; ++i) {
			blocks[i] = new RectangleShape[n];
			for (int j = 0; j < n; ++j) {
				RectangleShape block(Vector2f(verticalLineGap - 4, horizontalLineGap - 4));
				block.setPosition(Vector2f(i * verticalLineGap + 211, j * horizontalLineGap + 201));
				block.setFillColor(Color::Transparent);
				block.setOutlineColor(Color::Black);
				block.setOutlineThickness(1);
				blocks[i][j] = block;
			}
		}

		walls_sprite = new Sprite * [n + m];
		for (int i = 0; i < n; ++i) {
			walls_sprite[i] = new Sprite[n];
		}

		valid = new int* [m];
		for (int i = 0; i < m; ++i) {
			valid[i] = new int[n];
			for (int j = 0; j < n; ++j) {
				valid[i][j] = 1;
			}

		}
		int random = rand() %10;
		for (int i = 1; i < m - 1; i++)
		{
			int walls = n / 3;
			int j = 0;
			while (walls > 0)
			{
				if (rand()  % 10 == random && walls != 0) {
					valid[i][j] = 0;
					walls--;
				}
				j++;
				if (j == n) {
					j = 0;
				}
			}
		}





		coords = new int** [m];
		for (int i = 0; i < m; ++i) {
			coords[i] = new int* [n];
			for (int j = 0; j < n; ++j) {
				coords[i][j] = new int[2];
				coords[i][j][0] = i * verticalLineGap + 160;
				coords[i][j][1] = j * horizontalLineGap + 201;
			}
		}
	}

	void drawmaze(RenderWindow& window) {
		Texture bg_texture;
		bg_texture.loadFromFile("background.png");
		Sprite bg_sprite(bg_texture);
		bg_sprite.scale(2, 2);
		bg_sprite.setPosition(0, 200);

		Texture inventory_texture;
		inventory_texture.loadFromFile("inventory.png");
		Sprite inventory_sprite(inventory_texture);
		inventory_sprite.scale(5, 0.6);
		inventory_sprite.setPosition(0, 0);

		Texture highscore_texture;
		highscore_texture.loadFromFile("highscore.png");
		Sprite highscore_sprite(highscore_texture);
		highscore_sprite.scale(5, 0.64);
		highscore_sprite.setPosition(1300, 0);

		window.draw(bg_sprite);
		window.draw(inventory_sprite);
		window.draw(highscore_sprite);

		//for (int i = 0; i < m; ++i) {
		//	Vertex line[] = {
		//		Vertex(Vector2f(i * verticalLineGap + 150, 211)),
		//		Vertex(Vector2f(i * verticalLineGap + 150, 945)) };
		//	window.draw(line, 2, Lines);
		//}

		//for (int i = 0; i <= n; ++i) {
		//	Vertex line[] = {
		//		Vertex(Vector2f(150, i * horizontalLineGap + 211)),
		//		Vertex(Vector2f(1602, i * horizontalLineGap + 211)) };
		//	window.draw(line, 2, Lines);
		//}
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				if (valid[i][j] == 0) {
					RectangleShape square(Vector2f(verticalLineGap - 4, horizontalLineGap - 4));
					square.setPosition(Vector2f(i * verticalLineGap + 150, j * horizontalLineGap + 211));
					square.setFillColor(Color::Red); // Set color as per your preference
					window.draw(square);
				}
			}
		}

	}

	Sprite& getWallSprite(int i, int j) {
		return walls_sprite[i][j];
	}

	bool isValid(int i, int j) const {
		if (i >= m || j >= n) {
			return false;
		}
		return valid[i][j] != 0;
	}


};


class Fire {
public:
	Sprite sprite;
	Texture fire;

	int posX = 0;
	int posY = 0;
	int direction = 0;
	int moved = 0;
	int type;
	bool visible = true;
	Fire* next=nullptr;
	Fire* prev = nullptr;
	Fire(){}
	Fire(Maze& maze,int i,int j,int dir,int type,int subtype):type(type) {
		if (type == 1) {
			if(subtype==0)
			fire.loadFromFile("fire1.png");
			if (subtype == 1)
				fire.loadFromFile("newfire_rec1.png");
			if (subtype == 2)
				fire.loadFromFile("fire1.png");
			if (subtype == 3)
				fire.loadFromFile("fire1.png");
			if (subtype == 4)
				fire.loadFromFile("newfire_rec2.png");
			if (subtype == 5)
				fire.loadFromFile("newfire4.png");
			if (subtype == 6)
				fire.loadFromFile("newfire3.png");
			if (subtype == 7)
				fire.loadFromFile("newfire2.png");
			if (subtype == 8)
				fire.loadFromFile("newfire1.png");
		}
		if (type == 2)
			fire.loadFromFile("fire2.png");
		sprite.setTexture(fire);
		sprite.setPosition(maze.coords[i][j][0], maze.coords[i][j][1]+30);
		sprite.scale(0.1, 0.1);
		direction = dir;
	}
	void set_position(Maze& maze) {
		if (posX >= 0 && posX < 23 && posY >= 0 && posY < 12) {

		sprite.setPosition(maze.coords[posX][posY][0], maze.coords[posX][posY][1]+30);
		}
		else {
			visible = false;
		}
	}

	void moveUp(Maze& maze) {

			posY--;

			set_position(maze);
	}

	void moveDown(Maze& maze) {

			posY++;

			set_position(maze);
	}

	void moveLeft(Maze& maze) {

			posX--;
			set_position(maze);
	}

	void moveRight(Maze& maze) {

			posX++;
			set_position(maze);
	}
	void move(Maze& maze) {
		int val;
		if (type == 1) {
			val = 5;
		}
		else {
			val = 4;
		}
		if (moved <= val) {
			if (direction == 0) {
				moveUp(maze);
			}
			else if (direction == 1) {
				moveDown(maze);
			}
			else if (direction == 2) {
				moveLeft(maze);
			}
			else if (direction == 3) {
				moveRight(maze);
			}
			else {

			}
		}
		else {
			visible = false;


		}
		moved++;
	}


	void draw(RenderWindow& window) {
		if(visible==true)
		window.draw(sprite);
	}
};


class Player {
		Sprite sprite;
		Texture player;
	public:
		Fire* start=nullptr;
		int health = 500;
		int posX = 0;
		int posY = 0;
		Player(Maze& maze) {
			player.loadFromFile("PLAYER1.png");
			sprite.setTexture(player);
			sprite.setPosition(maze.coords[posX][posY][0], maze.coords[posX][posY][1]);
			sprite.scale(0.1, 0.1);
		}
		void set_position(Maze& maze) {
			sprite.setPosition(maze.coords[posX][posY][0], maze.coords[posX][posY][1]);



		}
		void create_fire(Maze& maze,int direction,int val) {
			Fire* create = new Fire(maze, posX, posY, direction,1,val); 
			create->posX = posX;
			create->posY = posY;

			if (start == nullptr) {
				start = create;
			}
			else {
				Fire* curr = start;
				while (curr->next != nullptr) {
					curr = curr->next;
				}
				curr->next = create;
				create->prev = curr;
			}
			create->move(maze); 
		}

		void updateFirePositions(Maze& maze) {
			Fire* curr = start;
			while (curr != nullptr) {
				curr->move(maze);

				if (curr->moved > 4) {

					if (curr == start) {
						start = curr->next;
						if (start)
							start->prev = nullptr;
					}
					else {
						if (curr->prev)
							curr->prev->next = curr->next;
						if (curr->next)
							curr->next->prev = curr->prev;
					}

					Fire* nextFire = curr->next;
					delete curr;
					curr = nextFire;
				}
				else {
					curr = curr->next;
				}
			}
		}


		void moveUp(Maze& maze) {
			if (posY > 0 && maze.isValid(posX, posY - 1)) {
				posY--;

				set_position(maze);
			}
		}

		void moveDown(Maze& maze) {
			if (posY < 35 && maze.isValid(posX, posY + 1)) {
				posY++;

				set_position(maze);
			}
		}

		void moveLeft(Maze& maze) {
			if (posX > 0 && maze.isValid(posX - 1, posY)) {
				player.loadFromFile("PLAYER4.png");
				sprite.setTexture(player);
				posX--;
				set_position(maze);
			}
		}

		void moveRight(Maze& maze) {
			if (posX < 80 && maze.isValid(posX + 1, posY)) {
				player.loadFromFile("PLAYER1.png");
				sprite.setTexture(player);
				posX++;
				set_position(maze);
			}
		}


		void draw(RenderWindow& window) {
			window.draw(sprite);
			Fire* curr = start;
			while (curr)
			{
				if (curr->visible)
				window.draw(curr->sprite);
				curr = curr->next;
			}
		}

	};

class Enemy {
		Sprite sprite;
		Texture enemy;
	public:
		bool visible = true;;
		int posX = 0;
		int posY = 0;
		int health=500;
		Fire* start=nullptr;
		Enemy* next = nullptr;
		Enemy() {}

		Enemy(Maze& maze) {

			srand(time(0));
			int val = rand() % 4;
			if (val == 0) {
				enemy.loadFromFile("demons1.png");

			}
			else  if (val == 1) {
				enemy.loadFromFile("demons2.png");
			}
			else  if (val == 2) {
				enemy.loadFromFile("demons3.png");
			}
			else {
				enemy.loadFromFile("demons4.png");
			}
			int random = rand() % 10;
			for (int j = 1; j < maze.n; ++j) {
				int i = 1;
				for (int i = 1; i < maze.m - 1; ++i) {
					if (rand() % 10 == random && maze.valid[i][j] != 0 && testing_row(i, j, maze)) {
						maze.valid[i][j] = 3;
						posX = i;
						posY = j;
						sprite.setTexture(enemy);
						sprite.setPosition(maze.coords[posX][posY][0], maze.coords[posX][posY][1]);
						sprite.scale(0.1, 0.1);
						j++;
						return;
					}

				}

			}


		}
		bool testing_row(int n, int j, Maze& maze) {
			for (int i = 0; i < maze.n; i++)
			{
				if (maze.valid[n][j] == 3) {
					return false;
				}
			}
			return true;
		}

		void set_position(Maze& maze) {
			sprite.setPosition(maze.coords[posX][posY][0], maze.coords[posX][posY][1]);
		}

		void create_fire(Maze& maze, int direction) {
			Fire* create = new Fire(maze, posX, posY, direction,2,0);
			create->posX = posX;
			create->posY = posY;
			if (start == nullptr) {
				start = create;
			}
			else {
				Fire* curr = start;
				while (curr->next != nullptr) {
					curr = curr->next;
				}
				curr->next = create;
				create->prev = curr;
			}
			create->move(maze);
		}

		void updateFirePositions(Maze& maze) {
			Fire* curr = start;
			while (curr != nullptr) {
				curr->move(maze);

				if (curr->moved > 4) {

					if (curr == start) {
						start = curr->next;
						if (start)
							start->prev = nullptr;
					}
					else {
						if (curr->prev)
							curr->prev->next = curr->next;
						if (curr->next)
							curr->next->prev = curr->prev;
					}

					Fire* nextFire = curr->next;
					delete curr;
					curr = nextFire;
				}
				else {
					curr = curr->next;
				}
			}
		}


		void moveUp(Maze& maze) {
			if (posY > 0 && maze.isValid(posX, posY - 1)) {
				posY--;

				set_position(maze);
			}
		}

		void moveDown(Maze& maze) {
			if (posY < 35 && maze.isValid(posX, posY + 1)) {
				posY++;

				set_position(maze);
			}
		}

		void moveLeft(Maze& maze) {
			if (posX > 0 && maze.isValid(posX - 1, posY)) {

				posX--;
				set_position(maze);
			}
		}

		void moveRight(Maze& maze) {
			if (posX < 79 && maze.isValid(posX + 1, posY)) {

				posX++;
				set_position(maze);
			}
		}

		void draw(RenderWindow& window) {
			if(visible)
			window.draw(sprite);
			Fire* curr = start;
			while (curr)
			{
				if(curr->visible)
				window.draw(curr->sprite);
				curr = curr->next;
			}
		}

	};

class Enemylist {
public:
	Enemy* start = nullptr;
	Enemylist() {}
	void insert(Maze& maze) {
		Enemy* create = new Enemy(maze);
		if (start == nullptr) {
			start = create;
		}
		else {
			Enemy* curr = start;
			while (curr->next)
			{
				curr = curr->next;
			}
			curr->next = create;
		}
	}
	void create(int val, Maze& maze) {
		for (int i = 0; i < val; i++)
		{
			insert(maze);
		}
	}
	void display(RenderWindow& window) {
		Enemy* curr = start;
		while (curr)
		{
			curr->draw(window);
			curr = curr->next;
		}

	}
	void move(Maze& maze) {
		Enemy* curr = start;
		int count = rand()%5;
		while (curr)
		{
			if(curr->visible)
			if (count % 2 == 1) {

				int val = rand() % 4;
				if (val == 0) {
					curr->moveUp(maze);
				}
				else if (val == 1) {
					curr->moveDown(maze);
				}
				else if (val == 2) {
					curr->moveLeft(maze);
				}
				else {
					curr->moveRight(maze);
				}
			}
			count++;

			curr = curr->next;
		}
	}

};





class Treasure {
	Sprite sprite;
	Texture enemy;
public:
	bool visible = true;;
	int posX = 0;
	int posY = 0;
	int health = 1000;
	Treasure* next = nullptr;
	int reward;
	Treasure() {}

	Treasure(Maze& maze,int reward):reward(reward) {
		
		int val = rand() % 4;
		if (val == 0) {
			enemy.loadFromFile("treasure1.png");

		}
		else  if (val == 1) {
			enemy.loadFromFile("treasure2.png");
		}
		else  if (val == 2) {
			enemy.loadFromFile("treasure3.png");
		}
		else {
			enemy.loadFromFile("treasure4.png");
		}
		int random = rand() % 10;
		for (int j = 1; j < maze.n; ++j) {
		
			for (int i = maze.m - 3; i>= 1; --i) {
				if (rand() % 10 == random && maze.valid[i][j] == 1 && testing_row(i, j, maze)) {
					maze.valid[i][j] = 4;
					posX = i;
					posY = j;
					sprite.setTexture(enemy);
					sprite.setPosition(maze.coords[posX][posY][0], maze.coords[posX][posY][1]);
					sprite.scale(0.1, 0.1);
					j++;
					return;
				}

			}

		}


	}
	bool testing_row(int i, int j, Maze& maze) {
		for (int i = 0; i < maze.n; i++)
		{
			if (maze.valid[i][j] == 4) {
				return false;
			}
		}
		return true;
	}

	void set_position(Maze& maze) {
		sprite.setPosition(maze.coords[posX][posY][0], maze.coords[posX][posY][1]);
	}


	void draw(RenderWindow& window) {
		if (visible)
			window.draw(sprite);
	}

};

class Treeasurelist {
public:
	Treasure* start = nullptr;
	Treeasurelist() {}
	void insert(Maze& maze,int reward) {
		Treasure* create = new Treasure(maze,reward);
		if (start == nullptr) {
			start = create;
		}
		else {
			Treasure* curr = start;
			while (curr->next)
			{
				curr = curr->next;
			}
			curr->next = create;
		}
	}
	void create(int val, Maze& maze) {
		for (int i = 0; i < val; i++)
		{
			insert(maze,i+1);
		}
	}
	void display(RenderWindow& window) {
		Treasure* curr = start;
		while (curr)
		{
			curr->draw(window);
			curr = curr->next;
		}

	}


};