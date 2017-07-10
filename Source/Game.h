#pragma once
#include <Engine/OGLGame.h>
#include <string>
#include <thread>

struct GameFont;
class Enemy;											// Decleration for Enemy class
class Player;											// Decleration for Player class

/**
*  Invaders Game. An OpenGL Game based on ASGE.
*/

namespace irrklang
{
	class ISoundEngine;									// Decleration for irrklang sound engines
}

class PacManGame:
	public ASGE::OGLGame								// Open GL
{
public:
	PacManGame();										// Constructor
	~PacManGame();										// Deconstructor

	// Inherited via Game
	virtual bool run() override;						// Loops through renderer and processes game actions untill exit is triggered
	bool shouldExit() const;							// Ff this is true - game closes
	void render();										// Draws frames

	// Inherited via OGLGame
	virtual bool initAudio();							// Initializes sound engines
	virtual bool init();								// Initializes all objects
	virtual void drawFrame();							// Renders all objects per tick

//	void SetTag(std::string _tag) { m_tag = _tag; }  // was susspoe to make detecting wall collision easier
//													 // if it worked, a lot of code could be simplified
//	std::string GetTag() { return m_tag; }			 // gave up on this as I needed to focus on other core elements
//
//protected:
//	std::string m_tag;

	bool sound_on = false;								// Used to pause sound engine for pacman
	bool ghost_sound = false;							// Used to pause sound engine for ghosts

	bool player_wall_col = false;						// Toggles if player (pacman) collided with a wall
	bool player_enemy_col = false;						// Toggles if player collided with an enemy (ghost)
	bool player_collectable_col = false;				// Toggles if player collided with a collectable
	bool cherry_col = false;							// Toggles if player collided with the bonus cherry collectable
	bool enemy_wall_col[4] = 
	{ false, false, false, false };						// Toggles flag for collision with walls for individual enemies

	int player_direction = 0;							// Tracks direction of players movement
	int enemy_direction[3] = { 0, 0, 0 };				// Tracks direction of enemy movement 
	int prev_enemy_direction[3] = { 0, 0, 0 };			// Used to prevent enemy head smashing into same wall indefinitely

	bool hardmode = false;								// Changes couple of things in the game when enabled (explained in info)
	bool show_info = false;								// Displayes info in menu screen (gives user instructions / tips)

	//std::atomic<char> userinput;						// Threaded user input (to do)
private:
	void processGameActions();		//
	void input(int key, int action);

	int  callback_id = -1;								// Input Callback ID. The callback ID assigned by the game engine
	bool exit = false;									// Exit boolean. If true the game loop will exit
	std::unique_ptr<ASGE::Sprite> sprite = nullptr;		// Background image for Main Menu
	std::unique_ptr<ASGE::Sprite> pacman = nullptr;		// Pacman sprite
	std::unique_ptr<ASGE::Sprite> enemy[4];				// Array of Ghost sprites
	std::unique_ptr<ASGE::Sprite> wall[55];				// Array of Wall tiles
	std::unique_ptr<ASGE::Sprite> collectable[36];		// Array of collectable sprites
	std::unique_ptr<ASGE::Sprite> cherry = nullptr;		// Bonus cherry collectable sprite

	int pacman_speed = 2;								// The speed at which player moves around the level (can be briefly increased upon collecting the cherry)
	int enemy_speed = 2;								// The default speed of Ghosts (it's higher on hard-mode)
	int total_enemies = 4;								// Total amount of Ghosts
	int total_walls = 55;								// Total amount of Walls
	int total_collectables = 36;						// Total amount of Enemies
	int score = 0;										// Score tracker
	int lives = 3;										// Live tracker
	int s = 0;											// Counter used for pacman animation
	int w = 0;											// Counter used to give pacman God-Mode for 5 seconds (if FPS is locked at 60)
	int u = 0;											// Counter used for couple of things to do with collectables (it needs tweaking)
	int e = 0;											// Counter used to trick program to play a sound just once
														 /*(Yes, I know these could have more meaningful names, but they are just simple counters that do barely anything
															and often only on a single line of code / or a very small section)*/
	std::unique_ptr<irrklang::ISoundEngine> engine1 = nullptr;
	std::unique_ptr<irrklang::ISoundEngine> engine2 = nullptr;
	std::unique_ptr<irrklang::ISoundEngine> engine3 = nullptr;
	std::unique_ptr<irrklang::ISoundEngine> engine4 = nullptr;
														/* 4 sound engines used to play different sound effects
															I needed to seperate them because some sounds sometimes need pausing*/
};

