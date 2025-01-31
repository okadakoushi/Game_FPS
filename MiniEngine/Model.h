#pragma once

#include "tkFile/TkmFile.h"
#include "tkFile/TkaFile.h"
#include "MeshParts.h"
#include "Skeleton.h"
#include "srcFile/Animation/AnimationClip.h"
#include "srcFile/Animation/Animation.h"
#include "srcFile/Culling/ObjectFrustumCulling.h"

class IShaderResource;

enum EnRenderMode {
	enRenderMode_Skin,				//スキンあり。
	enRenderMode_NonSkin,			//スキンなし。
	enRenderMode_DrawShadow,		//シャドウマップ描画。
	enRenderMode_NonSkinDrawShadow,	//スキンなしシャドウマップ描画。
	enRenderMode_Num				//いらんきもするが一応。
};

/// <summary>
/// モデルの初期化データ
/// </summary>
struct ModelInitData {
	const char* m_tkmFilePath = nullptr;		//tkmファイルパス。
	const char* m_vsEntryPointFunc = "VSMain";	//頂点シェーダーのエントリーポイント。
	const char* m_psEntryPointFunc = "PSMain";	//ピクセルシェーダーのエントリーポイント。
	const char* m_fxFilePath = nullptr;			//.fxファイルのファイルパス。
	const char* m_tkaFilePath = nullptr;		//tkaファイルパス。
	void* m_expandConstantBuffer = nullptr;		//ユーザー拡張の定数バッファ。
	int m_expandConstantBufferSize = 0;			//ユーザー拡張の定数バッファのサイズ。
	IShaderResource* m_expandShaderResoruceView = nullptr;	//ユーザー拡張のシェーダーリソース。
};

/// <summary>
/// モデルの初期化ステップ。
/// </summary>
/// <remarks>
/// こいつでcase管理しつつ、モデルの初期化を進める。
/// </remarks>


/// <summary>
/// モデルクラス。
/// </summary>
class Model {

public:
	~Model();
	/// <summary>
	/// モデルのみの初期化。
	/// </summary>
	void InitModel(const char* filepath);
	/// <summary>
	/// ワールド行列の更新。
	/// <para>レンダーモードで軸変換するか決めてる。todo:enZUP? enYUP?</para>
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	/// <param name="rm">レンダーモード。</param>
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale, EnRenderMode& rm);
	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="pos">位置。</param>
	/// <param name="rot">回転。</param>
	/// <param name="scale">拡大。</param>
	/// <param name="isForwardRender">フォワードレンダーする？。</param>
	void Update(Vector3 pos, Quaternion rot, Vector3 scale, EnRenderMode& rm);

	/// <summary>
	/// スケルトンを関連付ける。
	/// </summary>
	/// <param name="skeleton">スケルトン</param>
	void BindSkeleton(Skeleton& skeleton)
	{
		m_meshParts.BindSkeleton(skeleton);
	}
	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="renderContext"></param>
	/// <param name="viewMat">ビュー。</param>
	/// <param name="projMat">プロジェクション。</param>
	/// <param name="RenderMode">レンダーモード。</param>
	void Draw(RenderContext& renderContext, Matrix viewMat, Matrix projMat, int RenderMode = 0);
	/// <summary>
	/// ワールド行列を取得。
	/// </summary>
	/// <returns></returns>
	const Matrix& GetWorldMatrix() const
	{
		return m_world;
	}

	const bool& GetAnimFlag() const
	{
		return m_isAnimation;
	}
	/// <summary>
	/// シャドウレシーバーとして登録。
	/// </summary>
	void SetShadowReciever(bool flag)
	{
		isShadowReciever = flag;
	}
	/// <summary>
	/// シャドウキャスターとして登録。
	/// </summary>
	void SetShadwoCaster(bool flag)
	{
		isShadowCaster = flag;
	}
	/// <summary>
	/// シェーダーを設定。
	/// </summary>
	/// <param name="fxPath">シェーダーのファイルパス。</param>
	void SetShader(const wchar_t* fxPath)
	{
		m_shaderFilePath = fxPath;
	}
	/// <summary>
	/// tkmファイルを取得。
	/// </summary>
	/// <returns></returns>
	const TkmFile* GetTkmFile() const 
	{
		return m_tkmFile;
	}
	/// <summary>
	/// 乗算カラーを設定。
	/// </summary>
	/// <param name="color"></param>
	void SetMulColor(const Vector4& color)
	{
		m_meshParts.SetMulColor(color);
	}
	/// <summary>
	/// メッシュパーツを取得。
	/// </summary>
	/// <returns></returns>
	MeshParts& GetMeshParts()
	{
		return m_meshParts;
	}
	/// <summary>
	/// メッシュパーツに対してクエリ。
	/// </summary>
	/// <param name="func"></param>
	void QueryMeshParts(std::function<void(const SMesh*mesh)> func)
	{
		for (auto* mesh : m_meshParts.GetMeshList()) {
			func(mesh);
		}
	}
	/// <summary>
	/// シェーダーのファイルパスを設定。
	/// </summary>
	/// <param name="filePath"></param>
	void SetShaderPath(const wchar_t* filePath)
	{
		m_shaderFilePath = filePath;
	}
	/// <summary>
	/// PSシェーダーのエントリーポイントを設定。
	/// </summary>
	/// <param name="entry"></param>
	void SetPSEntry(const char* entry)
	{
		m_psEntry = entry;
	}
	/// <summary>
	/// 拡張定数バッファを設定。
	/// <para>Init前に呼び出さないと、処理の都合上効果がないので注意。</para>
	/// </summary>
	/// <param name="exCB">拡張定数バッファ。</param>
	/// <param name="size">定数バッファのサイズ。</param>
	void SetExpandConstantBuffer(void* exCB, int& size)
	{
		m_expandConstantBuffer = exCB;
		m_expandConstantBufferSize = size;
	}
private:
	/// <summary>
	/// aabbを初期化。
	/// </summary>
	void InitAABB();
	using AnimationClipPtr = std:: unique_ptr<AnimationClip>;
	Matrix			m_world;					//ワールド行列。
	TkmFile*		m_tkmFile = nullptr;		//tkmファイル。
	MeshParts		m_meshParts;	//メッシュパーツ。
	bool			isShadowCaster = true;		//シャドウキャスター？
	bool			isShadowReciever = true;	//シャドウレシーバー？
	bool			m_isAnimation = false;		//アニメーションしてる？ todo:頭悪い。
	void*  m_expandConstantBuffer = nullptr;				//拡張定数バッファ。
	int m_expandConstantBufferSize = 0;						//拡張定数バッファのサイズ。
	IShaderResource* m_expandShaderResoruceView = nullptr;	//拡張SRV
	const wchar_t* m_shaderFilePath = L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";			//シェーダーのファイルパス。
	const char* m_vsEntry = "VSMain";
	const char* m_psEntry = "PSMain";
	char m_name[50];									//確認用のネーム。
	Box m_aabb;									//ボックス。
	ObjectFrustumCulling m_frustomCulling;				//視錐台カリング。
};
