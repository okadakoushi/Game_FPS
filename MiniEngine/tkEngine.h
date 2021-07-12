#pragma once

#include "HID/GamePad.h"
#include "SrcFile/ModelDataManager.h"
#include "SrcFile/Effect/CEffectEngine.h";
#include "SrcFile/Timer/tkStopwatch.h"
#include "SrcFile/Physics/PhysicsWorld.h"
#include "SrcFile/InputKeyCode.h"

class GraphicsEngine;
class Camera;
class NaviMesh;

class TkEngine : Noncopyable{
public:
	TkEngine();
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
	/// <summary>
	///	�G�t�F�N�g�G���W���擾�B
	/// </summary>
	/// <returns></returns>
	CEffectEngine& GetEffectEngine()
	{
		return m_effectEngine;
	}
	/// <summary>
	/// �L�[�R�[�h�̎擾�B
	/// </summary>
	/// <returns></returns>
	InputKeyCode& GetInputKeyCode()
	{
		return m_inputKeyCode;
	}
	/// <summary>
	/// ���f���f�[�^�[�}�l�[�W���[���擾�B
	/// </summary>
	/// <returns></returns>
	ModelDataManager& GetModelDataManager() 
	{
		return m_modelDataManager;
	}
	/// <summary>
	/// �������[���h���擾�B
	/// </summary>
	/// <returns>�������[���h�B</returns>
	PhysicsWorld& GetPhysicsWorld() 
	{
		return m_physicsWorld;
	}
private:
	CStopwatch m_sw;								//�X�g�b�v�E�H�b�`�B
	GraphicsEngine* m_graphicsEngine = nullptr;		//�O���t�B�b�N�G���W���B
	PhysicsWorld m_physicsWorld;					//�t�B�W�b�N�X���[���h�B		
	CEffectEngine m_effectEngine;					//�G�t�F�N�g�G���W���B
	ModelDataManager m_modelDataManager;			//���f���f�[�^�[�}�l�W���[�B						
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//�Q�[���p�b�h�B
	InputKeyCode m_inputKeyCode;					//�L�[�{�[�h���́B
};

/// <summary>
/// �G���W���̃I�u�W�F�N�g���擾�B
/// </summary>
/// <returns></returns>
static inline TkEngine& EngineObj()
{
	return TkEngine::GetEngine();
}
/// <summary>
/// �O���t�B�b�N�X�G���W���擾�B
/// </summary>
/// <returns></returns>
static inline GraphicsEngine* GraphicsEngineObj()
{
	return EngineObj().GetGraphicsEngine();
}
/// <summary>
/// �G�t�F�N�g�G���W���擾�B
/// </summary>
/// <returns></returns>
static inline CEffectEngine& EffectEngineObj()
{
	return EngineObj().GetEffectEngine();
}
/// <summary>
/// �L�[�R�[�h�擾�B
/// </summary>
/// <returns></returns>
static inline InputKeyCode& KeyCodeObj()
{
	return EngineObj().GetInputKeyCode();
}
/// <summary>
/// �������[���h�擾�B
/// </summary>
/// <returns></returns>
static inline PhysicsWorld& PhysicObj()
{
	return EngineObj().GetPhysicsWorld();
}



