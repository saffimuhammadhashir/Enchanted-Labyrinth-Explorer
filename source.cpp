#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include"highscore.h"
#include"maze.h"
sf::Music music2; // Define music2 as a member variable of the Game class
bool resume = true;
bool gamePaused = false; // Add a flag to trac
void runGame();
void pause()
{
	Font font1;
	if (!font1.loadFromFile("font3.ttf"))
	{
		cerr << "Failed to load font!" << endl;
		return;
	}

	RenderWindow windowM(VideoMode(1080, 935), "pause");

	music2.pause();
	Texture pauseScr;
	pauseScr.loadFromFile("pause.jpg");
	Sprite p(pauseScr);
	p.setScale(0.85, 0.75);

	Text Qtext4;
	Qtext4.setFont(font1);
	Qtext4.setString("GAME PAUSED");
	Qtext4.setPosition(50, 80);
	Qtext4.setCharacterSize(40);
	Qtext4.setFillColor(Color::White);

	Text Qtext5;
	Qtext5.setFont(font1);
	Qtext5.setString("Press R to Resume.");
	Qtext5.setPosition(50, 158);
	Qtext5.setCharacterSize(40);
	Qtext5.setFillColor(Color::White);

	while (windowM.isOpen())
	{
		Event ev;
		while (windowM.pollEvent(ev))
		{

			if (ev.type == Event::Closed)
			{
				windowM.close();
			}
			if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::R)
			{
				windowM.close();
				gamePaused = false; // Update game pause state
				resume = true;
				//music2.play(); // Resume the music when the pause screen is closed
			}
		}
		windowM.draw(p);
		windowM.draw(Qtext4);
		windowM.draw(Qtext5);
		windowM.display();
	}
}


class Game {
	RenderWindow window;
	Maze maze;
	Player* player;
	Enemylist enemies;
	Font font;
	ScoreList highscore;
	Treeasurelist treasure;
	bool inserted = false;
	int score = 0;
	int* activerewards;
	int numImages = 8;
	sf::Texture* textures = new sf::Texture[numImages];
	sf::Sprite* sprites = new sf::Sprite[numImages];
	int lives = 0;
	int val = 0;
	string name = "Hashir";
	sf::Music music;

public:
	Game() : window(VideoMode(1800, 980), "Enchanted Labyrinth Explorer") {
		player = new Player(maze);
		enemies.create(10, maze);
		treasure.create(numImages, maze);
		font.loadFromFile("font2.ttf");
		highscore.readFromFile();
		activerewards = new int[numImages];
		music.openFromFile("Elevator_Music_Vanoss_Gaming_Background_Music_HD(256k).ogg");
		music.setLoop(true);
		for (int i = 0; i < numImages; i++)
		{
			activerewards[i] = 0;
		}
	}


