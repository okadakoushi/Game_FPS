#include "stdafx.h"
#include "SpriteRender.h"
#include "Sprite.h"

SpriteRender::~SpriteRender()
{
}

void SpriteRender::Init(const SpriteInitData& spriteData, bool isDraw3D)
{
	m_sprite.Init(spriteData);
	m_isDraw3D = isDraw3D;
	m_isInited = true;
}

bool SpriteRender::Start()
{
	return m_isInited;
}

void SpriteRender::Update()
{
	m_sprite.Update(m_pos, m_rot, m_scale, m_pivot);
}


void SpriteRender::ForwardRender()
{
	if (m_isDraw3D == true) {
		m_sprite.Draw(GraphicsEngineObj()->GetRenderContext());
	}
}

void SpriteRender::RenderHUD()
{
	if (m_isDraw3D == false) {
		m_sprite.Draw(GraphicsEngineObj()->GetRenderContext());
	}
}
