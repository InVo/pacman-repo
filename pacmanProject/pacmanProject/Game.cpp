#include "Game.h"

Game::Game()
{
	_level = new Level();
	_level->setPositionX(1.f);
	_level->setPositionZ(-6.f);
	Level* level = new Level();
	level->setPositionX(-1.f);
	level->setPositionZ(-7.f);
	addChild(_level);
	addChild(level);
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