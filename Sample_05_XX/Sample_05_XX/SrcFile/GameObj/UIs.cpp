#include "stdafx.h"
#include "UIs.h"
#include "Sprite.h"

void UIs::OnDestroy()
{
	DeleteGO(m_testSprite);
}

bool UIs::Start()
{
	//�������f�[�^�[�B
	SpriteInitData testInitData;
	//dds�t�@�C���������B
	testInitData.m_ddsFilePath[0] = "Assets/modelData/utc_all2.dds";
	testInitData.m_width = 500.0f;
	testInitData.m_height = 500.0f;
	testInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_testSprite = NewGO<SpriteRender>(EnPriority_UI);
	m_testSprite->Init(testInitData);
	return true;
}

