#pragma once

#include "HID/GamePad.h"

class GraphicsEngine;
class Camera;

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
	/// �X�V�����B
	/// </summary>
	void Update();
	/// <summary>
	/// �t���[���̏I�����ɌĂ΂�鏈���B
	/// </summary>
	void EndFrame();
	/// <summary>
	/// �Q�[���G���W���̏������B
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
	/// <summary>
	/// �G���W���̏I�������B
	/// </summary>
	void Final();
	/// <summary>
	/// �G���W���擾�B
	/// </summary>
	/// <returns></returns>
	static TkEngine& GetEngine()
	{
		static TkEngine instance;
		return instance;
	}
	/// <summary>
	/// �O���t�B�b�N�G���W���̎擾�B
	/// </summary>
	/// <returns></returns>
	GraphicsEngine* GetGraphicsEngine() const
	{
		return m_graphicsEngine;
	}
private:
	GraphicsEngine* m_graphicsEngine = nullptr;		//�O���t�B�b�N�G���W���B
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//�Q�[���p�b�h�B
};

/// <summary>
/// �G���W���̃I�u�W�F�N�g���擾�B
/// </summary>
/// <returns></returns>
static inline TkEngine& EngineObj()
{
	return TkEngine::GetEngine();
}

static inline GraphicsEngine* GraphicsEngineObj()
{
	return EngineObj().GetGraphicsEngine();
}


