#pragma once

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
	void SetEyePos(Vector3& eyePos)
	{
		m_playerPos = eyePos;
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
	Vector3 m_playerPos = g_vec3Zero;				//プレイヤーの位置。
	Vector3 m_toPos = { 0.0f, 0.0f, 100.0f };		//視点または、注視点に向かうベクトル。
	Vector3 m_cameraRot = g_vec3Zero;					//カメラの回転。
	bool m_isFPS = true;					//FPSモード。
	const float HORIZON = 1.0f;			//横のカメラ速度。
	const float VERTICAL = 1.0f;		//縦のカメラ速度。
};

