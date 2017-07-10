#include "Game.h"
#include "Actions.h"
#include "Constants.h"
#include "GameFont.h"
#include "Sprite.h"

#include "Enemy.h"

#include <Engine/Input.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>

#include <windows.h> 

#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll


/**
*   @brief   Default Constructor.
*/
PacManGame::PacManGame()
{
	//Enemy _enemy;
}


/**
*   @brief   Destructor.
*   @details Remove any non-managed memory and callbacks.
*/
PacManGame::~PacManGame()
{
	this->inputs->unregisterCallback(callback_id);
	engine1->stopAllSounds();
	engine2->stopAllSounds();
	engine3->stopAllSounds();
	engine4->stopAllSounds();
}

/**
*   @brief   Initialises sounds.
*   @details Enables all engines and checks if they started up correctly
*/
bool PacManGame::initAudio()
{
	using namespace irrklang;
	engine1.reset(createIrrKlangDevice());
	engine2.reset(createIrrKlangDevice());
	engine3.reset(createIrrKlangDevice());
	engine4.reset(createIrrKlangDevice());

	if (!engine1 || !engine2 || !engine3 || !engine4)
	{
		return false; // error starting up the engine
	}
	return true;
}

/**
*   @brief   Initialises the game.
*   @details The game window is created and all assets required to
			 run the game are loaded. The input callback should also
			 be set in the initialise function. 
*   @return  True if the game initialised correctly.
*/
bool PacManGame::init()
{
	width = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;
	if (!initAPI())
	{
		return false;
	}

	initAudio();
	if (!initAudio())
	{
		return false;
	}

	renderer->setWindowTitle("Invaders - Exercise 1");
	renderer->setClearColour(ASGE::COLOURS::BLACK);
	toggleFPS();

	// input callback function
	callback_id = this->inputs->addCallbackFnc(&PacManGame::input, this);
	
	// load fonts we need
	GameFont::fonts[0] = new GameFont(
		renderer->loadFont("..\\..\\Resources\\Fonts\\Comic.ttf", 42), "default", 42);
	
	if (GameFont::fonts[0]->id == -1)
	{
		return false;
	}

	// menu screen
	sprite = renderer->createSprite();
	sprite->position[0] = 0;
	sprite->position[1] = 0;
	sprite->scale = 1.5;
	if (!sprite->loadTexture("..\\..\\Resources\\Textures\\pacman-ghosts.jpg"))
	{
		return false;
	}

	// pacman textures
	pacman = renderer->createSprite();
	pacman->position[0] = 425;
	pacman->position[1] = 225;
	pacman->size[0] = 50;
	pacman->size[1] = 50;
	pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanRight1.png");
	pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanRight2.png");
	pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanLeft1.png");
	pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanLeft2.png");
	pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanUp1.png");
	pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanUp2.png");
	pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanDown1.png");
	pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanDown2.png");

	//Enemy::init();

	// enemies
	for (size_t j = 0; j < total_enemies; j++)		// following for loop loads textures for each enemy 
	{												// and sets their start co-ords as well as their start directions
		enemy[j] = renderer->createSprite();
		if (j == 0)
		{
			enemy[j]->position[0] = 220;
			enemy[j]->position[1] = 120;
			enemy_direction[j] = 1;
			if (!enemy[j]->loadTexture("..\\..\\Resources\\Textures\\inky.png"))
			{
				return false;
			}
		}
		if (j == 1)
		{
			enemy[j]->position[0] = 1120;
			enemy[j]->position[1] = 120;
			enemy_direction[j] = 4;
			if (!enemy[j]->loadTexture("..\\..\\Resources\\Textures\\pinky.png"))
			{
				return false;
			}
		}
		if (j == 2)
		{
			enemy[j]->position[0] = 420;
			enemy[j]->position[1] = 320;
			enemy_direction[j] = 4;
			if (!enemy[j]->loadTexture("..\\..\\Resources\\Textures\\blinky.png"))
			{
				return false;
			}
		}
		if (j == 3)
		{
			enemy[j]->position[0] = 620;
			enemy[j]->position[1] = 520;
			enemy_direction[j] = 2;
			if (!enemy[j]->loadTexture("..\\..\\Resources\\Textures\\clyde.png"))
			{
				return false;
			}
		}
		enemy[j]->size[0] = 60;
		enemy[j]->size[1] = 60;
	} // end of enemies

	// walls
	int x = 0;
	int y = 0;
	int k = 0;
	for (size_t i = 0; i < total_walls; i++)	// draws the whole level
	{
		wall[i] = renderer->createSprite();
		if (i < 12)							// top border
		{
			wall[i]->position[0] = x;
			wall[i]->position[1] = 0;
			x = x + 100;
		}
		else if (i >= 12 && i < 19)			// right border
		{
			wall[i]->position[0] = 1200;
			wall[i]->position[1] = y;
			y = y + 100;
		}
		else if (i >= 19 && i < 31)			// botom border
		{
			x = x - 100;
			wall[i]->position[0] = x;
			wall[i]->position[1] = 600;
		}
		else if (i >= 31 && i < 37)			// left border
		{
			y = y - 100;
			wall[i]->position[0] = 0;
			wall[i]->position[1] = y;
		}
											// from here on, drawing blocks horizontally line-by-line 
		else if (i >= 37 && i < 38)			// single block on co-ords (1000,100)
		{
			wall[i]->position[0] = 1000;
			wall[i]->position[1] = 100;
		}

		if (i == 37)						// sets x & y to start making new lines along y-axis 200
		{
			x = 100;
			y = 200;
		}
		else if (i >= 37 && i < 45)			// lines 3 by 1 and 4 by 1 
		{
			wall[i]->position[0] = x;
			wall[i]->position[1] = y;
			k++;
			if (k < 3 || k > 3)
			{
				x = x + 100;
			}
			else							// making a gap
			{
				x = x + 200;
			}
			if (k == 7)
			{
				k = 0;						// resetting 'k' so I can use it later
			}
		}

		if (i == 44)						// sets x & y to start making new line of blocks along y-axis 300
		{
			x = 600;
			y = 300;
		}
		else if (i >= 44 && i < 47)			// 2 blocks 1 by 1
		{
			wall[i]->position[0] = x;
			wall[i]->position[1] = y;
			x = x + 400;
		}

		if (i == 46)						// sets x & y to start making new line of blocks along y-axis 400
		{
			x = 200;
			y = 400;
		}
		else if (i >= 46 && i < 55)			// blocks 1 by 1 
		{
			wall[i]->position[0] = x;
			wall[i]->position[1] = y;
			k++;
			if (k < 3 || k == 5 || k > 6)	// line 3 by 1, block 1 by 1 and another 3 by 1 line
			{
				x = x + 100;
			}
			else if (k == 3 || k == 4)		// making 2 gaps
			{
				x = x + 200;
			}
		}

		wall[i]->size[0] = 100;
		wall[i]->size[1] = 100;
		//wall[i]->SetTag("Wall");
		if (!wall[i]->loadTexture("..\\..\\Resources\\Textures\\wall.png"))
		{
			return false;
		}
	} // end of walls

	// collectables
	collectable[0] = renderer->createSprite();		// an icon for Score
	collectable[0]->position[0] = 1245;
	collectable[0]->position[1] = 0;
	if (!collectable[0]->loadTexture("..\\..\\Resources\\Textures\\collectable.png"))
	{
		return false;
	}
	x = 125;
	y = 125;
	k = 0;
	for (size_t u = 0; u < total_collectables; u++)	// this for loop sets position for collectables
	{
		collectable[u] = renderer->createSprite();
		k++;
		if (u < 10)									// first line (starting in top left)
		{
			collectable[u]->position[0] = x;
			collectable[u]->position[1] = 125;

			if (k < 9)
			{
				x = x + 100;
			}
			else if (k == 9)
			{
				x = x + 200;
			}
		}
		else if (u >= 10 && u < 12)					// second line
		{
			if (k >= 9)
			{
				x = 925;
				k = 0;
			}
			collectable[u]->position[0] = x;
			collectable[u]->position[1] = 225;
			x = x + 200;
		}
		else if (u >= 12 && u < 21)					// third line
		{
			if (k == 2)
			{
				x = 125;
			}
			collectable[u]->position[0] = x;
			collectable[u]->position[1] = 325;
			if (k < 6 || k > 6 && k != 9)
			{
				x = x + 100;
			}
			else if (k == 6 || k == 9)
			{
				x = x + 200;
			}
		}
		else if (u >= 21 && u < 25)					// fourth line
		{
			if (k == 11)
			{
				x = 125;
			}
			collectable[u]->position[0] = x;
			collectable[u]->position[1] = 425;
			if (k >= 11 && k != 12)
			{
				x = x + 400;
			}
			if (k == 12)
			{
				x = x + 200;
			}
		}
		else if (u >= 25 && u < 36)					// fith (final) line 
		{
			if (k == 15)
			{
				x = 125;
			}
			collectable[u]->position[0] = x;
			collectable[u]->position[1] = 525;
			x = x + 100;
		}
		collectable[u]->size[0] = 50;			// sets same size for all collectables
		collectable[u]->size[1] = 50;			
		if (!collectable[u]->loadTexture("..\\..\\Resources\\Textures\\collectable.png"))
		{
			return false;
		}
	} // end of collectables

	cherry = renderer->createSprite();
	cherry->position[0] = 1025;
	cherry->position[1] = 225;
	cherry->size[0] = 50;
	cherry->size[1] = 50;
	if (!cherry->loadTexture("..\\..\\Resources\\Textures\\cherry.png"))
	{
		return false;
	}
	// attempt at doing animation from sprite sheets (further research required)
	//int number_of_frames_wide = 2;
	//int number_of_frames_tall = 4;
	//float width = pacman->size[0] / number_of_frames_wide;
	//float height = pacman->size[1] / number_of_frames_tall;
	////SetSize({ width, height });
	//int location = 0;
	//for (int h = 0; h < number_of_frames_tall; h++)
	//{
	//	for (int w = 0; w < number_of_frames_wide; w++)
	//	{
	//		sprite_cut[location] = (h + 1) * height;
	//		sprite_cut[location] = h * height;
	//		sprite_cut[location] = w * width;
	//		sprite_cut[location] = w * width + width;
	//		location++;
	//	}
	//}
	return true;
}


