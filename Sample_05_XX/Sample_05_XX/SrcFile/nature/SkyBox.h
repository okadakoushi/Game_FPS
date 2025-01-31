#pragma once
class SkyBox : public IGameObject
{
public:
	SkyBox();
	~SkyBox();

	void OnDestroy() override;
	void Init();
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
public:
	/// <summary>
	/// 拡大率を設定。
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(float scale) {
		m_scale = { scale, scale, scale };
		m_isDirty = true;
	}
	/// <summary>
	/// 空の位置決め。
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(Vector3& pos) {
		m_position = pos;
		m_isDirty = true;
	}
	/// <summary>
	/// スカイキューブのテクスチャパスを設定。
	/// </summary>
	/// <param name="path"></param>
	void SetSkyCubeTexturePath(const std::wstring& path) 
	{
		m_skyCubeMapFilePath = path;
	}
	/// <summary>
	/// スカイボックスの自己発光を設定。
	/// </summary>
	/// <param name="itensity"></param>
	void SetSelfLuminous(const float& itensity)
	{
		m_skyParams.selfLuminous = itensity;
	}
private:
	struct SkyBoxParams {
		float selfLuminous = 1.0f;	//自己発光。
	};
	SkinModelRender* m_modelRender = nullptr;			//モデルレンダー。
	Texture m_skyCube;									//スカイキューブ。
	Shader m_psShader;									//スカイ用のシェーダー。
	Vector3 m_scale = { 8000.0f, 8000.0f, 8000.0f };	//空の拡大。
	Vector3 m_position = Vector3::Zero;					//位置。
	Vector3 m_emissionColor = Vector3::Zero;			//自己発光。
	bool m_isDirty = false;								//ダーティフラグ。
	std::wstring m_skyCubeMapFilePath = L"Assets/modelData/nature/skyCubeMapNight_Toon_02.dds";		//空マップのファイルパス。
	const char* m_skyModelFilePath = "Assets/modelData/nature/skyBox.tkm";				//空モデルのファイルパス
	SkyBoxParams m_skyParams;
};

