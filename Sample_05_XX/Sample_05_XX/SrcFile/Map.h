#pragma once

#include "SrcFile/Input/InputKeyCode.h"

class SpriteRender;

class Map : public IGameObject
{
public:
	~Map();
	bool Start();
	void Update() override;
	/// <summary>
	/// ミニマップ用のレンダーターゲットを取得。
	/// </summary>
	/// <returns></returns>
	RenderTarget& GetRenderTarget()
	{
		return m_miniMap;
	}
	/// <summary>
	/// ミニマップの中心とする座標。
	/// </summary>
	/// <param name="pos"></param>
	void SetTargetPos(const Vector3& pos)
	{
		m_targetPos = pos;
	}
	/// <summary>
	/// 描画するか。
	/// </summary>
	/// <param name="draw"></param>
	void SetDraw(const bool& isDraw);
private:
	/// <summary>
	/// カメラ更新。
	/// </summary>
	void UpdateCamera();
	/// <summary>
	/// ミニマップを描画。
	/// </summary>
	void Draw();
	/// <summary>
	/// ミニマップのサイズを切り替え。
	/// </summary>
	void SwichMiniMapSize();
private:
	const float m_CAMERA_HEIGHT = 1300.0f;		//カメラの高さ。
	const float m_SHRINK_WITH_HEIGHT = 1500.0f;	//縮小時のマップの横縦。
	const int m_WIDTH = 250.0f;					//スプライトの横。
	const int m_HEIGHT = 250.0f;				//スプライトの縦。
	const Vector4 m_COLOR = Vector4::White;		//乗算カラー。
	RenderTarget m_miniMap;						//miniマップ用RTV。
	Matrix m_viewMatrix;						//ビュー。
	Matrix m_projMatirx;						//プロジェ。
	Vector3 m_targetPos = g_vec3Zero;			//ミニマップの中心とするターゲットの座標。
	Vector3 m_cameraPos = g_vec3Zero;			//ミニマップカメラの位置	
	bool m_isExpansion = false;					//拡大されてる?
	float m_currentWith_Height = m_SHRINK_WITH_HEIGHT;	//現在のマップの横縦。
	//基本パラム。
	Vector3 m_pos = g_vec3Zero;
	Vector3 m_scale = g_vec3One;
	Vector2 m_pivot = g_vec2Zero;
	SpriteRender* m_spriteRender = nullptr;
	SpriteRender* m_uiBoxForMap = nullptr;		//Map用UIBOX。
};