/**
*   @brief   The main game loop. 
*   @details The main loop should be responsible for updating the game
			 and rendering the current scene. Runs until the shouldExit
			 signal is received.
*   @return  True if the game ran correctly. 
*/
bool PacManGame::run()
{
	while (!shouldExit())
	{
		/*std::thread thread(input);
		while (true)
		{*/
		processGameActions();
		render();
		//}
	}

	return true;
}


/**
*   @brief   Should the game exit?
*   @details Has the renderer terminated or the game requesting to exit?
*   @return  True if the game should exit
*/
bool PacManGame::shouldExit() const
{
	return (renderer->exit() || this->exit);
}


/**
*   @brief   Renders the scene
*   @details Prepares the renderer subsystem before drawing the 
			 current frame. Once the current frame is has finished
			 the buffers are swapped accordingly and the image shown.
*   @return  void
*/
void PacManGame::render()
{
	beginFrame();
	drawFrame();
	endFrame();
}


/**
*   @brief   Renderers the contents for this frame 
*   @details All game objects that need rendering should be done
			 in this function, as it ensures they are completed
			 before the buffers are swapped.
*   @return  void
*/
void PacManGame::drawFrame()
{
	s++;				// if FPS is locked to 60, this will change the pacman sprite every 0.5 second
	if (s > 30)
	{
		s = 0;
	}
	if (cherry_col == true)
	{
		w++;
		if (w < 300)	// "God-Mode" lasts 5 seconds
		{
			if (w == 1)	// do this once
			{			// makes player faster than enemies
				engine1->stopAllSounds();
				engine4->setSoundVolume(0.2);
				engine4->play2D("..\\..\\Resources\\Sounds\\godmode.wav", true);
				pacman_speed = 3;
			}
		}
		else if (w == 300)
		{				// and here it resets to normal behaviours
			engine1->play2D("..\\..\\Resources\\Sounds\\pacman.wav", true);
			engine4->stopAllSounds();
			pacman_speed = 2;
			cherry_col = false;
		}
		else
		{
			w = 301;	// stops 'int w' from getting to needlesly huge numbers
		}
	}

	if (game_action == GameAction::PAUSE || game_action == GameAction::MENU)
	{
		player_direction = 0;		// stops player if P key is pressed
	}

	/***** PLAYER COLLISION / MOVEMET / SOUND *****/
	if (player_wall_col == false)
	{
		if (sound_on == true && cherry_col == false)
		{
			engine1->stopAllSounds();
			engine1->play2D("..\\..\\Resources\\Sounds\\pacman.wav", true);
			sound_on = false;
		}
		else if (sound_on == true && cherry_col == true)
		{
			engine1->stopAllSounds();
			engine4->stopAllSounds();
			engine4->play2D("..\\..\\Resources\\Sounds\\godmode.wav", true);
			sound_on = false;
		}
		if (player_direction == 1)			// Right
		{
			pacman->position[0] += pacman_speed;
			if (s < 15)
			{
				pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanRight1.png");
			}
			else if (s > 15)
			{
				pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanRight2.png");
			}
		}
		else if (player_direction == 2)		// Left
		{
			pacman->position[0] -= pacman_speed;
			if (s < 15)
			{
				pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanLeft1.png");
			}
			else if (s > 15)
			{
				pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanLeft2.png");
			}
		}
		else if (player_direction == 3)		// Up
		{
			pacman->position[1] -= pacman_speed;
			if (s < 15)
			{
				pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanUp1.png");
			}
			else if (s > 15)
			{
				pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanUp2.png");
			}
		}
		else if (player_direction == 4)		// Down
		{
			pacman->position[1] += pacman_speed;
			if (s < 15)
			{
				pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanDown1.png");
			}
			else if (s > 15)
			{
				pacman->loadTexture("..\\..\\Resources\\Textures\\pacman\\pacmanDown2.png");
			}
		}
	}
	else if (player_wall_col == true)
	{
		sound_on = false;
		engine1->stopAllSounds();
		engine4->stopAllSounds();
		pacman->position[1] = pacman->position[1];
		if (player_direction == 1)
		{
			pacman->position[0] = pacman->position[0] - 5;

		}
		else if (player_direction == 2)
		{
			pacman->position[0] = pacman->position[0] + 5;
		}
		else if (player_direction == 3)
		{
			pacman->position[1] = pacman->position[1] + 5;
		}
		else if (player_direction == 4)
		{
			pacman->position[1] = pacman->position[1] - 5;
		}
		player_direction = 0;
		player_wall_col = false;
	}

	if (player_collectable_col == true)								// adds score upon collision, gives sound feedback, and hides collectables player interacted with
	{
		engine1->play2D("..\\..\\Resources\\Sounds\\ding.wav");
		collectable[u]->position[0] = 1245;							// for some reason this stopped working... 
		collectable[u]->position[1] = 0;							// I have a suspition why... but this isn't essential...
		player_collectable_col = false;
		score++;
	}

	if (cherry_col == true)
	{
		cherry->position[0] = 1245;
		cherry->position[1] = 0;
	}

	// rendering collectables
	collectable[0]->render(renderer);
	for (size_t u = 0; u < total_collectables; u++)
	{
		collectable[u]->render(renderer);
	}

	cherry->render(renderer);

	/***** ENEMY COLLISION / MOVEMET *****/
	if (hardmode == false)
	{
		total_enemies = 3;
	}
	else if (hardmode == true)
	{
		enemy_speed = 2.5;
	}
	for (size_t j = 0; j < total_enemies; j++)											// loops through all enemies
	{
		if (game_action == GameAction::PAUSE || game_action == GameAction::MENU)
		{
			enemy_direction[j] = 0;														// stops enemy movement on pause
		}	
		if (prev_game_action == GameAction::PAUSE && game_action == GameAction::PLAY)
		{
			for (size_t j = 0; j < total_enemies; j++)
			{
				enemy_direction[j] = prev_enemy_direction[j];
				if (enemy_direction[j] == 0)
				{
					enemy_direction[j] ++;							// tell enemy to move
				}
			}
			prev_game_action = GameAction::NONE;
		}
		if (enemy_wall_col[j] == false)								// applies movement to enemies
		{
			if (enemy_direction[j] == 1)
			{
				enemy[j]->position[0] += enemy_speed;
			}
			else if (enemy_direction[j] == 2)
			{
				enemy[j]->position[0] -= enemy_speed;
			}
			else if (enemy_direction[j] == 3)
			{
				enemy[j]->position[1] -= enemy_speed;
			}
			else if (enemy_direction[j] == 4)
			{
				enemy[j]->position[1] += enemy_speed;
			}
			else/* if (enemy_direction[j] == 0)*/
			{
				enemy[j]->position[0] = enemy[j]->position[0];
				enemy[j]->position[1] = enemy[j]->position[1];
			}
		}
		else if (enemy_wall_col[j] == true)							// if enemy collieds with wall
		{
			int randNum = rand() % 4 + 1;							// tell him to go elsewhere (slighlty randomiesed)
			prev_enemy_direction[j] = enemy_direction[j];			// but not in same direction he was just going
			if (enemy_direction[j] == 1)
			{
				enemy[j]->position[0] = enemy[j]->position[0] - 5;
			}
			else if (enemy_direction[j] == 2)
			{
				enemy[j]->position[0] = enemy[j]->position[0] + 5;
			}
			else if (enemy_direction[j] == 3)
			{
				enemy[j]->position[1] = enemy[j]->position[1] + 5;
			}
			else if (enemy_direction[j] == 4)
			{
				enemy[j]->position[1] = enemy[j]->position[1] - 5;
			}
			enemy_direction[j] = randNum;
			enemy_wall_col[j] = false;
		}
		if (enemy_direction[j] == prev_enemy_direction[j])
		{
			enemy_direction[j]=+2;
			if (enemy_direction[j] > 4)
			{
				enemy_direction[j] = 1;
			}
		}
		enemy[j]->render(renderer);								// renders enemies

		if (ghost_sound == false)
		{
			engine2->setSoundVolume(0.2);
			engine2->play2D("..\\..\\Resources\\Sounds\\ghost.wav", true);
			ghost_sound = true;
		}
	}
	//for (int j = 0; j < total_enemies; j++)
	//{
		if (player_enemy_col == true)							// if player collides with enemy
		{								
			pacman->position[0] = 425;							// teleport him to starting position
			pacman->position[1] = 225;
			player_enemy_col = false;
			player_direction = 0;
			engine1->stopAllSounds();
			lives--;											// and take a life away from him
		}

		pacman->render(renderer);

		for (size_t i = 0; i < total_walls; i++)
		{
			wall[i]->render(renderer);							// renders all wals (in positions defined in main init() funciton)
		}
	//}

		renderer->setFont(GameFont::fonts[0]->id);				// the following 100 lines of code or so, render all UI on the screen as wel as text in menu

		if (game_action == GameAction::PLAY)					// displayed during game-play
		{
			renderer->renderText("Press P to pause the game", 100, 45, 1.0, ASGE::COLOURS::YELLOWGREEN); 
		}
		// below I'm converting int to string to pointer of a const char... insanity... (but it works!)
		renderer->renderText("Lives:", 770, 45, 0.9, ASGE::COLOURS::DARKORANGE);
		std::string b = std::to_string(lives);
		char const *n = b.c_str();
		renderer->renderText(n, 910, 45, 1.0, ASGE::COLOURS::RED);
	
		renderer->renderText("Score:", 1050, 45, 1.0, ASGE::COLOURS::DARKORANGE);
		std::string c = std::to_string(score);
		char const *v = c.c_str();
		renderer->renderText(v, 1210, 45, 1.0, ASGE::COLOURS::WHITE);

		if (game_action == GameAction::PAUSE && lives > 0 && score != total_collectables) // displayed if game is not finished, and user presses P key
		{
			renderer->renderText("    GAME PAUSED... ", 500, 340, 1.1, ASGE::COLOURS::AQUA);
			renderer->renderText("PRESS 'ENTER' TO RESUME", 450, 390, 1.1, ASGE::COLOURS::AQUA);
		}

		if (lives == 0)
		{
			if  (e == 0)
			{
				engine3->play2D("..\\..\\Resources\\Sounds\\dead.wav", false);			// displayed when game is lost
				e++;
			}
			game_action = GameAction::PAUSE;
			renderer->renderText("You lost  :(", 500, 340, 1.1, ASGE::COLOURS::ORANGERED);
			renderer->renderText("Your final score is:", 450, 390, 1.1, ASGE::COLOURS::ORANGERED);
			renderer->renderText(v, 625, 465, 1.1, ASGE::COLOURS::ORANGERED);
			renderer->renderText("Thank you for playing", 450, 540, 1.1, ASGE::COLOURS::ORANGERED);
			renderer->renderText("Press 'ESC' to exit.", 450, 590, 1.1, ASGE::COLOURS::ORANGERED);
		}

		if (score == total_collectables)												// displayed when game is won
		{
			game_action = GameAction::PAUSE;
			renderer->renderText("You won!  :)", 500, 340, 1.1, ASGE::COLOURS::ORANGERED);
			renderer->renderText("Thank you for playing", 450, 540, 1.1, ASGE::COLOURS::ORANGERED);
			renderer->renderText("Press 'ESC' to exit.", 450, 590, 1.1, ASGE::COLOURS::ORANGERED);
		}
		if (game_action == GameAction::MENU || game_action == GameAction::INFO)			// displayes menu screen
		{
			sprite->render(renderer);													// renderes the menu image
			if (show_info == false)
			{
				renderer->renderText("[ENTER]   to Start", 400, 500, 1.1, ASGE::COLOURS::ORANGE);
				renderer->renderText("   [ESC]       to Exit", 400, 550, 1.1, ASGE::COLOURS::ORANGE);
				renderer->renderText("      [i]           for more info", 400, 600, 1.1, ASGE::COLOURS::ORANGE);
				renderer->renderText("[square brackets] to change difficulity", 250, 650, 0.9, ASGE::COLOURS::ORANGE);
				if (hardmode == false)
				{
					renderer->renderText("{--EASY                           ", 400, 700, 1.1, ASGE::COLOURS::WHITE);
					renderer->renderText("                           HARD--}", 400, 700, 0.9, ASGE::COLOURS::ORANGE);
				}
				else if (hardmode == true)	// "cool" effect for the easy/hard buttons...
				{
					renderer->renderText("{--EASY                           ", 400, 700, 0.9, ASGE::COLOURS::ORANGE);
					renderer->renderText("                           HARD--}", 400, 700, 1.1, ASGE::COLOURS::WHITE);
				}
			}
		}
		if (show_info == true)
		{
			if (game_action == GameAction::INFO)										// displays info screen
			{
				renderer->renderText("* Use W, A, S, D to move around", 100, 470, 0.7, ASGE::COLOURS::DARKGRAY);
				renderer->renderText("* Collect white dots to gain score (You win when you collect them all)", 100, 500, 0.7, ASGE::COLOURS::DARKGRAY);
				renderer->renderText("* Cherry gives you God-Mode! (speed and invulnerability for 5 seconds)", 100, 530, 0.7, ASGE::COLOURS::DARKGRAY);
				renderer->renderText("* Avoid ghosts (You loose life when they touch you)", 100, 560, 0.7, ASGE::COLOURS::DARKGRAY);
				renderer->renderText("* On hardmode there is an additional ghost and they move faster!", 100, 590, 0.7, ASGE::COLOURS::DARKGRAY);
				
				renderer->renderText("Press 'i' to go back to menu or 'Enter' to start playing", 100, 650, 0.7, ASGE::COLOURS::DARKGRAY);
			}
		}
}

