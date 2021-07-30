#include "stdafx.h"
#include "PlayerIdleState.h"
#include "SrcFile/GameObj/GamePlayer.h"

void PlayerIdleState::Enter()
{
}

void PlayerIdleState::Update()
{
	m_player->GetRender()->PlayAnimation(GamePlayer::EnPlayerAnimation_Idle, 0.5f);
}

void PlayerIdleState::Leave()
{
}
