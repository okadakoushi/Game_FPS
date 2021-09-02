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
	m_sprite.SetDraw3DFlag(isDraw3D);
	if (isDraw3D) {
		m_view = GraphicsEngineObj()->GetCamera3D().GetViewMatrix();
		m_proj = GraphicsEngineObj()->GetCamera3D().GetProjectionMatrix();
	}
	else {
		m_view = GraphicsEngineObj()->GetCamera2D().GetViewMatrix();
		m_proj = GraphicsEngineObj()->GetCamera2D().GetProjectionMatrix();
	}

	m_isInited = true;
}

bool SpriteRender::Start()
{
	return m_isInited;
}

void SpriteRender::Update()
{
	if (m_isDraw3D) {
		m_view = GraphicsEngineObj()->GetCamera3D().GetViewMatrix();
		m_proj = GraphicsEngineObj()->GetCamera3D().GetProjectionMatrix();
	}
	m_sprite.Update(m_pos, m_rot, m_scale, m_pivot, true);
}


void SpriteRender::ForwardRender()
{
	if (m_isDraw3D == true) {
		m_sprite.Draw(GraphicsEngineObj()->GetRenderContext(), m_view, m_proj);
	}
}

void SpriteRender::RenderHUD()
{
	if (m_isDraw3D == false) {
		m_sprite.Draw(GraphicsEngineObj()->GetRenderContext(), m_view, m_proj);
	}
}