/**
*   @brief   Processes any key inputs and translates them to a GameAction
*   @details This function is added as a callback to handle the game's 
			 input. Key presseses are translated in to GameActions which 
			 are then processed by the main game loop.
*   @param   key is the key the action relates to
*   @param   action whether the key was released or pressed
*   @return  void
*/
void PacManGame::input(int key, int action)
{
	//while (true)
	//{
	//	userinput.store(_getch());
	//}

	if (action == ASGE::KEYS::KEY_PRESSED)
	{
		if (key == ASGE::KEYS::KEY_ESCAPE)
		{
			game_action = GameAction::EXIT;
		}
		else if (key == ASGE::KEYS::KEY_ENTER && lives > 0 && score != total_collectables)
		{
			if (game_action == GameAction::PAUSE || game_action == GameAction::MENU)
			{
				prev_game_action = GameAction::PAUSE;
			}			
			game_action = GameAction::PLAY;
		}
		if (key == ASGE::KEYS::KEY_D)		// player directions corresponding to WASD inptu
		{
			player_direction = 1;			// Right
		}
		if (key == ASGE::KEYS::KEY_A)
		{
			player_direction = 2;			// Left
		}
		if (key == ASGE::KEYS::KEY_W)
		{
			player_direction = 3;			// Up
		}
		if (key == ASGE::KEYS::KEY_S)
		{
			player_direction = 4;			// Down
		}
		if (player_direction != 0)
		{
			sound_on = true;				// plays the classic sound if pacman is moving
		}
		else
		{
			sound_on = false;
		}
		if (key == ASGE::KEYS::KEY_P)		// pauses everything if user presses P key
		{
			if (game_action == GameAction::PLAY)
			{
				prev_game_action = GameAction::PLAY;
			}
			game_action = GameAction::PAUSE;
		}
		if (game_action == GameAction::MENU) // difficulity selection
		{
			if (key == ASGE::KEYS::KEY_LEFT_BRACKET)
			{
				hardmode = false;
				total_enemies = 3;
			}
			else if  (key == ASGE::KEYS::KEY_RIGHT_BRACKET)
			{
				hardmode = true;
				total_enemies = 4;
			}
		}
		if (game_action != GameAction::PLAY && game_action != GameAction::PAUSE)
		{
			if (key == ASGE::KEYS::KEY_I)	// more info is displayed if user presses I key
			{
				game_action = GameAction::INFO;
				if (show_info == false)
				{
					show_info = true;
				}
				else
				{
					show_info = false;
					game_action = GameAction::MENU;
				}
			}
		}
	}

}

