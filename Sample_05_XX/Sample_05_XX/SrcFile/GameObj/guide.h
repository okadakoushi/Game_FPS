#pragma once

#include "SoldierCollision.h"

class GamePlayer;
class StageGenerator;

/// <summary>
/// ガイドキャラ。
/// <para>スタンバイ→バトルの遷移を管理。</para>
/// </summary>
class Guide : public IGameObject
{
public:
//	~Guide();
//	bool Start() override;
//	void Update() override;
//	/// <summary>
//	/// 位置を設定。
//	/// </summary>
//	/// <param name="pos"></param>
//	void SetPos(Vector3& pos) {
//		m_pos = pos;
//	}
//private:
//	/// <summary>
//	/// ステージ遷移可能かを計算。
//	/// </summary>
//	/// <returns>フラグ。</returns>
//	bool ClacPossibleTrans();
//private:
//	SkinModelRender* m_skinModelRender = nullptr;
//	Vector3 m_pos = g_vec3Zero;
//	Vector3 m_scale = g_vec3One;
//	Quaternion m_rot = g_quatIdentity;
//	GamePlayer* m_player = nullptr;
//	StageGenerator* m_stageGenerator = nullptr;	
//	float m_changeStageTimer = 0.0f;		//ステージ遷移するかのタイマー。
//	const float m_CHANGESCENETIME = 3.0f;	//この時間以上になると遷移する。
//	SoldierCollision m_collision;
};

