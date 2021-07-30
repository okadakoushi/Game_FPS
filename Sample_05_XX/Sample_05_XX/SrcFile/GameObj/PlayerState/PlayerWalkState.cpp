#include "stdafx.h"
#include "PlayerWalkState.h"
#include "SrcFile/GameObj/GamePlayer.h"

void PlayerWalkState::Enter()
{
}

void PlayerWalkState::Update()
{
	m_player->GetRender()->PlayAnimation(GamePlayer::EnPlayerAnimation_Walk, 0.5f);
}

void PlayerWalkState::Leave()
{
}