	void run() {
		music.play();
		Clock clock;

		while (window.isOpen()) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed)
					window.close();
				if (event.type == Event::KeyPressed) {
					handleKeyPress(event.key.code);
				}
				if (event.type == sf::Event::MouseButtonPressed) {
					// Check if mouse clicked inside any sprite
					for (int i = 0; i < numImages; ++i) {
						if (sprites[i].getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
							// Zoom the clicked sprite
							if (i != 1 && i != 2) {
								val = i+1;
								sprites[i].setScale(1.0f, 1.15f); // Adjust the scale factor as needed
						
							}
						}
						else {
							sprites[i].setScale(1.0f, 1.0f); // Adjust the scale factor as needed
						}
					}
					cout << val << endl;
				}
			}


			if (clock.getElapsedTime().asSeconds() >= 1.0f) {
				enemies.move(maze);
				clock.restart();
				player->updateFirePositions(maze);
				for (Enemy* enemy = enemies.start; enemy != nullptr; enemy = enemy->next) {
					enemy->updateFirePositions(maze);
					healthmanager();
				}
				fireAtplayer();
			}
			if (lives < 0) {
				window.close();
				highscore.insert(score, name);
				runGame();
				return;
			}

			if (player->posX >= 22) {
				window.close();
				runGame();
				return;
			}
			window.clear();
			maze.drawmaze(window);
			player->draw(window);
			treasurefound();
			enemies.display(window);
			treasure.display(window);
			printHealth();
			displayTopThreeScores();
			displayrewards();
			window.display();
		}
	}
	void displayrewards() {
		std::string imagePaths[] = {
	"rewards1.png",
	"rewards2.png",
	"rewards3.png",
	"rewards4.png",
	"rewards5.png",
	"rewards6.png",
	"rewards7.png",
	"rewards8.png"
		};
		const float spacing = 200.0f;
		float startX = 10.0f;
		float startY = 10.0f;

		for (int i = 0; i < numImages; ++i) {
			textures[i].loadFromFile(imagePaths[i]);
		}

		for (int i = 0; i < numImages; ++i) {
			sprites[i].setTexture(textures[i]);
			sprites[i].setPosition(startX + i * 140, startY);
		}


		for (int i = 0; i < numImages; i++)
		{
			if (activerewards[i] != 0) {
			window.draw(sprites[i]);
			}
		}

	}

	void healthmanager() {
		for (Enemy* enemy = enemies.start; enemy != nullptr; enemy = enemy->next) {
			if (enemy->visible) {
				for (Fire* fire = enemy->start; fire != nullptr; fire = fire->next) {
					if (fire->visible)
						if (fire->posX == player->posX && fire->posY == player->posY)
						{
							player->health -= 50;
							fire->visible = false;
							//game over screen
							//cout << "Player : " << player->health << endl;
							if (player->health <= 0 && !inserted) {
								player->health = 500;
								lives--;
								//highscore.insert(score);
							/*	inserted = true;*/
							}


						}
				}
				for (Fire* fire = player->start; fire != nullptr; fire = fire->next) {
					if (fire->visible)
						if (fire->posX == enemy->posX && enemy->posY == enemy->posY)
						{
							enemy->health -= 50;
							fire->visible = false;
							if (enemy->health <= 0) {
								maze.valid[enemy->posX][enemy->posY] = 1;
								enemy->visible = false;
								score += 50;
								/*enemies.insert(maze);*/
								/*for (int j = 0; j < 12; j++) {
									for (int i = 0; i < 23; i++) {
										cout << maze.valid[i][j] << "  ";
									}
									cout << endl;
								}
								cout << endl;
								cout << endl;
								cout << endl;*/
							}
							//cout << "Enemy : " << enemy->health << endl;


						}
				}
			}


		}
	}

	void fireAtplayer() {

		int playerX = player->posX;
		int playerY = player->posY;


		for (Enemy* enemy = enemies.start; enemy != nullptr; enemy = enemy->next) {
			int enemyX = enemy->posX;
			int enemyY = enemy->posY;

			if (enemy->visible) {


				if (enemyY == playerY && abs(enemyX - playerX) <= 3) {
					maze.valid[enemyX][enemyY] = 1;
					//enemy->visible = false;
					if (enemyX > playerX) {

						player->create_fire(maze, 3,val);
						enemy->create_fire(maze, 2);
					}
					if (enemyX < playerX) {
						enemy->create_fire(maze, 3);
						player->create_fire(maze, 2,val);
					}


				}
				if (enemyX == playerX && abs(enemyY - playerY) <= 3) {
					maze.valid[enemyX][enemyY] = 1;
					//enemy->visible = false;
					if (enemyY > playerY) {

						player->create_fire(maze, 1,val);
						enemy->create_fire(maze, 0);
					}
					if (enemyY < playerY) {

						player->create_fire(maze, 0,val);
						enemy->create_fire(maze, 1);
					}



				}
			}
		}
	}

	void fireAtenemies() {

		int playerX = player->posX;
		int playerY = player->posY;


		for (Enemy* enemy = enemies.start; enemy != nullptr; enemy = enemy->next) {
			int enemyX = enemy->posX;
			int enemyY = enemy->posY;

			if (enemy->visible) {


				if (enemyY == playerY && abs(enemyX - playerX) <= 5) {
					maze.valid[enemyX][enemyY] = 1;
					//enemy->visible = false;
					if (enemyX > playerX) {

						player->create_fire(maze, 3,val);
					}
					if (enemyX < playerX) {

						player->create_fire(maze, 2,val);

					}


				}
				if (enemyX == playerX && abs(enemyY - playerY) <= 5) {
					maze.valid[enemyX][enemyY] = 1;
					//enemy->visible = false;
					if (enemyY > playerY) {

						player->create_fire(maze, 1,val);

					}
					if (enemyY < playerY) {

						player->create_fire(maze, 0,val);

					}



				}
			}
		}
	}

	void handleKeyPress(Keyboard::Key key) {
		switch (key) {
		case Keyboard::Up:
			player->moveUp(maze);

			break;
		case Keyboard::Down:
			player->moveDown(maze);

			break;
		case Keyboard::Left:
			player->moveLeft(maze);

			break;
		case Keyboard::Right:
			player->moveRight(maze);

			break;
		case Keyboard::Space:
			fireAtenemies();
			break;
		case Keyboard::P:
			pause();
			break;
		default:
			break;
		}
	}

	void printHealth() {
		int playerHealth = player->health;
		int closestEnemyHealth = INT_MAX;
		float minDistance = FLT_MAX;
		Text playerHealthText, enemyHealthText, Myscore, mylives, myname;
		playerHealthText.setFont(font);
		enemyHealthText.setFont(font);
		Myscore.setFont(font);
		mylives.setFont(font);
		myname.setFont(font);
		playerHealthText.setCharacterSize(30);
		enemyHealthText.setCharacterSize(30);
		Myscore.setCharacterSize(30);
		mylives.setCharacterSize(30);
		myname.setCharacterSize(30);
		playerHealthText.setFillColor(Color::Yellow);
		enemyHealthText.setFillColor(Color::Yellow);
		Myscore.setFillColor(Color::Yellow);
		mylives.setFillColor(Color::Yellow);
		myname.setFillColor(Color::Yellow);
		playerHealthText.setPosition(320, 150);
		enemyHealthText.setPosition(570, 150);
		Myscore.setPosition(900, 150);
		mylives.setPosition(1040, 150);
		myname.setPosition(10, 150);
		Texture lifeT;
		lifeT.loadFromFile("lives.png");
		Sprite life(lifeT);



		for (Enemy* enemy = enemies.start; enemy != nullptr; enemy = enemy->next) {
			if (enemy->visible) {
				float distance = sqrt(pow(player->posX - enemy->posX, 2) + pow(player->posY - enemy->posY, 2));
				if (distance < minDistance) {
					minDistance = distance;
					closestEnemyHealth = enemy->health;
				}
			}
		}


		playerHealthText.setString("Player Health: " + to_string(playerHealth));
		enemyHealthText.setString("Closest Enemy Health: " + to_string(closestEnemyHealth));
		Myscore.setString("Score: " + to_string(score));
		mylives.setString("Lives: ");
		myname.setString("Name: " + name);

		for (int i = 0; i < lives; i++)
		{
			life.setPosition(1110 + i * 35, 155);
			window.draw(life);
		}
		window.draw(myname);
		window.draw(Myscore);

		window.draw(mylives);
		window.draw(playerHealthText);
		window.draw(enemyHealthText);


	}

	void displayTopThreeScores() {

		Text heading;
		heading.setFont(font);
		heading.setCharacterSize(40);
		heading.setFillColor(Color::Red);
		heading.setPosition(1450, 20);
		string scoreText = "Scoreboard";
		heading.setString(scoreText);
		window.draw(heading);
		Text text;
		text.setFont(font);
		text.setCharacterSize(24);
		text.setFillColor(Color::White);


		float x = 1450.f;
		float y = 80.0f;
		score_body* temp = highscore.head;
		int count = 0;
		bool done_display = false;
		while (temp != nullptr && count < 3) {
			if (temp->score < score && !done_display) {
				done_display = true;
				scoreText = to_string(count + 1) + ".  " + "You" + "(" + to_string(score) + ")";
				text.setFillColor(Color::Blue);
			}
			else {
				scoreText = to_string(count + 1) + ".  " + temp->name + "(" + to_string(temp->score) + ")";
				text.setFillColor(Color::White);
				temp = temp->next;
			}
			text.setString(scoreText);

			text.setPosition(x, y);

			window.draw(text);

			y += 30.f;

			count++;
		}
	}

	void treasurefound() {
		Treasure* curr = treasure.start;
		int i = 0;
		while (curr)
		{
			if (curr->visible == true && maze.coords[player->posX][player->posY][0] == maze.coords[curr->posX][curr->posY][0] && maze.coords[player->posX][player->posY][1] == maze.coords[curr->posX][curr->posY][1]) {
				activerewards[i] = curr->reward;

				if (curr->reward == 1) {
					score += 100;
				}
				else if (curr->reward == 2) {
					lives += 1;
				}
				else if (curr->reward == 3) {
					lives += 2;
				}
				else if (curr->reward == 4) {
					score += 150;
				}
				else if (curr->reward == 5) {
					score += 170;
				}
				else if (curr->reward == 6) {
					score += 190;
				}
				else if (curr->reward == 7) {
					score += 210;
				}
				else if (curr->reward == 8) {
					score += 250;
				}


				curr->visible = false;
			}
			curr = curr->next;
			i++;
		}


	}
};




