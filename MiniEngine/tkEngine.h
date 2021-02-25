#pragma once

#include "HID/GamePad.h"
#include "SrcFile/ModelDataManager.h"
#include "SrcFile/Effect/CEffectEngine.h";
//#include "SrcFile/NaviMesh/NaviMesh.h"

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
	/// ���f���f�[�^�[�}�l�[�W���[���擾�B
	/// </summary>
	/// <returns></returns>
	ModelDataManager& GetModelDataManager() 
	{
		return m_modelDataManager;
	}
	/// <summary>
	/// �i�r���b�V�����擾�B
	/// </summary>
	/// <returns></returns>
	NaviMesh* GetNaviMesh()
	{
		return m_naviMesh;
	}
private:
	GraphicsEngine* m_graphicsEngine = nullptr;		//�O���t�B�b�N�G���W���B
	CEffectEngine m_effectEngine;					//�G�t�F�N�g�G���W���B
	ModelDataManager m_modelDataManager;			//���f���f�[�^�[�}�l�W���[�B						
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//�Q�[���p�b�h�B
	NaviMesh* m_naviMesh = nullptr;							//�i�r���b�V���B
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

static inline CEffectEngine& EffectEngineObj()
{
	return EngineObj().GetEffectEngine();
}

static inline NaviMesh* NaviMeshObj()
{
	return EngineObj().GetNaviMesh();
}



