#pragma once

#include "tkFile/TkmFile.h"
#include "tkFile/TkaFile.h"
#include "MeshParts.h"
#include "Skeleton.h"
#include "srcFile/Animation/AnimationClip.h"

class IShaderResource;

/// <summary>
/// モデルの初期化データ
/// </summary>
struct ModelInitData {
	const char* m_tkmFilePath = nullptr;		//tkmファイルパス。
	const char* m_vsEntryPointFunc = "VSMain";	//頂点シェーダーのエントリーポイント。
	const char* m_psEntryPointFunc = "PSMain";	//ピクセルシェーダーのエントリーポイント。
	const char* m_fxFilePath = nullptr;			//.fxファイルのファイルパス。
	void* m_expandConstantBuffer = nullptr;		//ユーザー拡張の定数バッファ。
	int m_expandConstantBufferSize = 0;			//ユーザー拡張の定数バッファのサイズ。
	IShaderResource* m_expandShaderResoruceView = nullptr;	//ユーザー拡張のシェーダーリソース。
};

/// <summary>
/// アニメーション初期化データ。
/// </summary>
struct AnimInitData {
	const char* m_tkaFilePath = nullptr;		//tkaファイルパス。
};

/// <summary>
/// モデルの初期化ステップ。
/// </summary>
/// <remarks>
/// こいつでcase管理しつつ、モデルの初期化を進める。
/// </remarks>
enum ModelInitStep {
	enInitStep_LoadModel,
	enInitStep_LoadSkelton,
	enInitStep_Completed,
	enInitStep_LoadAnimationClips,
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
	int initSkeleton();
	/// <summary>
	/// アニメーションの初期化。
	/// </summary>
	/// <param name="initData"></param>
	void InitAnim(const AnimInitData& initData);
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
	/// 描画
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	void Draw(RenderContext& renderContext);
	/// <summary>
	/// ワールド行列を取得。
	/// </summary>
	/// <returns></returns>
	const Matrix& GetWorldMatrix() const
	{
		return m_world;
	}
private:
	using AnimationClipPtr = std:: unique_ptr<AnimationClip>;
	Matrix			m_world;		//ワールド行列。
	TkmFile			m_tkmFile;		//tkmファイル。
	TkaFile			m_tkaFile;		//tkaファイル。
	Skeleton		m_skeleton;		//スケルトン。
	MeshParts		m_meshParts;	//メッシュパーツ。
	std::vector<std::string>		m_tkaFilePaths;		//tkaファイルのファイルパスリスト。
	std::vector<AnimationClipPtr>	m_animationClips;	//アニメーションクリップ。
};
