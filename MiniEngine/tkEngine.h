#pragma once

#include "HID/GamePad.h"

class GraphicsEngine;

class TkEngine : Noncopyable{
public:
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~TkEngine();
	/// <summary>
	/// �t���[���̊J�n���ɌĂ΂�鏈���B
	/// </summary>
	void BeginFrame();
	/// <summary>
	/// �t���[���̏I�����ɌĂ΂�鏈���B
	/// </summary>
	void EndFrame();
	/// <summary>
	/// �Q�[���G���W���̏������B
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
	/// <summary>
	/// �G���W���擾�B
	/// </summary>
	/// <returns></returns>
	static TkEngine& GetEngine()
	{
		static TkEngine instance;
		return instance;
	}
private:
	GraphicsEngine* m_graphicsEngine = nullptr;		//�O���t�B�b�N�G���W���B
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//�Q�[���p�b�h�B
	
};

/// <summary>
/// �G���W���̃I�u�W�F�N�g���擾�B
/// </summary>
/// <returns></returns>
static inline TkEngine& GetEngineObj()
{
	return TkEngine::GetEngine();
}

extern TkEngine* g_engine;	//TK�G���W���B