#pragma once

class Rifle;

#include "IEnemy.h"
#include "SrcFile/GameObj/SoldierCollision.h"

enum EnEnemyState {
	EnEnemyState_Wandering,		//�p�j�B
	EnEnemyState_Tracking,		//�ǐՁB
	EnEnemyState_Attack,		//�U���B
	EnEnemyState_AttackWait,	//�U���ҋ@���ԁB
	EnEnemyState_Damage,
	EnEnemyState_Death,			//���S�B
	EnEnemyState_Num
};

class RifleEnemy : public Enemy
{
public:
	bool Start() override;
	void Update() override;
	void Move();
	void Attack();
	void Tracking();
	void Damage();
	void Death();
	/// <summary>
	/// �v���C���[�𔭌����Ă��邩�B
	/// </summary>
	/// <returns></returns>
	bool IsFindPlayer();
	/// <summary>
	/// �X�e�[�g��ݒ�B
	/// </summary>
	/// <param name="state"></param>
	void SetState(const EnEnemyState& state)
	{
		m_enemyState = state;
	}
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
private:
	Rifle* m_rifle = nullptr;				//�e�B
	float m_currentTime = 0.0f;				//�^�C�}�[�B
	float COOLDOWN = 0.19f;					//�N�[���_�E���B
	SoldierCollision m_collision;	
	EnEnemyState m_enemyState = EnEnemyState_Wandering;	//�G�l�~�[�̃X�e�[�g�B
	std::vector<Vector3> m_paths;				//�T���p�X�Btodo:�z��B
	int m_pathIndex = 0;						//
	const float m_FOV = 20.0f;					//����p�B
	const float m_VISION = 1500.0f;				//�G�l�~�[�̎���͈́B
	Bone* m_head;								//���{�[���B
	Vector3 m_toNextCell;						//���̃Z���Ɍ��������ʃx�N�g���B
	int m_hp = 100;								//HP�B
	const int MAX_RANDOM_AIM = 120;				//�G�l�~�[��AIM�̍ő嗐��l�B
	int m_currentRondomAIM = MAX_RANDOM_AIM;	//�G�l�~�[�̌��݂�AIM�̗���l�B
};

