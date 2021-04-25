#pragma once

class GameCamera;
class Rifle;

/// <summary>
/// �v���C���[�N���X�B
/// </summary>
class GamePlayer : public IGameObject
{
public:
	void OnDestroy() override;
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
	/// <summary>
	/// �����_�[�擾�B
	/// </summary>
	/// <returns></returns>
	SkinModelRender* GetRender()
	{
		return m_unityChan;
	}

	void SetCamera(GameCamera* camera)
	{
		m_camera = camera;
	}
private:
	enum EnPlayerState {
		EnPlayerState_Idle,		//�����Ȃ��B
		EnPlayerState_Walk,		//�����B
		EnPlayerState_Run,		//����B
		EnPlayerState_Buck,		//��ށB
		EnPlayerState_Shot,		//�ˌ�
		EnPlayerState_Reload,	//�����[�h�B
		EnPlayerState_Deth,		//���S�B
		EnPlayerState_Num		//���B
	};
	EnPlayerState m_playerState = EnPlayerState_Idle;
	SkinModelRender* m_unityChan = nullptr;		//���j�e�B�[�����B
	CharacterController m_cCon;					//�L�����R���B
	SpriteRender* m_reticle = nullptr;			//���e�B�N���B
	Vector3 m_pos = g_vec3Zero;					//�ʒu�B
	Vector3 m_cameraPos = g_vec3Zero;			//�J�����̈ʒu�B
	Quaternion m_rot = g_quatIdentity;			//��]�B
	GameCamera* m_camera = nullptr;				//�Q�[���J�����B
	Rifle* m_wepon = nullptr;					//����B
	Vector3 m_move = g_vec3Zero;				//�ړ��B
	float m_speed = 100.0f;						//�ړ����x�B
	float const m_JUMPFORSE = 800.0f;			//�W�����v�B
	float const m_GRAVITY = 30.0f;				//�d�́B
	const float fixYToEyePos = 110.0f;			//���_���W�ɕς���Y���C���B
	int m_flame = 0;
};

