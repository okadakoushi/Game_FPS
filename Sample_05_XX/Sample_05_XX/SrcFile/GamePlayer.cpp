#include "stdafx.h"
#include "GamePlayer.h"

GamePlayer::~GamePlayer()
{
	DeleteGO(m_unityChan);
}

bool GamePlayer::Start()
{	
	//unitychan初期化。
	m_unityChan = NewGO<SkinModelRender>(EnPriority_3DModel, "Unity");
	m_unityChan->Init("Assets/modelData/unityChan.tkm", "Assets/animData/unityChan/Idle.tka");
	//シャドウキャスター。
	m_unityChan->SetShadwoCaster(true);
	//スキン描画。
	m_unityChan->SetRenderMode(enRenderMode_Skin);

	return true;
}

void GamePlayer::Update()
{
	//移動。
	if (GetAsyncKeyState('A')) {
		//m_pos.x++;
	}
	else if (GetAsyncKeyState('D')) {
		//m_pos.x--;
	}
	else if (GetAsyncKeyState('W')) {
		m_pos.z++;
	}
	else if (GetAsyncKeyState('S')) {
		m_pos.z--;
	}
	m_unityChan->SetPosition(m_pos);
}