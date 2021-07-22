#pragma once

class GamePlayer;

/// <summary>
/// ゲーム用のカメラクラス。
/// </summary>
/// <code>
/// 1.NewGO();
/// 2.SetEyePos();
/// 3.(change camera)SetCameraType();
/// </code>
class GameCamera : public IGameObject
{
public:
	~GameCamera();
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
	/// 視点の位置を設定。
	/// </summary>
	/// <param name="eyePos">視点。</param>
	void SetEyePos(const Vector3& eyePos)
	{
		m_playerPos = eyePos;
	}

	void AddRecoil(const float& recoil)
	{
		if(m_recoil < 10.0f){
			m_recoil += recoil;
		}
	}
	/// <summary>
	/// カメラの種類を設定。
	/// <para>falseでTPSカメラ。</para>
	/// </summary>
	/// <param name="flag">カメラの種類。</param>
	void SetCameraType(bool cameraType)
	{
		m_isFPS = cameraType;
	}
	/// <summary>
	/// カメラの向き取得。
	/// </summary>
	/// <returns>カメラの向き。</returns>
	Vector3& GetToPos()
	{
		return m_toPos;
	}
	/// <summary>
	/// カメラの修正値を設定。
	/// </summary>
	void SetFixVal(const float& val)
	{
		fixVal = val;
	}
	/// <summary>
	/// カメラの移動量を取得。
	/// </summary>
	/// <returns></returns>
	const Vector2& GetCameraMove() const 
	{
		return m_cameraMove;
	}
private:
	/// <summary>
	/// FPSカメラ。
	/// </summary>
	void MoveCameraOnFPS();
	/// <summary>
	/// TPSカメラ。
	/// <para>todo:カメラの位置、近平面再設定。</para>
	/// </summary>
	void MoveCameraOnTPS();
private:
	Bone* m_headBone = nullptr;
	Vector3 m_playerPos = {0.0f, 100.0f, 0.0f};				//プレイヤーの位置。
	Vector3 m_toPos = { 0.0f, 0.0f, 100.0f };		//視点または、注視点に向かうベクトル。
	Vector3 m_cameraRot = g_vec3Zero;				//カメラの回転。
	float m_recoil = 0.0f;					//反動。
	bool m_isFPS = true;					//FPSモード。
	float fixVal = 15.0f;				//修正値。
	const float HORIZON = 1.0f;			//横のカメラ速度。
	const float VERTICAL = 1.0f;		//縦のカメラ速度。
	Vector2 m_beforeFramePoint = g_vec2Zero;			//前フレームのマウスポインタの位置。
	GamePlayer* m_player = nullptr;
	Vector2 m_cameraMove = g_vec2Zero;
};

