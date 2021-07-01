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
	/// <summary>
	/// ボーン座標を参照するレンダーを設定。
	/// </summary>
	void SetRefBoneRender(SkinModelRender* render)
	{
		m_refBoneRender = render;
	}
	/// <summary>
	/// 位置を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPos() const 
	{
		return m_pos;
	}
	/// <summary>
	/// 回転を取得。
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation() const 
	{
		return m_rot;
	}
private:
	//モデル基礎パラメーター。
	SkinModelRender* m_render = nullptr;
	Vector3 m_pos = g_vec3Zero;
	Quaternion m_rot = g_quatIdentity;
	Vector3 m_scale = g_vec3One;	
	Bone* m_rHandBone = nullptr;					//右手ボーン。
	SkinModelRender* m_refBoneRender = nullptr;		//ボーンを参照するレンダー。
	GamePlayer* pl = nullptr;
};

