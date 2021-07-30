#pragma once

#include "IPlayerState.h"

class PlayerReloadState : public IPlayerState
{
public:
	PlayerReloadState(GamePlayer* player) :
		IPlayerState(player)
	{
	}
	void Enter() override;
	void Update() override;
	void Leave() override;
};

