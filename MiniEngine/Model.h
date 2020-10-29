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
enum ModelInitStep {
	enInitStep_None,
	enInitStep_LoadModel,
	enInitStep_LoadSkelton,
	enInitStep_LoadAnimationClips,
	enInitStep_Completed,
	initStep
};

/// <summary>
/// モデルクラス。
/// </summary>
class Model {

public:

	/// <summary>
	/// モデル初期化ステップ。
	/// </summary>
	/// <param name="initData">初期化データ。</param>
	/// <returns>ステップ</returns>
	int Init( const ModelInitData& initData );
	/// <summary>
	/// スケルトンデータ読み込みステップ。
	/// </summary>
	/// <param name="initData">初期化データ</param>
	/// <returns>ステップ</returns>
	int LoadTks(const ModelInitData& initData);
	/// <summary>
	/// スケルトンの初期化処理。
	/// </summary>
	/// <returns></returns>
	int initSkeleton(const ModelInitData& initData);
	/// <summary>
	/// アニメーションの初期化。
	/// </summary>
	/// <param name="initData"></param>
	int InitAnim();
	/// <summary>
	/// モデルのみの初期化。
	/// </summary>
	void InitModel(const char* filepath);
	/// <summary>
	/// ワールド座標とアニメーションの更新。
	/// </summary>
	void UpdateMatAndAnim();
	/// <summary>
	/// ワールド行列の更新。
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale);

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
	void SetPosition(Vector3 pos) {
		m_pos = pos;
	}

	void SetRotation(Quaternion qRot)
	{
		m_rot = qRot;
	}
	friend class DirectionalShadowMap;
protected:
	/// <summary>
	/// シャドウキャスターとして登録。
	/// </summary>
	/// <remarks>
	/// ユーザー側には使用してほしくないが、シャドウマップで使うので。
	/// </remarks>
	void SetShadwoCaster()
	{
		isShadowCaster = true;
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
	Vector3	m_pos = Vector3::Zero;				//座標。
	Vector3	m_scale = Vector3::One;				//拡大率。
	Quaternion m_rot = Quaternion::Identity;	//回転。
};
