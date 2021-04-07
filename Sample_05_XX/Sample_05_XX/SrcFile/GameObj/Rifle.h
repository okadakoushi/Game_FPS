#pragma once

class GamePlayer;

class Rifle : public IGameObject
{
	//~Rifle();

public:
	/// <summary>
	/// スタート。
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
private:
	//モデル基礎パラメーター。
	SkinModelRender* m_render = nullptr;
	Vector3 m_pos = g_vec3Zero;
	Quaternion m_rot = g_quatIdentity;
	Vector3 m_scale = g_vec3One;	
	//ハンドのパラメーター。
	Vector3 m_Rpos = g_vec3Zero;
	Quaternion m_Rrot = g_quatIdentity;
	Vector3 m_Lpos = g_vec3Zero;
	Quaternion m_Lrot = g_quatIdentity;
	
	GamePlayer* m_player = nullptr;	//プレイヤー。
	Bone* m_rHandBone = nullptr;	//右手ボーン。
	Bone* m_lHandBone = nullptr;	//右手ボーン。
};

