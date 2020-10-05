#pragma once

#include "AnimationClip.h"
class Skeleton;
class Animation;

class AnimationController
{
public:
	AnimationController()
	{
	}
	~AnimationController()
	{
	}
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="skelton">スケルトン。</param>
	/// <param name="footStepBoneNo">footstepボーン番号。</param>
	void Init(Skeleton* skelton, int footStepBoneNo);
	/// <summary>
	/// アニメーションクリップの変更。
	/// </summary>
	/// <param name="clip">アニメーションクリップ。</param>
	void ChangeAnimationClip(AnimationClip* clip)
	{
		//変更があるので初期化。
		m_animationClip = clip;
		m_currentKeyFrameNo = 0;
		m_time = 0.0f;
		m_isPlaying = true;
		m_footstepPos = g_vec3Zero;
		m_footstepDeltaValue = g_vec3Zero;
	}
	/// <summary>
	/// 補間時間を設定。
	/// </summary>
	/// <param name="interpolateTime">補間時間。</param>
	void SetInterpolateTime(float interpolateTime)
	{
		if (interpolateTime < 0.0f) {
			MessageBox(
				nullptr,
				L"SetInterpolateTime(value) 補間時間が不正です。",
				L"エラー",
				S_OK
			);
			m_interpolateEndTime = interpolateTime;
			m_interpolateTime = 0.0f;
		}
	}
	/// <summary>
	/// 補間率を取得。
	/// </summary>
	/// <returns></returns>
	float GetInterpolateRate() const
	{
		if (m_interpolateEndTime <= 0.0f) {
			//値が不正、補間しない。
			return 1.0f;
		}
		return min(1.0f, m_interpolateTime / m_interpolateEndTime);
	}
	/// <summary>
	/// アニメーションを進める。
	/// </summary>
	/// <param name="deltaTime">進める時間。。</param>
	/// <param name="animtion">アニメーション。</param>
	void Update(float deltaTime, Animation* animtion);
	/// <summary>
	/// ローカルボーン行列を取得。
	/// </summary>
	/// <returns></returns>
	const std::vector<Matrix>& GetBoneLocalMatrix() const
	{
		return m_boneMatrix;
	}
	/// <summary>
	/// アニメーションクリップを取得。
	/// </summary>
	/// <returns>アニメーションクリップ。</returns>
	AnimationClip* GetAnimClip() const
	{
		return m_animationClip;
	}
	/// <summary>
	/// 再生中？
	/// </summary>
	/// <returns></returns>
	bool IsPlaying() const
	{
		return m_isPlaying;
	}
	/// <summary>
	/// Update関数を呼び出した時の、footStepボーンの移動量を取得。
	/// </summary>
	/// <returns></returns>
	Vector3 GetFootstepDeltaValueOnUpdate() const
	{
		return m_footstepDeltaValue;
	}
private:
	/// <summary>
	/// ループ再生を開始するときの処理。
	/// </summary>
	void StartLoop();
	/// <summary>
	/// ルートボーン空間でのボーン行列を計算。
	/// </summary>
	/// <param name="bone">計算する骨</param>
	/// <param name="parentMarix">親の行列。</param>
	void ClacBoneMatrixInRootBoneSpace(Bone& bone, Matrix parentMatrix);
	/// <summary>
	/// アニメーションクリップからボーン行列をサンプリング。
	/// </summary>
	void SamplingBoneMatrixFromAnimationClip();
	/// <summary>
	/// ルートボーン空間での行列を計算。
	/// </summary>
	void ClacBoneMatrixInRootBoneSpace();
	/// <summary>
	/// footstepボーンの変化量をサンプリング。
	/// </summary>
	void SamplingDeltaValueFootstepBone();
	/// <summary>
	/// footstepボーンの座標を全体の骨から減算する。
	/// </summary>
	void SubtractFootstepbonePosFromAllBone();
	/// <summary>
	/// キーフレーム番号を進める。
	/// </summary>
	/// <param name="deltaTime">進める時間。</param>
	void ProgressKeyFrameNo(float deltaTime);

private:
	AnimationClip*		m_animationClip = nullptr;			//アニメーションクリップ。
	int					m_currentKeyFrameNoLastFrame = 0;	//1フレーム前のキーフレーム。
	int					m_currentKeyFrameNo = 0;			//現在のフレーム番号。
	float				m_time = 0.0f;						//時間。
	std::vector<Matrix>	m_boneMatrix;						//ボーン行列。
	float				m_interpolateTime;					//補間時間。
	float				m_interpolateEndTime;				//補間終了時間。
	bool				m_isPlaying = false;				//再生中かどうか。
	Skeleton*			m_skelton = nullptr;				//スケルトン。
	Vector3				m_footstepDeltaValue = g_vec3Zero;	//フットステップの移動ベクトル。
	Vector3				m_footstepPos = g_vec3Zero;			//フットステップボーンの座標。
	int					m_footstepBoneNo = -1;				//フットステップのボーンの番号。
};

