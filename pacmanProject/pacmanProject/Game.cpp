#include "Game.h"

Game::Game()
{
	_level = new Level();
	addChild(_level);
}

Game::~Game()
{
	if (_level) {
		delete _level;
	}
}

void Game::render()
{
	Object::render();
}