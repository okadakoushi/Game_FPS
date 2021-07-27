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
	/// �v���C���[��Find���Ď擾�B
	/// <para>������g�p���Ȃ��悤�ɁB</para>
	/// </summary>
	/// <returns></returns>
	GamePlayer* GetPlayerForUseFind() const;
	friend class EnemyAttackState;
	friend class EnemyDamageState;
	friend class EnemyDethState;
	friend class EnemyWanderingState;
	friend class EnemyTrackingState;
protected:
	Rifle* m_rifle = nullptr;				//�e�B
	float m_currentTime = 0.0f;				//�^�C�}�[�B
	float COOLDOWN = 0.22f;					//�N�[���_�E���B
	SoldierCollision m_collision;	
	int m_pathIndex = 0;						//
	const float m_FOV = 40.0f;					//����p�B
	const float m_VISION = 2500.0f;				//�G�l�~�[�̎���͈́B
	Bone* m_head;								//���{�[���B
	int m_hp = 100;								//HP�B
	const int ATTACK = 10;						//�U���́B
	const int MAX_RANDOM_AIM = 120;				//�G�l�~�[��AIM�̍ő嗐��l�B
	int m_currentRondomAIM = MAX_RANDOM_AIM;	//�G�l�~�[�̌��݂�AIM�̗���l�B
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
	//
	GamePlayer* m_player = nullptr;						//�v���C���[�B
private:
	IEnemyState*		m_enemyState;			//�X�e�[�g�B
	EnemyAttackState*	m_attackState;			//�U��
	EnemyDamageState*	m_damageState;			//�_���[�W
	EnemyDethState*		m_deathState;			//���S�B
	EnemyWanderingState* m_wanderingState;		//�p�j�B
	EnemyTrackingState*	m_trackingState;		//�ǐՁB
};

