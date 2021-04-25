#include "stdafx.h"
#include "PlayerPoint.h"

PlayerPoint::PlayerPoint()
{
	//initÅB
	{
		SpriteInitData initData;
		initData.m_ddsFilePath[0] = "Assets/sprite/reticule.dds";
		initData.m_width = 100.0f;
		initData.m_height = 100.0f;
		initData.m_fxFilePath = "Assets/shader/sprite.fx";
		m_spriteRender = NewGO<SpriteRender>(EnPriority_UI);
		m_spriteRender->Init(initData);
	}
}

PlayerPoint::~PlayerPoint()
{
}

//void PlayerPoint::Update()
//{
//}
