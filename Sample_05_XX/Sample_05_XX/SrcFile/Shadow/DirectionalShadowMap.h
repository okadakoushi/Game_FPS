#pragma once
#include "SrcFile/util/Noncopyable.h"
#include "Model.h"
#include "RenderTarget.h"

/// <summary>
/// カスケードシャドウマップ。
/// </summary>
/// <code>
/// --func call step-- *引数は省略*
/// 1.Init();
/// 2.(if Needed)SetShadowAreas();
/// 3.RegisterShadowCaster();
/// 4.SkinModel::RegisterShadowReciever();
/// 5.Update();	todo : Engine::Update()
/// 6.RenderToShadowMap() TODO : Engine::Update(); SkinModelに作らないこと。複数回Draw呼ばれる。
/// </code>
class DirectionalShadowMap : Noncopyable
{
	struct SShadowCb;
public:
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="w">横の解像度。</param>
	/// <param name="h">縦の解像度。</param>
	/// <param name="lightHeight">太陽の高さ。</param>
	void Init(int w, int h, float lightHeight);
	/// <summary>
	/// 更新。
	/// </summary>
	void Update();
	/// <summary>
	/// シャドウマップを描画
	/// </summary>
	/// <remarks>
	/// こいつ呼び出すと自動的にシャドウマップ用のRenderTargetに変更する。
	/// 描画が終わり次第、元の描画していたフレームバッファに戻ります。
	/// todo:スキンモデルレンダーで勝手に呼ばれる処理にする。
	/// </remarks>
	/// <param name="rc"></param>
	void RenderToShadowMap();
	/// <summary>
	/// シャドウキャスターとして登録。
	/// <para>本関数を呼び出すと、シャドウ描画が自動的にONになります。</para>
	/// </summary>
	/// <param name="model">登録するモデル。</param>
	void RegisterShadowCaster(Model* model)
	{
		m_shadowCasters.push_back(model);
		//シャドウキャスタのフラグ。
		model->SetShadwoCaster();
		//シャドウだすで！
		m_isEnable = true;
	}
	/// <summary>
	/// レンダーターゲットを取得。
	/// todo : protected
	/// </summary>
	/// <param name="shadowNum">シャドウマップ。</param>
	/// <returns>レンダーターゲット。</returns>
	RenderTarget& GetRenderTarget(int shadowNum)
	{
		return m_shadowMaps[shadowNum];
	}
	ConstantBuffer& GetShadowCB() 
	{
		return m_shadowConstantBuffer;
	}
	/// <summary>
	/// シャドウの範囲を決定。
	/// <para>デフォルトはすべて500。</para>
	/// </summary>
	/// <param name="Area1">1枚目</param>
	/// <param name="Area2">2枚目</param>
	/// <param name="Area3">3枚目</param>
	void SetShadowAreas(float Area1, float Area2, float Area3)
	{
		m_shadowAreas[0] = Area1;
		m_shadowAreas[1] = Area2;
		m_shadowAreas[2] = Area3;
	}
	/// <summary>
	/// シャドウの描画範囲を取得。
	/// </summary>
	/// <param name="shadowNum">マップの番号。</param>
	/// <returns></returns>
	float GetShadowArea(int& shadowNum) const
	{
		return m_shadowAreas[shadowNum];
	}
	/// <summary>
	/// シャドウ描画する？
	/// </summary>
	/// <param name="flag">フラグ。</param>
	void SetShadowFlag(bool flag)
	{
		m_isEnable = flag;
	}
private://ユーザー側が使う必要のない関数。
	/// <summary>
	/// ライトの位置を計算。
	/// </summary>
	/// <remarks>
	/// メンバではない値を返却する関数のため、参照に変えないこと。
	/// </remarks>
	/// <param name="lightHeight">ライトカメラの高さ。</param>
	/// <param name="viewFrustomCenterPosition">視錐台の中心。</param>
	/// <returns>視錐台真ん中のライトカメラの位置。</returns>
	Vector3 CalcLightPosition(float lightHeight, Vector3 viewFrustomCenterPosition) const;
private:
	struct SShadowCb {
		Matrix mLVP[NUM_SHADOW_MAP];						//ライトビュープロジェクション。
		float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//カメラ空間での影を落とすエリアの深度テーブル。
	};
	RenderTarget m_shadowMaps[NUM_SHADOW_MAP];	//シャドウマップ。
	std::vector<Model*> m_shadowCasters;		//シャドウキャスター
	Matrix m_LVPMatrix[NUM_SHADOW_MAP];			//ライトビュープロジェクション行列。
	Matrix m_lightViewMatrix[NUM_SHADOW_MAP];	//ライトビュー行列
	Matrix m_lightProjMatirx[NUM_SHADOW_MAP];	//ライトプロジェクション行列
	SShadowCb m_shadowCBEntity;					//シャドウの値。
	ConstantBuffer m_shadowConstantBuffer;		//シャドウ用定数バッファ。
	Vector3 m_lightDirection = { 0.0f, -1.0f, 0.0f };	//ライトの方向。
	float m_lightHeight;						//ライトの高さ。
	bool m_isEnable = false;					//シャドウマップが有効？
	bool m_Inited[NUM_SHADOW_MAP] = {false};						//シャドウのリソースバリア呼ばれた？
	float m_shadowAreas[NUM_SHADOW_MAP] = { 500.0f,500.0f,500.0f };		//影の落ちる範囲。
};

