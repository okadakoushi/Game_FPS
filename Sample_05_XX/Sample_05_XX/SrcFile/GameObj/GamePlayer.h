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
	/// <summary>
	/// ��]�B
	/// <para>�s���ɂ��GameCamera���Ăяo���B</para>
	/// </summary>
	void Rotation();
	/// <summary>
	/// ���ˁI�I
	/// </summary>
	void Shot();
	/// <summary>
	/// �ʒu���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPos()const
	{
		return m_pos;
	}
private:
	enum EnPlayerState {
		EnPlayerState_Idle,		//�����Ȃ��B
		EnPlayerState_Walk,		//�����B
		EnPlayerState_Run,		//����B
		EnPlayerState_Shot,		//�ˌ�
		EnPlayerState_Reload,	//�����[�h�B
		EnPlayerState_Deth,		//���S�B
		EnPlayerState_Num		//���B
	};
	EnPlayerState m_playerState = EnPlayerState_Idle;
	SkinModelRender* m_unityChan = nullptr;		//���j�e�B�[�����B
	Vector3 m_pos = g_vec3Zero;					//�ʒu�B
	Quaternion m_rot = g_quatIdentity;			//��]�B
	GameCamera* m_camera = nullptr;				//�Q�[���J�����B
	float m_speed = 1.0f;						//�ړ����x�B
	int m_flame = 0;
};

