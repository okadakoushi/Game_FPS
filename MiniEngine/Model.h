#pragma once

#include "tkFile/TkmFile.h"
#include "tkFile/TkaFile.h"
#include "MeshParts.h"
#include "Skeleton.h"
#include "srcFile/Animation/AnimationClip.h"
#include "srcFile/Animation/Animation.h"

class IShaderResource;

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

	/// <summary>
	/// モデルのみの初期化。
	/// </summary>
	void InitModel(const char* filepath);
	/// <summary>
	/// ワールド行列の更新。
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale);
	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="pos">位置。</param>
	/// <param name="rot">回転。</param>
	/// <param name="scale">拡大。</param>
	/// <param name="isForwardRender">フォワードレンダーする？。</param>
	void Update(Vector3 pos, Quaternion rot, Vector3 scale);

	/// <summary>
	/// スケルトンを関連付ける。
	/// </summary>
	/// <param name="skeleton">スケルトン</param>
	void BindSkeleton(Skeleton& skeleton)
	{
		m_meshParts.BindSkeleton(skeleton);
	}
	/// <summary>
	/// アニメーションを再生。
	/// </summary>
	/// <param name="animNo">クリップ番号。</param>
	/// <param name="interpolateTime">補間率。</param>
	void PlayAnim(int animNo, float interpolateTime) {
		m_animation.Play(animNo, interpolateTime);
	}
	/// <summary>
	/// 
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
	/// <summary>
	/// シャドウレシーバーとして登録。
	/// </summary>
	void SetShadowReciever()
	{
		isShadowReciever = true;
	}
	/// <summary>
	/// シャドウキャスターとして登録。
	/// </summary>
	void SetShadwoCaster()
	{
		isShadowCaster = true;
	}
	/// <summary>
	/// シェーダーを設定。
	///<para>ちょっと実用性がない実装かも。todo:修正。</para>
	/// </summary>
	/// <param name="fxPath">シェーダーのファイルパス。</param>
	void SetShader(const wchar_t* fxPath)
	{
		m_shaderFilePath = fxPath;
	}
private:
	using AnimationClipPtr = std:: unique_ptr<AnimationClip>;
	Matrix			m_world;		//ワールド行列。
	TkmFile			m_tkmFile;		//tkmファイル。
	TkaFile			m_tkaFile;		//tkaファイル。
	Skeleton		m_skeleton;		//スケルトン。
	MeshParts		m_meshParts;	//メッシュパーツ。
	bool			isShadowCaster = false;		//シャドウキャスター？
	bool			isShadowReciever = false;	//シャドウレシーバー？
	void*  m_expandConstantBuffer = nullptr;				//拡張定数バッファ。
	int m_expandConstantBufferSize = 0;						//拡張定数バッファのサイズ。
	IShaderResource* m_expandShaderResoruceView = nullptr;	//拡張SRV
	std::vector<std::string>		m_tkaFilePaths;		//tkaファイルのファイルパスリスト。
	std::vector<AnimationClipPtr>	m_animationClips;	//アニメーションクリップ。
	Animation						m_animation;		//アニメーション
	const wchar_t* m_shaderFilePath = L"Assets/shader/NoAnimModel_LambertSpecularAmbient.fx";			//シェーダーのファイルパス。
};
