#pragma once

#include "SrcFile/Fade.h"

class GamePlayer;
class Guide;
class SoldierMob;
class NaviMesh;

/// <summary>
/// �X�^���o�C�X�e�[�W�N���X�B
/// </summary>
class Stage : public IGameObject
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
	void ForwardRender() override;
	/// <summary>
	/// �v���C���[��ݒ�B
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(GamePlayer* player)
	{
		m_player = player;
	}
private:
	SkinModelRender* m_modelRender[20];			//�����_�[�B
	SoldierMob* m_soldierMob[20];				//���mMob�B
	Level m_level;								//���x���B
	int m_mobCount = 0;							//���u�J�E���g�B
	PhysicsStaticObject m_physicsStaticObject;	//�ÓI�I�u�W�F�N�g�B
	GamePlayer* m_player = nullptr;				//�v���C���[�B
	Guide* m_guide = nullptr;					//�K�C�h�B
	NaviMesh* m_naviMesh = nullptr;				//�i�r�Q�[�V�������b�V���B
	Fade m_fade;								//�t�F�[�h�B
};

