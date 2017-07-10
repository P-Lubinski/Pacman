#include "Actions.h"

/**< Queued Game Action. The next action to be processed as determined by user input. */
std::atomic<GameAction> prev_game_action = GameAction::NONE;
std::atomic<GameAction> game_action = GameAction::MENU;
