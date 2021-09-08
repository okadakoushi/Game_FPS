#pragma once

#include "SrcFile/Fade.h"

class Stage;
class BattleStage1;
class GamePlayer;
class SkyBox;
class IStage;

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
		EnStageNumber_BattleStage1,	//�X�^���o�C
		EnStageNumber_BattleStage2,	//�o�g��1
		EnStageNumber_BattleStage3,	//�o�g��2
		EnStageNumber_BattleStage4,	//�o�g��3
		EnstageNumber_None,
		EnstageNumber_StageCount	//�X�e�[�W��
	};
public:
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �t�F�[�h���s���B
	/// <para>Update���̍X�V�֐��ŌĂяo�����ƁB</para>
	/// </summary>
	/// <param name="fadeIn">�t�F�[�h�C���H</param>
	/// <returns>�t�F�[�h���I���������B</returns>
	bool FadeProcess(const bool& fadeIn);
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
	void SetPlayerRespawnPos();
	/// <summary>
	/// ���݂̃X�e�[�W�ԍ����擾�B
	/// </summary>
	/// <returns></returns>
	const StageNumber& GetCurrentStageNumber() const 
	{
		return m_currentStageNum;
	}
	/// <summary>
	/// �o�g���X�e�[�W1���擾�B
	/// </summary>
	/// <returns></returns>
	Stage* GetBattleStage1()
	{
		//�|�����[�t�B�Y�����p�Ŕh���N���X�Ɍ^�ϊ��B
		return reinterpret_cast<Stage*>(m_stage);
	}
private:
	//��p�̃t�@�C���p�X�B
	std::wstring m_skyBoxFilePaths[EnstageNumber_StageCount]
	{
		L"Assets/modelData/nature/skyCubeMapNight_Toon_02.dds",	//S1:Night_Toon
		L"Assets/modelData/nature/skyCubeMapSpace_Toon_2.dds",	//S2:Evening_Toon
		L"Assets/modelData/nature/skyCubeMap.dds",				//S1:Day_Toon
		L"Assets/modelData/nature/skyCubeMapDay_Toon_03.dds"	//S2:Day_Toon �����͖v�\��B
	};
	struct StageLigPram {
		Vector4 DirectionLightColor;	//DLig�̃J���[�B
		Vector3 DirectionLightDir;		//DLig�̕����B
		float SkySelfLuminous;			//�X�J�C�̎��Ȕ���param
		float IBLItensity;				//IBL�̋P�x�B
		float MiddleGray;				//�g�[���}�b�v�~�h���O���C�B
	};

	StageLigPram m_ligParams[EnstageNumber_StageCount];	//�X�e�[�W�p���C�g�̃p�����[�^�[�B
private:
	StageNumber m_currentStageNum = EnstageNumber_None;	//�������X�e�[�W�ԍ��B
	GamePlayer* m_player = nullptr;						//playerPtr�B
	Fade* m_fade;										//fade�B
	SoundSource* m_windSE = nullptr;					//���̉��B						
	bool m_firstFadeCall = true;						//���ڂ̃t�F�[�h�̃R�[���B
	bool m_isChangeStage = false;						//�X�e�[�W��ς��邩�B
	bool m_isStageDeleteCall = false;					//�X�e�[�W�̃f���[�g�R�[�����Ă΂ꂽ
	SkyBox* m_sky = nullptr;							//��B
	IStage* m_stage = nullptr;							//�X�e�[�W�B�|�����[�t�B�Y�����p�B
};

