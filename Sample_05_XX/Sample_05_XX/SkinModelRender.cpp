#include "stdafx.h"
#include "SkinModelRender.h"

SkinModelRender::SkinModelRender()
{
}

SkinModelRender::~SkinModelRender()
{
}

bool SkinModelRender::Start()
{
	return true;
}

void SkinModelRender::Init(const char* filepath)
{
	m_model.InitModel(filepath);
}

void SkinModelRender::Update()
{
	m_model.UpdateWorldMatrix(m_pos, m_rot, m_scale);
}

void SkinModelRender::Draw()
{
	auto& rc = GraphicsEngineObj()->GetRenderContext();
	auto& camera3D = GraphicsEngineObj()->GetCamera3D();
	//ƒ‚ƒfƒ‹‚ÌDraw
	m_model.Draw(rc, camera3D.GetViewMatrix(), camera3D.GetProjectionMatrix(), m_renderMode);
}

