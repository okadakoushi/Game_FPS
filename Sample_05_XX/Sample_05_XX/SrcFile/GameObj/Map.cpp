#include "stdafx.h"
#include "Map.h"

Map::~Map()
{
}

bool Map::Start()
{
	//init�B
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
			//�g��ς݂Ȃ̂Ń}�b�v���k������B
			m_pos = { -540, 260, 0 };
			m_scale = g_vec3One;
			m_isExpansion = false;
		}
		else {
			//�g��ς݂ł͂Ȃ��̂Ń}�b�v���g�傷��B
			m_pos = g_vec3Zero;
			m_scale.x = GraphicsEngineObj()->GetFrameBufferWidth() / m_WIDTH;
			m_scale.y = GraphicsEngineObj()->GetFrameBufferHeight() / m_HEIGHT;
			m_isExpansion = true;
		}
	}
	m_spriteRender->SetPos(m_pos);
	m_spriteRender->SetScale(m_scale);
}
