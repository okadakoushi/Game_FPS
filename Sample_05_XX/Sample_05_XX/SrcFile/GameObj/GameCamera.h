#pragma once

/// <summary>
/// ゲーム用のカメラクラス。
/// FPSCamera。
/// </summary>
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
	/// カメラ設定。
	/// </summary>
	/// <param name="cameraPos">カメラの位置。</param>
	/// <param name="height">高さ。</param>
	void SetPosOnFPS(Vector3& cameraPos)
	{
		GraphicsEngineObj()->GetCamera3D().SetPosition(cameraPos);
	}
	/// <summary>
	/// ターゲット設定。
	/// </summary>
	/// <param name="Target"></param>
	void SetTarget(Vector3& Target)
	{
		GraphicsEngineObj()->GetCamera3D().SetTarget(Target);
	}
private:
};

