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
	//�O���t�B�b�N�G���W���̏������B
	m_graphicsEngine = new GraphicsEngine();
	m_graphicsEngine->Init(hwnd, frameBufferWidth, frameBufferHeight);
	//�������[���h�������B
	m_physicsWorld.Init();
	//�G�t�F�N�g�G���W���������B
	m_effectEngine.Init();
	//�Q�[���p�b�h�̏������B
	for (int i = 0; i < GamePad::CONNECT_PAD_MAX; i++) {
		g_pad[i] = &m_pad[i];
	}
	//�L�[�R�[�h�̏������B
	m_inputKeyCode.Init();
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
	//�X�^�[�g�B
	m_sw.Start();
	//�������[���h�X�V�B
	m_physicsWorld.Update();
	//�L�[�R�[�h�̍X�V�B
	m_inputKeyCode.Update();
	//GOM�̏������B
	gameObjectManager().Start();
	//GOM�̍X�V�B
	gameObjectManager().UpdateManager();
	//�`��I���B
	EndFrame();
	//�X�g�b�v�B
	m_sw.Stop();
	GameTime().PushFrameDeltaTime((float)m_sw.GetElapsed());
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