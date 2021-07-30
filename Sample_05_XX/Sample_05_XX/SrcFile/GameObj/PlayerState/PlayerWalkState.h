#pragma once

#include "IPlayerState.h"

class PlayerWalkState : public IPlayerState
{
public:
	PlayerWalkState(GamePlayer* player) :
		IPlayerState(player)
	{
	}
	void Enter() override;
	void Update() override;
	void Leave() override;
private:
	const float MOVESPEED_WALK = 3.0f;
};

