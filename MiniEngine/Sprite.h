#pragma once

//#include "Indexbuffer.h"
//#include "VertexBuffer.h"
//#include "ConstantBuffer.h"
//#include "DescriptorHeap.h"

class Texture;

//スプライトに設定できる最大テクスチャ数。
const int MAX_TEXTURE = 16;	
//拡張SRVが設定されるレジスタの開始番号。
const int EXPAND_SRV_REG__START_NO = 10;
//ユーザー拡張コンスタントバッファー設定用のレジスタの開始番号。
const int EXPAND_SET_CONSTANT_BUFFER_START_NO = 10;

class IShaderResource;

/// <summary>
/// アルファブレンディングモード
/// </summary>
enum AlphaBlendMode {
	AlphaBlendMode_None,	//アルファブレンディングなし(上書き)。
	AlphaBlendMode_Trans,	//半透明合成
	AlphaBlendMode_Add,		//加算合成
	AlphaBlendMode_Multiply	//乗算合成
};

template< class TExpandData > struct SpriteExpandDataInfo {
	TExpandData* m_expandData = nullptr;
	int m_expandDataSize = 0;
};
/// <summary>
/// スプライトの初期化データ。
/// </summary>
struct SpriteInitData {
	const char* m_ddsFilePath[MAX_TEXTURE]= {nullptr};		//DDSファイルのファイルパス。
	Texture* m_textures[MAX_TEXTURE] = { nullptr };			//使用するテクスチャ。DDSファイルのパスが指定されている場合は、このパラメータは無視されます。
	const char* m_vsEntryPointFunc = "VSMain";				//頂点シェーダーのエントリーポイント。
	const char* m_psEntryPoinFunc = "PSMain";				//ピクセルシェーダーのエントリーポイント。
	const char* m_fxFilePath = nullptr;						//.fxファイルのファイルパス。
	UINT m_width = 0;										//スプライトの幅。
	UINT m_height = 0;										//スプライトの高さ。
	void* m_expandConstantBuffer = nullptr;					//ユーザー拡張の定数バッファ
	int m_expandConstantBufferSize = 0;						//ユーザー拡張の定数バッファのサイズ。
	IShaderResource* m_expandShaderResoruceView = nullptr;	//ユーザー拡張のシェーダーリソース。
	AlphaBlendMode m_alphaBlendMode = AlphaBlendMode_Trans;	//アルファブレンディングモード。
	std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT> m_colorBufferFormat = {
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
	};	//レンダリングするカラーバッファのフォーマット。
	UINT8 renderTargetMask = D3D12_COLOR_WRITE_ENABLE_ALL;
};
/// <summary>
/// スプライトクラス。
/// </summary>
class Sprite  {
public:
	static const Vector2	DEFAULT_PIVOT;					//!<ピボット。
	virtual ~Sprite();
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="initData">初期化データ</param>
	void Init(const SpriteInitData& initData);
	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pivot">
	/// <param name="isDraw3D">3D描画？</param>
	/// ピボット
	/// 0.5, 0.5で画像の中心が基点。
	/// 0.0, 0.0で画像の左下。
	/// 1.0, 1.0で画像の右上。
	/// UnityのuGUIに準拠。
	/// </param>
	void Update(const Vector3& pos, const Quaternion& rot, const Vector3& scale, const Vector2& pivot = DEFAULT_PIVOT, const bool& isDraw3D = false);
	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="renderContext"></param>
	/// <param name="view"></param>
	/// <param name="proj"></param>
	void Draw(RenderContext& renderContext, const Matrix& view, const Matrix& proj);
	/// <summary>
	/// 乗算カラーを設定。
	/// </summary>
	/// <param name="color"></param>
	void SetMulColor(const Vector4& color)
	{
		m_constantBufferCPU.mulColor = color;
	}
	/// <summary>
	/// テクスチャを取得。
	/// </summary>
	/// <param name="index">テクスチャの添え字。</param>
	/// <returns></returns>
	Texture* GetTexture(int index)
	{
		return m_textureExternal[index];
	}
	/// <summary>
	/// ユーザー設定型定数バッファを設定。
	/// </summary>
	/// <param name="entity"></param>
	void SetUserSetConstantBufferEntity(void* entity)
	{
		m_userSetConstantBufferCPU = entity;
	}
	/// <summary>
	/// 外部テクスチャを設定。FromUser。
	/// </summary>
	/// <param name="tex">テクスチャ</param>
	/// <param name="registNumber">登録レジスタ。</param>
	void SetExternalTexture(Texture& tex, int registNumber)
	{
		//外部登録されるテクスチャを設定。
		m_descriptorHeap.RegistShaderResource(registNumber, tex);
		//変更を確定。
		m_descriptorHeap.Commit();
	}
	/// <summary>
	/// シェーダーリソースの登録を解除。
	/// </summary>
	/// <param name="registNumber"></param>
	void UnRegistSRV(int registNumber)
	{
		//レジストを外す。
		m_descriptorHeap.UnRegistShaderResource(registNumber);
		//変更を確定。
		m_descriptorHeap.Commit();
	}
	/// <summary>
	/// IBLの輝度を設定。
	/// </summary>
	void SetIBLItensity(const float& itensity)
	{
		m_constantBufferCPU.lightIntensity = itensity;
	}
	/// <summary>
	/// テクスチャの幅を取得。
	/// </summary>
	/// <param name="texNo"></param>
	/// <returns></returns>
	int GetTextureWidth(int texNo) const
	{
		return m_textures[texNo].GetWidth();
	}
	/// <summary>
	/// テクスチャの幅を取得。
	/// </summary>
	/// <param name="texNo"></param>
	/// <returns></returns>
	int GetTextureHeight(int texNo) const
	{
		return m_textures[texNo].GetHeight();
	}
	/// <summary>
	/// 3D描画するか。
	/// </summary>
	/// <param name="flag"></param>
	void SetDraw3DFlag(bool& flag)
	{
		m_isDraw3D = flag;
	}
private:
	/// <summary>
	/// テクスチャを初期化。
	/// </summary>
	/// <param name="initData"></param>
	void InitTextures(const SpriteInitData& initData);
	/// <summary>
	/// シェーダーを初期化。
	/// </summary>
	/// <param name="initData"></param>
	void InitShader( const SpriteInitData& initData );
	/// <summary>
	/// ディスクリプタヒープを初期化。
	/// </summary>
	/// <param name="initData"></param>
	void InitDescriptorHeap(const SpriteInitData& initData);
	/// <summary>
	/// 頂点バッファとインデックスバッファを初期化。
	/// </summary>
	/// <param name="initData"></param>
	void InitVertexBufferAndIndexBuffer(const SpriteInitData& initData);
	/// <summary>
	/// パイプラインステートを初期化する。
	/// </summary>
	void InitPipelineState(const SpriteInitData& initData);
	/// <summary>
	/// 定数バッファを初期化。
	/// </summary>
	/// <param name="initData"></param>
	void InitConstantBuffer(const SpriteInitData& initData);
private:
	IndexBuffer m_indexBuffer;			//インデックスバッファ。
	VertexBuffer m_vertexBuffer;		//頂点バッファ。
	int m_numTexture = 0;				//テクスチャの枚数。
	Texture m_textures[MAX_TEXTURE];	//テクスチャ。
	Texture* m_textureExternal[MAX_TEXTURE] = {nullptr};	//外部から指定されたテクスチャ
	Vector3 m_position ;				//座標。
	Vector2 m_size;						//サイズ。
	Quaternion m_rotation ;				//回転。
	Matrix m_world;						//ワールド行列。
	SpriteInitData m_initData;

	struct LocalConstantBuffer {
		Matrix mvp;
		Vector4 mulColor = Vector4::White;
		float lightIntensity = 4.0f;
	};
	LocalConstantBuffer m_constantBufferCPU;	//CPU側の定数バッファ。
	ConstantBuffer		m_constantBufferGPU;	//GPU側の定数バッファ。
	ConstantBuffer		m_userExpandConstantBufferGPU;	//ユーザー拡張の定数バッファ(GPU側)
	void* m_userExpandConstantBufferCPU = nullptr;		//ユーザー拡張の定数バッファ(CPU側)
	ConstantBuffer		m_userSetConstantBufferGPU;		//ユーザー設定型定数バッファ。
	void* m_userSetConstantBufferCPU = nullptr;			//ユーザー設定型定数Entity。
	DescriptorHeap		m_descriptorHeap;		//ディスクリプタヒープ。
	RootSignature		m_rootSignature;		//ルートシグネチャ。
	PipelineState		m_pipelineState;		//パイプラインステート。
	Shader				m_vs;					//頂点シェーダー。
	Shader				m_ps;					//ピクセルシェーダー。
	bool m_isDraw3D = false;					//isDraw3D.viewProjの関係上ここに定義。
};