#pragma once

/// <summary>
/// カスケードシャドウマップ。
/// </summary>
class DirectionalShadowMap : Noncopyable
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="w"></param>
	/// <param name="h"></param>
	/// <param name="lightHeight"></param>
	void Init(int w, int h, float lightHeight);

	void RenderToShadowMap(RenderContext& rc);

private:
	struct SShadowCb {
		Matrix mLVP[NUM_SHADOW_MAP];						//ライトビュープロジェクション。
		float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//カメラ空間での影を落とすエリアの深度テーブル。
	};
	RenderTarget m_shadowMaps[NUM_SHADOW_MAP];	//シャドウマップ。
	std::vector<Model*> m_shadowCasters;		//シャドウキャスター
	Matrix m_LVPMatrix;							//ライトビュープロジェクション行列。
	SShadowCb m_shadowCBEntity;					//シャドウの値。
	ConstantBuffer m_shadowCB;					//シャドウの定数バッファ。
	Vector3 m_lightDirection;					//ライトの方向。
	float m_shadowAreas[NUM_SHADOW_MAP];		//影の落ちる範囲。
	float m_lightHeight;						//ライトの高さ。
	bool m_isEnable = false;					//シャドウマップが有効？
};

