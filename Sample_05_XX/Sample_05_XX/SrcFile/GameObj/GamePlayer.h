#pragma once

class GameCamera;
class Rifle;
class Bullet;

#include "SrcFile/RayTestCallBack.h"
#include "PlayerUIs.h"

/// <summary>
/// �v���C���[�N���X�B
/// </summary>
class GamePlayer : public IGameObject
{
public:
	//�萔�l��public�ł�������邩
	struct BASE_PARAM {
		static const float	PLAYER_MAX_HP;				//�v���C���[�̍ő�̗́B
		static const int	REGENE_VALUE_SECOND;		//�b�ԃ��W�F�l�ʁB
		static const float	REGENE_COOL_TIME;		//���W�F�l�̃N�[���^�C���B
		static const float	JUMPFORSE;				//�W�����v�B
		static const float	GRAVITY;					//�d�́B
	};
private:
	~GamePlayer() override;
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
	/// ���ˁI�I
	/// </summary>
	void Shot();
	/// <summary>
	/// �����[�h�B
	/// </summary>
	void Reload();
	/// <summary>
	/// ���ȉ񕜁B
	/// </summary>
	void Regene();
public:
	void Init();
	/// <summary>
	/// ��]�B
	/// <para>1�t���[������邽�߁A�J����������Ăяo���B</para>
	/// </summary>
	void Rotation();
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
	/// <summary>
	/// �ʒu��ݒ�B
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3& pos)
	{
		m_pos = pos;
		m_cCon.SetPosition(m_pos);
	}
	/// <summary>
	/// �̗͂��擾�B
	/// </summary>
	/// <returns></returns>
	const float& GetHP() const
	{
		return m_hp;
	}
	/// <summary>
	/// �v���C���[�Ƀ_���[�W��^����B
	/// </summary>
	/// <param name="damage"></param>
	void DamageToPlayer(const int& damage)
	{
		m_currentRegeneTime = 0.0f;
		if (m_hp > 0) {
			m_hp -= damage;
		}
	}
	/// <summary>
	/// ����擾�B
	/// </summary>
	/// <returns></returns>
	const Rifle* GetWepon() const
	{
		return m_wepon;
	}
	/// <summary>
	/// ���̈ʒu���v�Z�B
	/// </summary>
	/// <returns></returns>
	Vector3& CalcHeadPos();
	/// <summary>
	/// �v���C���[UI���擾�B
	/// </summary>
	/// <returns></returns>
	PlayerUIs* GetPlayerUIs()
	{
		return m_playerUIs;
	}
private:
	/// <summary>
	/// SkinModelRender�̃A�j���[�V�����i�s���I������^�C�~���O�ŌĂ΂�鏈���B
	/// </summary>
	/// <remarks>
	/// �A�j���[�V������i�s���āA���̃��[�J���s����X�V�������ƂŁA
	/// ���̍����v���O�����I�ɓ������K�v���������̂ŁA�{�֐���ǉ��B
	/// </remarks>
	void OnPostAnimationProgress();
private:
	enum EnPlayerState {
		EnPlayerState_Idle,		//�����Ȃ��B
		EnPlayerState_Walk,		//�����B
		EnPlayerState_Shot,		//�ˌ�
		EnPlayerState_Deth,		//���S�B
		EnPlayerState_Buck,		//��ށB
		EnPlayerState_Run,		//����B
		EnPlayerState_Reload,	//�����[�h�B
		EnPlayerState_Num		//���B
	};
	EnPlayerState m_playerState = EnPlayerState_Idle;
	SkinModelRender* m_unityChan = nullptr;		//���j�e�B�[�����B
	CharacterController m_cCon;					//�L�����R���B
	SpriteRender* m_reticle = nullptr;			//���e�B�N���B
	Vector3 m_pos = g_vec3Zero;					//�ʒu�B
	Quaternion m_rot = g_quatIdentity;			//��]�B
	Rifle* m_wepon = nullptr;					//����B
	Bone* m_spineBone = nullptr;				//���̃{�[���B
	Bone* m_rHandBone = nullptr;				//�E��̃{�[���B
	Bone* m_headBone = nullptr;					//���{�[���B
	Vector3 m_headPos = g_vec3Zero;				//���̈ʒu�B
	Vector3 m_rHandBonePos = g_vec3Zero;
	Quaternion m_rHandBoneRot = g_quatIdentity;	
	Vector3 m_move = g_vec3Zero;				//�ړ��B
	float m_speed = 100.0f;						//�ړ����x�B
	int m_flame = 0;
	GameCamera* m_camera = nullptr;				//�J�����B
	myEngine::Effect* m_effect = nullptr;		//�G�t�F�N�g�B
	const float RAY_RANGE = 8000.0f;			
	float m_hp = BASE_PARAM::PLAYER_MAX_HP;		//HP�B
	PlayerUIs* m_playerUIs;						//UI�B
	float m_currentRegeneTime = 0.0f;			//���W�F�l�^�C���B
};

