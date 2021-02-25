#include "stdafx.h"
#include "tkEngine.h"
#include "GraphicsEngine.h"

TkEngine::TkEngine()
{
}

TkEngine::~TkEngine()
{
	m_effectEngine.Release();
}
void TkEngine::Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
	//グラフィックエンジンの初期化。
	m_graphicsEngine = new GraphicsEngine();
	m_graphicsEngine->Init(hwnd, frameBufferWidth, frameBufferHeight);
	//エフェクトエンジン初期化。
	m_effectEngine.Init();
	//ゲームパッドの初期化。
	for (int i = 0; i < GamePad::CONNECT_PAD_MAX; i++) {
		g_pad[i] = &m_pad[i];
	}
	m_naviMesh = new NaviMesh;
	m_naviMesh->Load("Assets/nvm/NavSample.nvm");
}
void TkEngine::BeginFrame()
{
	m_graphicsEngine->BeginRender();
	for (auto& pad : m_pad) {
		pad.BeginFrame();
		pad.Update();
	}
	
}
void TkEngine::Update()
{
	//GOMの初期化。
	gameObjectManager().Start();
	//GOMの更新。
	gameObjectManager().UpdateManager();
}

void TkEngine::EndFrame()
{
	m_graphicsEngine->EndRender();
}

void TkEngine::Final()
{
	if (m_graphicsEngine) {
		delete m_graphicsEngine;
	}
}