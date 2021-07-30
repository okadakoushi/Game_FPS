#include "stdafx.h"
#include "PlayerReloadState.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/GameObj/Rifle.h"

void PlayerReloadState::Enter()
{
}

void PlayerReloadState::Update()
{
	if (m_player->GetWepon()->GetRifleEvent() == Rifle::EnRifleEvent_Reloading) {
		m_player->GetWepon()->AddReloadTime();
	}

}

void PlayerReloadState::Leave()
{
}
