#include "Game.h"

Game::Game()
{
	_level = new Level();
	_level->setPositionZ(-6.f);
	_level->setRotationZ(30.f);
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