#pragma once

#include "SrcFile/Fade.h"
#include "IStage.h"

class GamePlayer;
class Guide;
class SoldierMob;
class NaviMesh;
class RifleEnemy;
class StageGenerator;

/// <summary>
/// �X�^���o�C�X�e�[�W�N���X�B
/// </summary>
class Stage : public IStage
{
public:
	Stage();
	~Stage();
	/// <summary>
	/// �폜�B
	/// </summary>
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
	/// �t�H���[�h�����_�����O�B
	/// </summary>
	void ForwardRender() override;
	/// <summary>
	/// �v���C���[��ݒ�B
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(GamePlayer* player)
	{
		m_player = player;
	}
	/// <summary>
	/// �Z�L�����e�B�[�J�����̈ʒu���X�g���擾�B
	/// </summary>
	/// <returns></returns>
	std::vector<Vector3>& GetSecurityCameraPosList()
	{
		return m_securityCameraPosList;
	}
	/// <summary>
	/// �Z�L�����e�B�J�����̉�]���X�g���擾�B
	/// </summary>
	/// <returns></returns>
	std::vector<Quaternion>& GetSecurityCameraRotList()
	{
		return m_securityCameraRotList;
	}
	/// <summary>
	/// �v���C���[�̃��X�|�[���ʒu���擾�B
	/// </summary>
	/// <returns></returns>
	Vector3& GetPlayerPos()
	{
		return m_playerRootPos;
	}
private:
	static const int m_enemyCount = 5;					//���݂̃G�l�~�[�̐��B			
	SkinModelRender* m_modelRender[20];					//�����_�[�B
	RifleEnemy* m_rifleEnemys[m_enemyCount];			//���C�t���G�l�~�[�B
	Level m_level;										//���x���B
	PhysicsStaticObject m_physicsStaticObject;			//�ÓI�I�u�W�F�N�g�B
	Vector3 m_playerRootPos = g_vec3Zero;				//���x��������������ė����v���C���[�̃��X�|�[���ʒu�Btitle����̑J�ڂ̓s����ۑ����Ă����B
	std::vector<Vector3> m_securityCameraPosList;		//�Ď��J�����̈ʒu���X�g�B
	std::vector<Quaternion> m_securityCameraRotList;	//�Ď��J�����̌������X�g�B
	//GamePlayer* m_player = nullptr;					//�v���C���[�B
	//NaviMesh* m_naviMesh = nullptr;					//�i�r�Q�[�V�������b�V���B
	//StageGenerator* m_stageGenerator = nullptr;		//�X�e�[�W�W�F�l���[�^�[�B

};

