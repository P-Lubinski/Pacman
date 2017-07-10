//#include "Enemy.h"
//#include <Engine/Sprite.h>
//#include "Actions.h"
//
//Enemy::Enemy()
//{
//}
//
//Enemy::~Enemy()
//{
//}
//
//bool Enemy::init()
//{
//	for (size_t j = 0; j < total_enemies; j++)
//	{
//		enemy[j] = renderer->createSprite();
//		if (j == 0)
//		{
//			enemy[j]->position[0] = 220;
//			enemy[j]->position[1] = 120;
//			enemy_direction[j] = 1;
//			if (!enemy[j]->loadTexture("..\\..\\Resources\\Textures\\inky.png"))
//			{
//				return false;
//			}
//		}
//		if (j == 1)
//		{
//			enemy[j]->position[0] = 1120;
//			enemy[j]->position[1] = 120;
//			enemy_direction[j] = 4;
//			if (!enemy[j]->loadTexture("..\\..\\Resources\\Textures\\pinky.png"))
//			{
//				return false;
//			}
//		}
//		if (j == 2)
//		{
//			enemy[j]->position[0] = 420;
//			enemy[j]->position[1] = 320;
//			enemy_direction[j] = 4;
//			if (!enemy[j]->loadTexture("..\\..\\Resources\\Textures\\blinky.png"))
//			{
//				return false;
//			}
//		}
//		if (j == 3)
//		{
//			enemy[j]->position[0] = 620;
//			enemy[j]->position[1] = 520;
//			enemy_direction[j] = 2;
//			if (!enemy[j]->loadTexture("..\\..\\Resources\\Textures\\clyde.png"))
//			{
//				return false;
//			}
//		}
//		enemy[j]->size[0] = 60;
//		enemy[j]->size[1] = 60;
//	}
//	return true;
//}
//
//void Enemy::drawFrame()
//{
//	if (hardmode == false)
//	{
//		total_enemies = 3;
//	}
//	else if (hardmode == true)
//	{
//		enemy_speed = 2.5;
//	}
//	for (size_t j = 0; j < total_enemies; j++)
//	{
//		if (game_action == GameAction::PAUSE || game_action == GameAction::MENU)
//		{
//			enemy_direction[j] = 0;
//		}
//		if (prev_game_action == GameAction::PAUSE && game_action == GameAction::PLAY)
//		{
//			for (size_t j = 0; j < total_enemies; j++)
//			{
//				enemy_direction[j] = prev_enemy_direction[j];
//				if (enemy_direction[j] == 0)
//				{
//					enemy_direction[j] ++;
//				}
//			}
//			prev_game_action = GameAction::NONE;
//		}
//		if (enemy_wall_col[j] == false)
//		{
//			if (enemy_direction[j] == 1)
//			{
//				enemy[j]->position[0] += enemy_speed;
//			}
//			else if (enemy_direction[j] == 2)
//			{
//				enemy[j]->position[0] -= enemy_speed;
//			}
//			else if (enemy_direction[j] == 3)
//			{
//				enemy[j]->position[1] -= enemy_speed;
//			}
//			else if (enemy_direction[j] == 4)
//			{
//				enemy[j]->position[1] += enemy_speed;
//			}
//			else
//			{
//				enemy[j]->position[0] = enemy[j]->position[0];
//				enemy[j]->position[1] = enemy[j]->position[1];
//			}
//		}
//		else if (enemy_wall_col[j] == true)
//		{
//			int randNum = rand() % 4 + 1;
//			prev_enemy_direction[j] = enemy_direction[j];
//			if (enemy_direction[j] == 1)
//			{
//				enemy[j]->position[0] = enemy[j]->position[0] - 5;
//			}
//			else if (enemy_direction[j] == 2)
//			{
//				enemy[j]->position[0] = enemy[j]->position[0] + 5;
//			}
//			else if (enemy_direction[j] == 3)
//			{
//				enemy[j]->position[1] = enemy[j]->position[1] + 5;
//			}
//			else if (enemy_direction[j] == 4)
//			{
//				enemy[j]->position[1] = enemy[j]->position[1] - 5;
//			}
//			enemy_direction[j] = randNum;
//			enemy_wall_col[j] = false;
//		}
//		if (enemy_direction[j] == prev_enemy_direction[j])
//		{
//			enemy_direction[j] = +2;
//			if (enemy_direction[j] > 4)
//			{
//				enemy_direction[j] = 1;
//			}
//		}
//		enemy[j]->render(renderer);
//	}
//}
