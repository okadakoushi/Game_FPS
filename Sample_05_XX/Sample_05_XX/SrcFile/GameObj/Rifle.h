#pragma once

class GamePlayer;

class Rifle : public IGameObject
{
public:
	//~Rifle();
	enum EnRifleEvent {
		EnRifleEvent_None,
		EnRifleEvent_NoAmo,
		EnRifleEvent_Reloading
	};
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
	/// ボーンを参照するレンダーを設定。
	/// </summary>
	/// <param name="mr"></param>
	void SetRefBoneRender(SkinModelRender* mr)
	{
		m_refBoneRender = mr;
	}
	/// <summary>
	/// リロードタイムを加算。
	/// </summary>
	void AddReloadTime()
	{
		m_currentReloadTime += GameTime().GetFrameDeltaTime();
	}
	/// <summary>
	/// 装填弾をへらす。
	/// </summary>
	void ReduseAmo()
	{
		m_currentAmo--;
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
	/// <summary>
	/// ライフルイベントを取得。
	/// </summary>
	/// <returns></returns>
	const EnRifleEvent& GetRifleEvent() const
	{
		return m_currentRifleEvent;
	}
	/// <summary>
	/// ライフルイベントを設定。
	/// </summary>
	/// <param name="event"></param>
	void SetRifleEvent(const EnRifleEvent& event)
	{
		m_currentRifleEvent = event;
	}
	/// <summary>
	/// 現在の装填数を取得。
	/// </summary>
	/// <returns></returns>
	const int& GetCurrentAmo() const
	{
		return m_currentAmo;
	}
	/// <summary>
	/// 最大装填を取得。
	/// </summary>
	/// <returns></returns>
	const int& GetMAX_AMO() const
	{
		return MAX_AMO;
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
	bool m_isShoot = true;						//発砲可能？
	const int MAX_AMO = 30;						//最大弾数。todo:Rifleに移譲。
	int m_currentAmo = MAX_AMO;					//現在の弾数。todo
	const float RELOADTIME = 0.5f;				//リロード時間。todo
	float m_currentReloadTime;					//現在のリロード時間。
	EnRifleEvent m_currentRifleEvent = EnRifleEvent_None;			//ライフルイベント。
};

