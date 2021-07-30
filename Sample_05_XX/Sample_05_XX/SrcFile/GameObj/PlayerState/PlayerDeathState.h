#pragma once

#include "IPlayerState.h"

class PlayerDeathState : public IPlayerState
{
public:
	PlayerDeathState(GamePlayer* player) :
		IPlayerState(player)
	{
	}
	void Enter() override;
	void Update() override;
	void Leave() override;
};

