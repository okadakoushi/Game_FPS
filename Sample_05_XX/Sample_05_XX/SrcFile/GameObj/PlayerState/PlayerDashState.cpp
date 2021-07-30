#include "stdafx.h"
#include "PlayerDashState.h"
#include "SrcFile/GameObj/GamePlayer.h"

void PlayerDashState::Enter()
{
	m_player->GetRender()->PlayAnimation(GamePlayer::EnPlayerAnimation_Walk, 0.5f);
}

void PlayerDashState::Update()
{
}

void PlayerDashState::Leave()
{
}
