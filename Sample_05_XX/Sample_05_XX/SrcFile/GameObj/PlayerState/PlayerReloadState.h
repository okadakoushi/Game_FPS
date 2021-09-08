#pragma once

#include "IPlayerState.h"

class PlayerReloadState : public IPlayerState
{
public:
	PlayerReloadState(GamePlayer* player) :
		IPlayerState(player)
	{
		m_reloadSE = NewGO<SoundSource>(0);
		m_reloadSE->Init(L"Assets/Audio/Reload.wav");
	}

	~PlayerReloadState()
	{
		DeleteGO(m_reloadSE);
	}
	void Enter() override;
	void Update() override;
	void Leave() override;
private:
	SoundSource* m_reloadSE = nullptr;	//ÉäÉçÅ[ÉhÅB
};