class Menu
{
private:
	Texture menuBgTexture;
	Sprite menuBackground;
	sf::Music music; // Added music object


public:
	Menu() {
		if (!menuBgTexture.loadFromFile("background.jpg")) {
			cerr << "Failed to load menu background image!" << endl;
			return;
		}

		menuBackground.setTexture(menuBgTexture);
		menuBackground.setScale(0.85, 0.75);

		if (!music.openFromFile("Elevator_Music_Vanoss_Gaming_Background_Music_HD(256k).ogg")) {
			cerr << "Failed to load menu music!" << endl;
			return;
		}

		music.play();
		music.setLoop(true);
	}

	~Menu()
	{
		music.stop();

		music.setLoop(false);
	}

	void display_menu() {
		RenderWindow window(VideoMode(1080, 935), "Menu");

		Font font;
		if (!font.loadFromFile("font3.ttf")) {
			cerr << "Failed to load font!" << endl;
			return;
		}

		sf::Text startText3;
		startText3.setFont(font);
		startText3.setString("ENCHANTED");
		startText3.setCharacterSize(43);
		startText3.setFillColor(sf::Color(255, 215, 0)); // RGB values for golden color
		startText3.setPosition(400.f, 760.f);

		Text startText4;
		startText4.setFont(font);
		startText4.setString("LABYRINTH");
		startText4.setCharacterSize(55);
		startText4.setFillColor(sf::Color(255, 215, 0)); // RGB values for golden color
		startText4.setPosition(367.f, 800.f);

		Text startText5;
		startText5.setFont(font);
		startText5.setString("EXPLORER");
		startText5.setCharacterSize(43);
		startText5.setFillColor(sf::Color(255, 215, 0)); // RGB values for golden color
		startText5.setPosition(420.f, 852.f);

		Text startText;
		startText.setFont(font);
		startText.setString("Press ENTER to Play");
		startText.setCharacterSize(22);
		startText.setFillColor(Color::White);
		startText.setPosition(680.f, 892.f);

		Text instructionText;
		instructionText.setFont(font);
		instructionText.setString("Press 'I' for instructions");
		instructionText.setCharacterSize(22);
		instructionText.setFillColor(Color::White);
		instructionText.setPosition(85.f, 892.f);

		bool showInstruction = false;

		while (window.isOpen()) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed)
					window.close();
				else if (event.type == Event::KeyPressed) {
					if (!showInstruction) {
						if (event.key.code == Keyboard::Enter) {
							// Start the game when Enter is pressed
							music.stop();
							window.close();
							Game g;
							g.run();
							// window.close(); // Close the menu window
						}
						else if (event.key.code == Keyboard::Escape) {
							window.close();
						}
						else if (event.key.code == Keyboard::I) {
							// Show instruction page on 'I' key press
							show_instruction();
						}
					}
					else {
						if (event.key.code == Keyboard::Enter) {
							// Go back to the menu when Enter is pressed on the instruction page
							showInstruction = false;
						}
					}
				}
			}

			window.clear(Color::Black);
			window.draw(menuBackground);

			if (!showInstruction) {
				window.draw(startText3);
				window.draw(startText4);
				window.draw(startText5);
				window.draw(startText);
				window.draw(instructionText);
			}

			window.display();
		}
	}



	void show_instruction() {
		RenderWindow instructionWindow(VideoMode(1080, 935), "Instructions");

		Texture instBgTexture;
		Sprite instBackground;
		Font font;

		if (!instBgTexture.loadFromFile("pbg.jpg")) {
			cerr << "Failed to load instruction background image!" << endl;
			return;
		}

		instBackground.setTexture(instBgTexture);
		instBackground.setScale(0.85, 0.75);

		if (!font.loadFromFile("font3.ttf")) {
			cerr << "Failed to load font!" << endl;
			return;
		}

		Text instructionText;
		instructionText.setFont(font);
		instructionText.setString("ENCHANT, DISCOVER & CONQUER!");
		instructionText.setCharacterSize(56);
		instructionText.setFillColor(Color::White);
		instructionText.setPosition(70.f, 43.f);

		// Rest of the instruction texts
		Text instructionText2;
		instructionText2.setFont(font);
		instructionText2.setString("---->>> KEY CONTROLS <<<----");
		instructionText2.setCharacterSize(30);
		instructionText2.setFillColor(Color::White);
		instructionText2.setPosition(300.f, 176.f);

		Text instructionText3;
		instructionText3.setFont(font);
		instructionText3.setString("Up key: Player moves upward");
		instructionText3.setCharacterSize(25);
		instructionText3.setFillColor(Color::White);
		instructionText3.setPosition(300.f, 230.f);

		Text instructionText4;
		instructionText4.setFont(font);
		instructionText4.setString("Down key: Player moves downward");
		instructionText4.setCharacterSize(25);
		instructionText4.setFillColor(Color::White);
		instructionText4.setPosition(270.f, 260.f);

		Text instructionText5;
		instructionText5.setFont(font);
		instructionText5.setString("Right key: Player moves right");
		instructionText5.setCharacterSize(25);
		instructionText5.setFillColor(Color::White);
		instructionText5.setPosition(300.f, 290.f);

		Text instructionText6;
		instructionText6.setFont(font);
		instructionText6.setString("Left key: Player moves left");
		instructionText6.setCharacterSize(25);
		instructionText6.setFillColor(Color::White);
		instructionText6.setPosition(300.f, 320.f);

		Text instructionText7;
		instructionText7.setFont(font);
		instructionText7.setString("---->>> OBJECTIVES <<<----");
		instructionText7.setCharacterSize(30);
		instructionText7.setFillColor(Color::White);
		instructionText7.setPosition(335.f, 450.f);

		Text instructionText8;
		instructionText8.setFont(font);
		instructionText8.setString("Strategically navigate , beware hidden traps.");
		instructionText8.setCharacterSize(25);
		instructionText8.setFillColor(Color::White);
		instructionText8.setPosition(184.f, 510.f);

		Text instructionText9;
		instructionText9.setFont(font);
		instructionText9.setString("Collect enchantments for power-ups.");
		instructionText9.setCharacterSize(25);
		instructionText9.setFillColor(Color::White);
		instructionText9.setPosition(240.f, 555.f);

		Text instructionText10;
		instructionText10.setFont(font);
		instructionText10.setString("Explore labyrinth depths , stay vigilant for treasures.");
		instructionText10.setCharacterSize(25);
		instructionText10.setFillColor(Color::White);
		instructionText10.setPosition(130.f, 600.f);

		Text instructionText12;
		instructionText12.setFont(font);
		instructionText12.setString(" Battle mystical creatures with treasures collected.");
		instructionText12.setCharacterSize(25);
		instructionText12.setFillColor(Color::White);
		instructionText12.setPosition(140.f, 645.f);

		Text instructionText11;
		instructionText11.setFont(font);
		instructionText11.setString("Press ENTER to return to menu");
		instructionText11.setCharacterSize(35);
		instructionText11.setFillColor(Color::White);
		instructionText11.setPosition(215.f, 800.f);


		while (instructionWindow.isOpen()) {
			Event event;
			while (instructionWindow.pollEvent(event)) {
				if (event.type == Event::Closed)
					instructionWindow.close();
				else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
					// Close the instruction window on Enter
					instructionWindow.close();
				}
			}

			instructionWindow.clear(Color::Black);
			instructionWindow.draw(instBackground);
			instructionWindow.draw(instructionText);
			instructionWindow.draw(instructionText2);
			instructionWindow.draw(instructionText3);
			instructionWindow.draw(instructionText4);
			instructionWindow.draw(instructionText5);
			instructionWindow.draw(instructionText6);
			instructionWindow.draw(instructionText7);
			instructionWindow.draw(instructionText8);
			instructionWindow.draw(instructionText9);
			instructionWindow.draw(instructionText10);
			instructionWindow.draw(instructionText12);
			instructionWindow.draw(instructionText11);
			instructionWindow.display();
		}
	}
};





void runGame() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Game");

	sf::Font font;
	if (!font.loadFromFile("font3.ttf")) {
		std::cerr << "Failed to load font" << std::endl;
		return;
	}

	sf::Text gameOverText("Game Over", font, 50);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setPosition(200, 200);

	sf::Text restartText("Press R to Restart", font, 30);
	restartText.setFillColor(sf::Color::White);
	restartText.setPosition(200, 300);

	sf::Text exitText("Press Escape to Exit", font, 30);
	exitText.setFillColor(sf::Color::White);
	exitText.setPosition(200, 350);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					exit(0);
					window.close();
					
				}
				if (event.key.code == sf::Keyboard::R) {
					std::cout << "Restarting the game..." << std::endl;
					Menu m;
					m.display_menu();
				}
			}
		}

		// Here you would typically have your game logic and drawing code

		// For demonstration purposes, let's simulate a game over after 5 seconds

		window.clear();
		window.draw(gameOverText);
		window.draw(restartText);
		window.draw(exitText);
		window.display();
	}
}



int main() {
	Menu m;
	m.display_menu();
	return 0;
}
