#include "stdafx.h"
#include "PlayerDeathState.h"
#include "SrcFile/GameObj/GamePlayer.h"

void PlayerDeathState::Enter()
{
	m_player->GetRender()->PlayAnimation(GamePlayer::EnPlayerAnimation_Death, 0.5f);
}

void PlayerDeathState::Update()
{
}

void PlayerDeathState::Leave()
{
}
