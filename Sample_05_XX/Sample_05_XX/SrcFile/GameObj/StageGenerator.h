#pragma once

#include "SrcFile/Fade.h"

class Stage;
class BattleStage1;
class GamePlayer;

/// <summary>
/// ���x���Ǘ��I�u�W�F�N�g�B
/// </summary>
class StageGenerator : public IGameObject
{
public:
	StageGenerator();
	~StageGenerator() {}
public:
	enum StageNumber {
		EnstageNumber_None,			//�Ȃɂ��Ȃ�
		EnStageNumber_BattleStage1,	//�X�^���o�C
		EnStageNumber_BattleStage2,	//�o�g��1
		EnStageNumber_BattleStage3,	//�o�g��2
		EnStageNumber_BattleStage4,	//�o�g��3
		EnstageNumber_StageCount	//�X�e�[�W��
	};
public:
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �w�肳�ꂽ�X�e�[�W���쐬�B
	/// </summary>
	/// <param name="stageNum">��������X�e�[�W�ԍ��B</param>
	void CreateStage(const StageNumber& stageNum);
	/// <summary>
	/// ���ݐ������Ă���X�e�[�W���폜�B
	/// </summary>
	void DeleteCurrentStage();
	/// <summary>
	/// �v���C���[��ݒ�B
	/// </summary>
	/// <param name="pl"></param>
	void SetPlayer(GamePlayer* pl) {
		m_player = pl;
	}
	/// <summary>
	/// ���݂̃X�e�[�W�ԍ����擾�B
	/// </summary>
	/// <returns></returns>
	const StageNumber& GetCurrentStageNumber() const 
	{
		return m_currentStageNum;
	}
private:
	StageNumber m_currentStageNum = EnstageNumber_None;			//�������X�e�[�W�ԍ��B
	Stage* m_stanbyStage = nullptr;				//�X�^���o�C�X�e�[�W�B
	BattleStage1* m_battleStage = nullptr;		//�o�g���X�e�[�W�B
	GamePlayer* m_player = nullptr;
	Fade* m_fade;
	SoundSource* m_windSE = nullptr;						//���̉��B
};

