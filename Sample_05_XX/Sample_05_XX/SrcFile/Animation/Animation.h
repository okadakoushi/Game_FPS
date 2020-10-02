#pragma once

#include <utility>
#include "AnimationClip.h"
#include "AnimationController.h"

/// <summary>
/// アニメーションクラス。
/// </summary>
/// <remarks>
/// シンプルなアニメーション再生。
/// アニメーション補間。
/// </remarks>


class Animation
{
public:
	/// <summary>
	/// 初期化済みかの判定。
	/// </summary>
	/// <returns>成功、失敗。</returns>
	bool IsInited() const
	{
	}
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="skelton">スケルトン。</param>
	/// <param name="animClip">アニメーションクリップ。</param>
	void Init(Skeleton& skelton, const vector<unique_ptr<AnimationClip>>& animClip);
	
	void Play(int clipNo, float interpolateTime = 0.0f)
	{
		if (clipNo < m_animationClips.size()) {
			PlayCommon(m_animationClips[clipNo], interpolateTime);
		}
	}
	/// <summary>
	/// アニメーションクリップのループフラグを設定。
	/// </summary>
	/// <param name="clipName">アニメーションクリップ名。</param>
	/// <param name="flag">フラグ。</param>
	void SetAnimationClipLoopFlag(const wchar_t* clipName, bool flag)
	{
		//クリップネームでイテレーター。
		auto it = find_if(
			m_animationClips.begin(),
			m_animationClips.end(),
			[clipName](auto& clip) {return clip->GetName() == clipName; }
		);
		if (it == m_animationClips.end()) {
			//見つからなかった。
			return;
		}
		(*it)->SetLoopFlag(flag);
	}

	bool IsPlaying() const
	{
		int lastIndex = GetLastAnimatitonControllerIndex();
		return m_animationController[lastIndex].IsPlaying();
	}

	/// <summary>
	/// アニメーションを進める。
	/// <para>インターフェイス側処理。</para>
	/// </summary>
	/// <param name="deltaTime"></param>
	void Progress(float deltaTime);

	//Vector3 CalcFootstepDeltaValueInWorldSpace(Quaternion rot, Vector3 scale) const;

private:
	//処理が内部でのみしか使わないので隠蔽。

	void PlayCommon(AnimationClip* nextClip, float interpolateTime)
	{
		if (nextClip->IsLoaded() == false) {
			//アニメーションがロードされてない。
			MessageBox(
				nullptr,
				L"PlayCommon(value) アニメーションクリップがロードされていません。",
				L"エラー",
				S_OK
			);
			return;
		}
		int index = GetLastAnimatitonControllerIndex();
		if (m_animationController[index].GetAnimClip() == nextClip) {
			//これ以上アニメーションコントローラーがない。
			return;
		}
		if (interpolateTime == 0.0f) {
			//補間ないので複数のコントローラーが必要ない。
			m_numAnimationPlayController = 1;
		}
		else {
			//補間するのでコントローラーを増やす。
			m_numAnimationPlayController++;
		}
		index = GetLastAnimatitonControllerIndex();
		m_animationController[index].ChangeAnimationClip(nextClip);
		m_animationController[index].SetInterpolateTime(interpolateTime);
		interpolateTime = 0.0f;
		m_isterpolateTimeEnd = interpolateTime;
	}
	/// <summary>
	/// ローカルポーズの更新。
	/// </summary>
	/// <param name="deltaTime">アニメーションを進める時間。</param>
	void UpdateLocalPose(float deltaTime);
	/// <summary>
	/// グローバルポーズの更新。
	/// </summary>
	void UpdateGlobalPose();
	/// <summary>
	/// 最終ポーズになるアニメーションのリングバッファ上でのインデックスを取得。
	/// </summary>
	/// <returns>最終ポーズアニメーションのインデックス。</returns>
	int GetLastAnimatitonControllerIndex() const
	{
		return GetAnimationControllerIndex(m_startAnimationPlayController, m_numAnimationPlayController - 1);
	}
	/// <summary>
	/// アニメーションコントローラのリングバッファ上でのインデックスを取得。
	/// </summary>
	/// <param name="startIndex">開始インデックス。</param>
	/// <param name="localIndex">ローカルインデックス。</param>
	/// <returns></returns>
	int GetAnimationControllerIndex(int startIndex, int localIndex) const
	{
		return (startIndex + localIndex) % ANIMATION_PLAY_CONTROLLER_NUM;
	}
private:
	static const int ANIMATION_PLAY_CONTROLLER_NUM = 32;	//アニメーションコントローラの数。
	vector<AnimationClip*> m_animationClips;				//アニメーションクリップのリスト。
	Skeleton* m_skelton = nullptr;							//アニメーションを適用するボーン。
	AnimationController m_animationController[ANIMATION_PLAY_CONTROLLER_NUM];	//アニメーションコントローラー。
	int m_numAnimationPlayController = 0;					//使用中のアニメーションコントローラー。
	int m_startAnimationPlayController = 0;					//アニメーションコントローラ開始インデックス。
	float m_isterpolateTime = 0.0f;							//補間時間？						
	float m_isterpolateTimeEnd = 0.0f;						//補間終了時間？
	bool m_isInterpolate = false;							//補間中かのフラグ。
	Vector3 m_footstepDeltaValue = g_vec3Zero;				//footstepボーンの移動量。
	bool m_isInited = false;								//初期化フラグ。
	float m_deltaTimeOnUpdate = 0.0f;						//UpdateするときのDeltaTime。

};


