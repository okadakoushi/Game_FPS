#include "stdafx.h"
#include "Map.h"

Map::~Map()
{
}

bool Map::Start()
{
	//initÅB
	SpriteInitData initData;
	initData.m_ddsFilePath[0] = "Assets/sprite/map.dds";
	initData.m_width = 200.0f;
	initData.m_height = 200.0f;
	initData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_spriteRender = NewGO<SpriteRender>(EnPriority_UI);
	m_spriteRender->Init(initData);
	m_pos = { -540, 260, 0 };
	m_spriteRender->SetPos(m_pos);
	return true;
}

void Map::Update()
{
}
