#pragma once

/// <summary>
/// �Q�[���p�̃J�����N���X�B
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
	/// �X�^�[�g�B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
	/// <summary>
	/// ���_�̈ʒu��ݒ�B
	/// </summary>
	/// <param name="eyePos">���_�B</param>
	void SetEyePos(Vector3& eyePos)
	{
		m_playerPos = eyePos;
	}
	/// <summary>
	/// �J�����̎�ނ�ݒ�B
	/// <para>false��TPS�J�����B</para>
	/// </summary>
	/// <param name="flag">�J�����̎�ށB</param>
	void SetCameraType(bool cameraType)
	{
		m_isFPS = cameraType;
	}
	/// <summary>
	/// �J�����̌����擾�B
	/// </summary>
	/// <returns>�J�����̌����B</returns>
	Vector3& GetToPos()
	{
		return m_toPos;
	}
private:
	/// <summary>
	/// FPS�J�����B
	/// </summary>
	void MoveCameraOnFPS();
	/// <summary>
	/// TPS�J�����B
	/// <para>todo:�J�����̈ʒu�A�ߕ��ʍĐݒ�B</para>
	/// </summary>
	void MoveCameraOnTPS();
private:
	Vector3 m_playerPos = g_vec3Zero;				//�v���C���[�̈ʒu�B
	Vector3 m_toPos = { 0.0f, 0.0f, 100.0f };		//���_�܂��́A�����_�Ɍ������x�N�g���B
	Vector3 m_cameraRot = g_vec3Zero;					//�J�����̉�]�B
	bool m_isFPS = true;					//FPS���[�h�B
	const float HORIZON = 1.0f;			//���̃J�������x�B
	const float VERTICAL = 1.0f;		//�c�̃J�������x�B
};

