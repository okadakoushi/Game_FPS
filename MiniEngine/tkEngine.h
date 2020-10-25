#pragma once

#include "HID/GamePad.h"

class GraphicsEngine;

class TkEngine : Noncopyable{
public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~TkEngine();
	/// <summary>
	/// フレームの開始時に呼ばれる処理。
	/// </summary>
	void BeginFrame();
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
private:
	GraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。

	
};

/// <summary>
/// エンジンのオブジェクトを取得。
/// </summary>
/// <returns></returns>
static inline TkEngine& EngineObj()
{
	return TkEngine::GetEngine();
}
