#pragma once

#include "IPlayerState.h"

class PlayerDashState : public IPlayerState
{
public:
	PlayerDashState(GamePlayer* player) :
		IPlayerState(player)
	{
	}
	void Enter() override;
	void Update() override;
	void Leave() override;
private:
	const float MOVESPEED_DASH = 5.0f;
};

