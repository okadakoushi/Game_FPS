#pragma once

#include "IPlayerState.h"

class PlayerIdleState : public IPlayerState
{
public:
	PlayerIdleState(GamePlayer* player) :
		IPlayerState(player)
	{
	}
	void Enter() override;
	void Update() override;
	void Leave() override;
};

