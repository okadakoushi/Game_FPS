#pragma once

/// <summary>
/// �Q�[���p�̃J�����N���X�B
/// FPSCamera�B
/// </summary>
class GameCamera : public IGameObject
{
public:
	~GameCamera();
	/// <summary>
	/// �X�^�[�g�B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �J�����ݒ�B
	/// </summary>
	/// <param name="cameraPos">�J�����̈ʒu�B</param>
	/// <param name="height">�����B</param>
	void SetPosOnFPS(Vector3& cameraPos)
	{
		GraphicsEngineObj()->GetCamera3D().SetPosition(cameraPos);
	}
	/// <summary>
	/// �^�[�Q�b�g�ݒ�B
	/// </summary>
	/// <param name="Target"></param>
	void SetTarget(Vector3& Target)
	{
		GraphicsEngineObj()->GetCamera3D().SetTarget(Target);
	}
private:
};

