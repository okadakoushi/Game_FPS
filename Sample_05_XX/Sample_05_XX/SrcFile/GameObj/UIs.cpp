#include "stdafx.h"
#include "UIs.h"
#include "Sprite.h"

void UIs::OnDestroy()
{
	DeleteGO(m_testSprite);
}

bool UIs::Start()
{
	//初期化データー。
	SpriteInitData testInitData;
	//ddsファイル初期化。
	testInitData.m_ddsFilePath[0] = "Assets/sprite/reticle.dds";
	testInitData.m_width = 100.0f;
	testInitData.m_height = 100.0f;
	testInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_testSprite = NewGO<SpriteRender>(EnPriority_UI);
	m_testSprite->Init(testInitData);
	return true;
}

