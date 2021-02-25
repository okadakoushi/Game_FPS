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
	/// デストラクタ。
	/// </summary>
	~TkEngine();
	/// <summary>
	/// フレームの開始時に呼ばれる処理。
	/// </summary>
	void BeginFrame();
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();
	/// <summary>
	/// フレームの終了時に呼ばれる処理。
	/// </summary>
	void EndFrame();
	/// <summary>
	/// ゲームエンジンの初期化。
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
	/// <summary>
	/// エンジンの終了処理。
	/// </summary>
	void Final();
	/// <summary>
	/// エンジン取得。
	/// </summary>
	/// <returns></returns>
	static TkEngine& GetEngine()
	{
		static TkEngine instance;
		return instance;
	}
	/// <summary>
	/// グラフィックエンジンの取得。
	/// </summary>
	/// <returns></returns>
	GraphicsEngine* GetGraphicsEngine() const
	{
		return m_graphicsEngine;
	}
	/// <summary>
	///	エフェクトエンジン取得。
	/// </summary>
	/// <returns></returns>
	CEffectEngine& GetEffectEngine()
	{
		return m_effectEngine;
	}
	/// <summary>
	/// モデルデーターマネージャーを取得。
	/// </summary>
	/// <returns></returns>
	ModelDataManager& GetModelDataManager() 
	{
		return m_modelDataManager;
	}
	/// <summary>
	/// ナビメッシュを取得。
	/// </summary>
	/// <returns></returns>
	NaviMesh* GetNaviMesh()
	{
		return m_naviMesh;
	}
private:
	GraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
	CEffectEngine m_effectEngine;					//エフェクトエンジン。
	ModelDataManager m_modelDataManager;			//モデルデーターマネジャー。						
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。
	NaviMesh* m_naviMesh = nullptr;							//ナビメッシュ。
};

/// <summary>
/// エンジンのオブジェクトを取得。
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



