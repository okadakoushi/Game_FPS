#pragma once

/// <summary>
/// マテリアル。
/// </summary>
class Material {
public:
	/// <summary>
	/// tkmファイルのマテリアル情報から初期化する。
	/// </summary>
	/// <param name="tkmMat">tkmマテリアル</param>
	void InitFromTkmMaterila(
		const TkmFile::SMaterial& tkmMat,
		const wchar_t* filePath, 
		const char* VSEntry, 
		const char* PSEntry
		);
	/// <summary>
	/// レンダリングを開始するときに呼び出す関数。
	/// </summary>
	/// <param name="rc">レンダリングコンテキスト</param>
	/// <param name="hasSkin">スキンがあるかどうかのフラグ</param>
	void BeginRender(RenderContext& rc, int renderMode);
	/// <summary>
	/// アルベドマップを設定。
	/// </summary>
	/// <param name="tex"></param>
	void SetAlbedoMap(Texture& tex)
	{
		m_albedoMap = tex;
	}
	/// <summary>
	/// アルベドマップを取得。
	/// </summary>
	/// <returns></returns>
	Texture& GetAlbedoMap()
	{
		return m_albedoMap;
	}
	/// <summary>
	/// 法線マップを取得。
	/// </summary>
	/// <returns></returns>
	Texture& GetNormalMap()
	{
		return m_normalMap;
	}
	/// <summary>
	/// スペキュラマップを取得。
	/// </summary>
	/// <returns></returns>
	Texture& GetSpecularMap()
	{
		return m_specularMap;
	}
	/// <summary>
	/// 定数バッファを取得。
	/// </summary>
	/// <returns></returns>
	ConstantBuffer& GetConstantBuffer()
	{
		return m_constantBuffer;
	}
	/// <summary>
	/// モデル用頂点シェーダーを設定。
	/// </summary>
	/// <param name="isAnim">アニメーションする？</param>
	/// <param name="shader">シェーダー。</param>
	void SetModelVS_Shader(bool& isAnim, Shader& shader)
	{
		if (isAnim) {
			m_vsSkinModel.SetShader(shader);
		}
		else {
			m_vsNonSkinModel.SetShader(shader);
		}
	}
	/// <summary>
	/// パイプラインステートの初期化。
	/// </summary>
	void InitPipelineState();
private:
	/// <summary>
	/// シェーダーの初期化。
	/// </summary>
	/// <param name="fxFilePath">fxファイルのファイルパス</param>
	/// <param name="vsEntryPointFunc">頂点シェーダーのエントリーポイントの関数名</param>
	/// <param name="psEntryPointFunc">ピクセルシェーダーのエントリーポイントの関数名</param>
	void InitShaders(const wchar_t* filePath, const char* VSEntry, const char* PSEntry);
	/// <summary>
	/// テクスチャを初期化。
	/// </summary>
	/// <param name="tkmMat"></param>
	void InitTexture(const TkmFile::SMaterial& tkmMat);
private:
	/// <summary>
	/// マテリアルパラメータ。
	/// </summary>
	struct SMaterialParam {
		int hasNormalMap;	//法線マップを保持しているかどうかのフラグ。
		int hasSpecMap;		//スペキュラマップを保持しているかどうかのフラグ。
	};
	Texture	m_albedoMap;							//アルベドマップ。
	Texture	m_normalMap;							//法線マップ。
	Texture	m_specularMap;							//スペキュラマップ。
	ConstantBuffer m_constantBuffer;				//定数バッファ。
	RootSignature m_rootSignature;					//ルートシグネチャ。
	PipelineState m_skinModelPipelineState;			//スキンありモデル用のパイプラインステート。(Not半透明。)
	PipelineState m_transSkinModelPipelineState;	//スキンありモデル用のパイプラインステート(半透明マテリアル)。
	PipelineState m_transNonSkinModelPipelineState;	//スキンなしモデル用のパイプラインステート(半透明マテリアル)。
	PipelineState m_NonSkinShadowMapDraw;			//スキンなしモデル用のシャドウ描画パイプラインステート。
	PipelineState m_SkinShadowMapDraw;				//スキンありモデル用のシャドウ描画パイプラインステート。
	Shader m_vsNonSkinModel;						//スキンなしモデル用の頂点シェーダー。
	Shader m_vsSkinModel;							//スキンありモデル用の頂点シェーダー。
	Shader m_psModel;								//モデル用のピクセルシェーダー。
	Shader m_vsNonSkinShadowDraw;					//スキンなしシャドウ描画用ピクセルシェーダー。
	Shader m_vsSkinShadowDraw;						//スキンありシャドウ描画用ピクセルシェーダー。
	Shader m_psSkinShadowDraw;						//シャドウ描画用ピクセルシェーダー。
};


