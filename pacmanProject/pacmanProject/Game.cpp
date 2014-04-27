#include "Game.h"

const float Game::LEVEL_Z_DEPTH = -50.f;

Game::Game()
{
	_level = new Level();
	_level->setPositionX(100.f);
	_level->setPositionY(100.f);
//	_level->setPositionZ(LEVEL_Z_DEPTH);
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