/**
*   @brief   Processes the next game action
*   @details Uses the game action that was a direct result of 
*            user input. It allows input to processed in a different
             thread and the game actions performed in the main thread. 
*   @return  void
*/
void PacManGame::processGameActions()
{
	if (game_action == GameAction::EXIT)			// game shuts down
	{
		this->exit = true;
	}

	if (game_action == GameAction::PLAY)			// loops through game
	{
		engine1->setAllSoundsPaused(false);
		engine2->setAllSoundsPaused(false);
		if (cherry_col == true)
		{
			engine4->setAllSoundsPaused(false);
		}
	}

	if (game_action == GameAction::PAUSE || game_action == GameAction::MENU) // stops game if game is paused (or still in menu)
	{
		engine1->setAllSoundsPaused(true);
		engine2->setAllSoundsPaused(true);
		if ((game_action == GameAction::PAUSE && cherry_col == true )|| (game_action == GameAction::MENU && cherry_col == true))
		{
			engine4->setAllSoundsPaused(true);
		}
	}
	// I go into more depth regarding collisions in my documentation
		for (size_t j = 0; j < total_enemies; j++)		// PLAYER - ENEMY collision detection
		{
			if ((pacman->position[0] + pacman->size[0] >= enemy[j]->position[0])
				&& (pacman->position[0] <= enemy[j]->position[0] + enemy[j]->size[0]))
			{
				if ((pacman->position[1] + pacman->size[1] >= enemy[j]->position[1])
					&& (pacman->position[1] <= enemy[j]->position[1] + enemy[j]->size[1]))
				{
					if (cherry_col == false)
					{
						player_enemy_col = true;
					}
				}
			}
		}

		for (size_t i = 0; i < total_walls; i++)		// PLAYER - WALL collision detection
		{
			if ((pacman->position[0] + pacman->size[0] >= wall[i]->position[0])
				&& (pacman->position[0] <= wall[i]->position[0] + wall[i]->size[0]))
			{
				if ((pacman->position[1] + pacman->size[1] >= wall[i]->position[1])
					&& (pacman->position[1] <= wall[i]->position[1] + wall[i]->size[1]))
				{
					player_wall_col = true;
				}
			}
		}

		for (size_t j = 0; j < total_collectables; j++)	// PLAYER - COLLECTABLE collision detection
		{
			if ((pacman->position[0] + pacman->size[0] >= collectable[j]->position[0])
				&& (pacman->position[0] <= collectable[j]->position[0] + collectable[j]->size[0]))
			{
				if ((pacman->position[1] + pacman->size[1] >= collectable[j]->position[1])
					&& (pacman->position[1] <= collectable[j]->position[1] + collectable[j]->size[1]))
				{
					player_collectable_col = true;
					u = j;
				}
			}
		}

		for (size_t j = 0; j < total_collectables; j++)	// PLAYER - CHERRY (bonus collectable) collision detection
		{
			if ((pacman->position[0] + pacman->size[0] >= cherry->position[0])
				&& (pacman->position[0] <= cherry->position[0] + cherry->size[0]))
			{
				if ((pacman->position[1] + pacman->size[1] >= cherry->position[1])
					&& (pacman->position[1] <= cherry->position[1] + cherry->size[1]))
				{
					cherry_col = true;
				}
			}
		}

		for (size_t i = 0; i < total_walls; i++)
			for (size_t p = 0; p < total_enemies; p++)		// ENEMY - WALL collision detection
			{
				{
					if ((enemy[p]->position[0] + enemy[p]->size[0] >= wall[i]->position[0])
						&& (enemy[p]->position[0] <= wall[i]->position[0] + wall[i]->size[0]))
					{
						if ((enemy[p]->position[1] + enemy[p]->size[1] >= wall[i]->position[1])
							&& (enemy[p]->position[1] <= wall[i]->position[1] + wall[i]->size[1]))
						{
							enemy_wall_col[p] = true;
						}
					}
				}
			}

	//if (pacman->GetTag() == "Wall")
	//{
	//	GameAction::EXIT;
	//	this->exit = true;
	//}

	//game_action = GameAction::NONE;
}

