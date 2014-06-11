#include <QtCore>

#include "PlayerAction.h"
#include "InputAction.h"
#include "InputConfig.h"

using namespace Zk::Game;

InputConfig::InputConfig()
{
	for (int i = 0; i < (int)PlayerAction::NUM_ACTIONS; i++)
		playerToInput[(PlayerAction)i] = InputAction();
}

QDataStream & Zk::Game::operator<<(QDataStream & ds, const InputConfig & ic)
{
	for (int i = 0; i < (int)PlayerAction::NUM_ACTIONS; i++)
	{
		auto it = ic.playerToInput.find((PlayerAction)i);
		ds << it->second;
	}
	
	return ds;
}

QDataStream & Zk::Game::operator>>(QDataStream & ds, InputConfig & ic)
{
	for (int i = 0; i < (int)PlayerAction::NUM_ACTIONS; i++)
	{
		InputAction ia;
		ds >> ia;
		ic.playerToInput[(PlayerAction)i] = ia;
	}
	
	return ds;
}