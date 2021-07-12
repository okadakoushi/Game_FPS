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
	/// キーコードの取得。
	/// </summary>
	/// <returns></returns>
	InputKeyCode& GetInputKeyCode()
	{
		return m_inputKeyCode;
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
	/// 物理ワールドを取得。
	/// </summary>
	/// <returns>物理ワールド。</returns>
	PhysicsWorld& GetPhysicsWorld() 
	{
		return m_physicsWorld;
	}
private:
	CStopwatch m_sw;								//ストップウォッチ。
	GraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
	PhysicsWorld m_physicsWorld;					//フィジックスワールド。		
	CEffectEngine m_effectEngine;					//エフェクトエンジン。
	ModelDataManager m_modelDataManager;			//モデルデーターマネジャー。						
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。
	InputKeyCode m_inputKeyCode;					//キーボード入力。
};

/// <summary>
/// エンジンのオブジェクトを取得。
/// </summary>
/// <returns></returns>
static inline TkEngine& EngineObj()
{
	return TkEngine::GetEngine();
}
/// <summary>
/// グラフィックスエンジン取得。
/// </summary>
/// <returns></returns>
static inline GraphicsEngine* GraphicsEngineObj()
{
	return EngineObj().GetGraphicsEngine();
}
/// <summary>
/// エフェクトエンジン取得。
/// </summary>
/// <returns></returns>
static inline CEffectEngine& EffectEngineObj()
{
	return EngineObj().GetEffectEngine();
}
/// <summary>
/// キーコード取得。
/// </summary>
/// <returns></returns>
static inline InputKeyCode& KeyCodeObj()
{
	return EngineObj().GetInputKeyCode();
}
/// <summary>
/// 物理ワールド取得。
/// </summary>
/// <returns></returns>
static inline PhysicsWorld& PhysicObj()
{
	return EngineObj().GetPhysicsWorld();
}



