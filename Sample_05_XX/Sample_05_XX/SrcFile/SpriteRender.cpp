#include "stdafx.h"
#include "SpriteRender.h"
#include "Sprite.h"

SpriteRender::~SpriteRender()
{
}

void SpriteRender::Init(const SpriteInitData& spriteData)
{
	m_sprite.Init(spriteData);
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

void SpriteRender::RenderHUD()
{
	m_sprite.Draw(GraphicsEngineObj()->GetRenderContext());
}
