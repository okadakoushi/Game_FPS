#pragma once

class GameCamera;

/// <summary>
/// �v���C���[�N���X�B
/// </summary>
class GamePlayer : public IGameObject
{
public:
	~GamePlayer();
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
	/// �ړ��B
	/// <para>�ړ��̓J��������ɍs����B</para>
	/// </summary>
	void Move();
private:
	SkinModelRender* m_unityChan = nullptr;		//���j�e�B�[�����B
	Vector3 m_pos = g_vec3Zero;					//�ʒu�B
	Quaternion m_rot = g_quatIdentity;			//��]�B
	GameCamera* m_camera = nullptr;				//�Q�[���J�����B
	float m_speed = 1.0f;						//�ړ����x�B
};

