#include "stdafx.h"
#include "Map.h"

Map::~Map()
{
}

bool Map::Start()
{
	//init。
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
	if (KeyCodeObj().IsTriggerKey('M')) {
		if (m_isExpansion) {
			//拡大済みなのでマップを縮小する。
			m_pos = { -540, 260, 0 };
			m_scale = g_vec3One;
			m_isExpansion = false;
		}
		else {
			//拡大済みではないのでマップを拡大する。
			m_pos = g_vec3Zero;
			m_scale.x = GraphicsEngineObj()->GetFrameBufferWidth() / m_WIDTH;
			m_scale.y = GraphicsEngineObj()->GetFrameBufferHeight() / m_HEIGHT;
			m_isExpansion = true;
		}
	}
	m_spriteRender->SetPos(m_pos);
	m_spriteRender->SetScale(m_scale);
}
