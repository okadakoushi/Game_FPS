#pragma once

class Rifle;
class GamePlayer;
class SoldierCollision;

class IEnemyState;
class EnemyAttackState;
class EnemyDamageState;
class EnemyDethState;
class EnemyTrackingState;
class EnemyWanderingState;

//#include "IEnemy.h"
#include "SrcFile/GameObj/SoldierCollision.h"

//#include "EnemyState/IEnemyState.h"
//#include "EnemyState/EnemyAttackState.h"
//#include "EnemyState/EnemyDamageState.h"
//#include "EnemyState/EnemyDethState.h"
//#include "EnemyState/EnemyTrackingState.h"
//#include "EnemyState/EnemyWanderingState.h"

enum EnEnemyAnimation {
	EnEnemyAnimation_Walk,
	EnEnemyAnimation_Run,
	EnEnemyAnimation_Damage,
	EnEnemyAnimation_Shoot,
	EnEnemyAnimation_Death,
};

class RifleEnemy : public IGameObject
{
public:
	~RifleEnemy();
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �ړ��B
	/// </summary>
	/// <remarks>
	///	���X�e�[�g�N���X������Q�Ƃ���邽��public�B
	/// </remarks>
	void Move();
	/// <summary>
	/// �v���C���[�𔭌����Ă��邩�B
	/// </summary>
	/// <returns></returns>
	bool IsFindPlayer();
	/// <summary>
	/// �X�e�[�g��ݒ�B
	/// </summary>
	/// <param name="state"></param>
	void ChangeState(IEnemyState* state);
	/// <summary>
	/// �_���[�W���󂯂�B
	/// </summary>
	void GetDamage();
	/// <summary>
	/// �ʒu��ݒ�B
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(Vector3& pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// �p�X��o�^�B
	/// </summary>
	/// <param name="path"></param>
	void RegistPath(const Vector3& path)
	{
		m_paths.push_back(path);
	}
	/// <summary>
	/// �ʒu���擾�B
	/// </summary>
	/// <returns></returns>
	Vector3& GetPosition()
	{
		return m_pos;
	}
	/// <summary>
	/// ��]���擾�B
	/// </summary>
	/// <returns></returns>
	Quaternion& GetRotation()
	{
		return m_rot;
	}
	/// <summary>
	/// ���f�������_�[���擾�B
	/// </summary>
	/// <returns></returns>
	SkinModelRender* GetRender()
	{
		return m_modelRender;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	NaviMeshAgent& GetAgent()
	{
		return m_agent;
	}
	/// <summary>
	/// �i�r���b�V����ݒ�B
	/// </summary>
	/// <param name="mesh"></param>
	void SetNaviMesh(NaviMesh* mesh)
	{
		m_naviMesh = mesh;
	}
	/// <summary>
	/// �v���C���[�����Ă��邩�̃t���O��ݒ�B
	/// </summary>
	/// <param name="flag"></param>
	void SetFindPlayerFlag(const bool& flag)
	{
		m_isMissingPlayer = flag;
	}
	/// <summary>
	/// �v���C���[�̃|�C���^���擾�B
	/// </summary>
	/// <returns>player</returns>
	GamePlayer* GetPlayerPtr()
	{
		return m_player;
	}
	/// <summary>
	/// ���̈ʒu���擾�B
	/// </summary>
	/// <returns></returns>
	Vector3& GetHeadPos()
	{
		return m_headPos;
	}
	/// <summary>
	/// ������擾�B
	/// </summary>
	/// <returns></returns>
	Rifle* GerRifle()
	{
		return m_rifle;
	}
	/// <summary>
	/// �q�b�g�|�C���g���擾�B
	/// </summary>
	/// <returns></returns>
	int& GetHP()
	{
		return m_hp;
	}
	/// <summary>
	/// ���������t���O��ݒ�B
	/// </summary>
	/// <param name="flag"></param>
	void SetIsMissingFlag(const bool& flag)
	{
		m_isMissingPlayer = flag;
	}
	/// <summary>
	/// �R���W�������擾�B
	/// </summary>
	/// <returns></returns>
	SoldierCollision& GetCollision()
	{
		return m_collision;
	}
protected:
	Rifle* m_rifle = nullptr;				//�e�B
	SoldierCollision m_collision;	
	int m_pathIndex = 0;						//
	float m_FOV = 40.0f;						//����p�B
	float m_vision = 700.0f;					//�G�l�~�[�̎���͈́B
	Bone* m_head;								//���{�[���B
	int m_hp = 100;								//HP�B
	bool m_isMissingPlayer = false;				//�v���C���[�����������B
	Vector3 m_toPlayerDir = g_vec3Zero;			//�G�l�~�[�̃w�b�h����v���C���[�ɐL�т�x�N�g���B
	Vector3 m_headPos = g_vec3Zero;				//�w�b�h�̈ʒu�B
	NaviMeshAgent m_agent;						//�o�H�T��AI�B
	std::vector<Vector3> m_paths;				//�p�X�B
	//��{Param�B
	SkinModelRender* m_modelRender;							//���f�������_�[�B
	Vector3							m_pos = { 50.0f, 0.0f, 0.0f };			//�ʒu�B
	Quaternion						m_rot = g_quatIdentity;					//��]�B
	Vector3							m_scale = { 1.5f, 1.5f, 1.5f };			//�g��B					
	NaviMesh* m_naviMesh = nullptr;
	SoundSource* m_findSE;						//������
	SoundSource* m_footStep = nullptr;			//�����B
	GamePlayer* m_player = nullptr;				//�v���C���[�B
	SpriteRender* m_findMark = nullptr;			//�I�}�[�N�B
	float m_rayTestTimer = 0.0f;				//���݂̃��C�e�X�g�^�C�}�[�B
	const float RAY_TEST_INTERVAL = 0.5f;		//���C�e�g���s���Ԋu�B

protected:
	IEnemyState*		m_enemyState;			//�X�e�[�g�B
	EnemyAttackState*	m_attackState;			//�U��
	EnemyDamageState*	m_damageState;			//�_���[�W
	EnemyDethState*		m_deathState;			//���S�B
	EnemyWanderingState* m_wanderingState;		//�p�j�B
	EnemyTrackingState*	m_trackingState;		//�ǐՁB
};

