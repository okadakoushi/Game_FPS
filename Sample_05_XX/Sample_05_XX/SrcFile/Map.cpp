#include "stdafx.h"
#include "Map.h"
#include "SpriteRender.h"

Map::~Map()
{
}



bool Map::Start()
{
	m_miniMap.Create(
		FRAME_BUFFER_W, FRAME_BUFFER_H,
		1, 1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT,
		CLEARCOLOR
	);
	m_spriteRender = NewGO<SpriteRender>(EnPriority_UI);
	SpriteInitData initData;
	initData.m_textures[0] = &m_miniMap.GetRenderTargetTexture();
	initData.m_fxFilePath = "Assets/shader/sprite.fx";
	initData.m_height = m_WIDTH;
	initData.m_width = m_HEIGHT;
	m_spriteRender->Init(initData, false);

	//BoxUI_MAP
	m_uiBoxForMap = NewGO<SpriteRender>(EnPriority_UI);
	initData.m_ddsFilePath[0] = "Assets/sprite/MapUI_Box.dds";
	initData.m_width = 295.0f;
	initData.m_height = 290.0f;
	m_uiBoxForMap->Init(initData);
	m_uiBoxForMap->SetPos({ FRAME_BUFFER_W * -0.387f, FRAME_BUFFER_H * 0.275f, 0.0f });

	m_pos = { -495.0f, 200.0f, 0.0f };
	return true;
}

void Map::Update()
{
	SwichMiniMapSize();
	UpdateCamera();
	Draw();
	m_spriteRender->SetPos(m_pos);
	m_spriteRender->SetScale(m_scale);
}

void Map::SetDraw(const bool& isDraw)
{
	this->SetActive(isDraw);
	m_spriteRender->SetActive(isDraw);
}

void Map::UpdateCamera()
{
	//視点の位置計算。
	m_cameraPos = m_targetPos;
	m_cameraPos.y = m_targetPos.y + m_CAMERA_HEIGHT;
	//カメラ方向計算。
	Vector3 camDir = m_targetPos - m_cameraPos;
	if (camDir.Length() < 0.0001f) {
		MessageBoxA(nullptr, "ミニマップカメラの注視点と視点が近すぎます", "MiniMap::Camera", MB_OKCANCEL);
	}
	camDir.Normalize();
	Vector3 cameraUp;
	if (fabsf(camDir.y) > 0.99998f) {
		//ほぼ真上or真下。
		cameraUp = g_vec3AxisX;
	}
	else {
		//真上向いてないので
		cameraUp = g_vec3AxisY;
	}
	//カメラビュー計算。
	m_viewMatrix.MakeLookAt(
		m_cameraPos,
		m_targetPos,
		cameraUp
	);
	//プロジェも作成。
	m_projMatirx.MakeOrthoProjectionMatrix(
		m_currentWith_Height,
		m_currentWith_Height,
		0.1f,
		5000.0f
	);
}

void Map::Draw()
{
	GraphicsEngineObj()->GetRenderContext().WaitUntilToPossibleSetRenderTarget(m_miniMap);
	GraphicsEngineObj()->GetRenderContext().ClearRenderTargetView(m_miniMap.GetRTVCpuDescriptorHandle(), CLEARCOLOR);
	GraphicsEngineObj()->GetRenderContext().SetRenderTarget(m_miniMap, m_miniMap.GetDSVCpuDescriptorHandle());
	GraphicsEngineObj()->GetRenderContext().ClearDepthStencilView(m_miniMap.GetDSVCpuDescriptorHandle(), 1.0f);
	for (auto render : GraphicsEngineObj()->GetRenderingEngine().GetRenderList()) {
		if (render->isPlayAnim()) {
			render->GetModel().Draw(GraphicsEngineObj()->GetRenderContext(), m_viewMatrix, m_projMatirx, enRenderMode_Skin);
		}
		else {
			render->GetModel().Draw(GraphicsEngineObj()->GetRenderContext(), m_viewMatrix, m_projMatirx, enRenderMode_NonSkin);
		}
	}
	EngineObj().GetGraphicsEngine()->EndRender(false);
	EngineObj().GetGraphicsEngine()->BeginRender();
}

void Map::SwichMiniMapSize()
{

	if (KeyCodeObj().IsTriggerKey('M')) {
		if (m_isExpansion) {
			//拡大済みなのでマップを縮小する。
			m_pos = { -495.0f, 200.0f, 0.0f };
			m_scale = g_vec3One;
			m_currentWith_Height = m_SHRINK_WITH_HEIGHT;
			m_spriteRender->SetMulColor(m_COLOR);
			m_uiBoxForMap->SetPos({ FRAME_BUFFER_W * -0.39f, FRAME_BUFFER_H * 0.275f, 0.0f });
			m_uiBoxForMap->SetScale(g_vec3One);
			m_isExpansion = false;
		}
		else {
			//拡大済みではないのでマップを拡大する。
			m_pos = g_vec3Zero;
			m_scale.x = GraphicsEngineObj()->GetFrameBufferWidth() / m_WIDTH * 0.8f;
			m_scale.y = GraphicsEngineObj()->GetFrameBufferHeight() / m_HEIGHT * 0.8f;
			m_currentWith_Height = m_SHRINK_WITH_HEIGHT * 4;
			m_spriteRender->SetMulColor({ m_COLOR.r, m_COLOR.g, m_COLOR.b, m_COLOR.w * 0.8f });
			m_uiBoxForMap->SetScale({ 3.8f, 1.6f, 1.0f });
			m_uiBoxForMap->SetPos(g_vec3Zero);
			m_isExpansion = true;
		}
	}
}
