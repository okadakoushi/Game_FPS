#include "stdafx.h"
#include "UIs.h"
#include "Sprite.h"

void UIs::OnDestroy()
{
}

bool UIs::Start()
{	
	SpriteInitData testInitData;
	//ddsファイル初期化。
	testInitData.m_ddsFilePath[0] = "Assets/sprite/reticle.dds";
	testInitData.m_width = 100.0f;
	testInitData.m_height = 100.0f;
	testInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_reticle.Init(testInitData);
	return true;
}

void UIs::Update()
{
	m_reticle.Update(g_vec3Zero,g_quatIdentity ,g_vec3Zero);
}

void UIs::RenderHUD()
{
	m_reticle.Draw(GraphicsEngineObj()->GetRenderContext());
}

