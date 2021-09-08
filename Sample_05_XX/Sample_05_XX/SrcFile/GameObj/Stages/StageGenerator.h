#pragma once

#include "SrcFile/Fade.h"

class Stage;
class BattleStage1;
class GamePlayer;
class SkyBox;
class IStage;

/// <summary>
/// レベル管理オブジェクト。
/// </summary>
class StageGenerator : public IGameObject
{
public:
	StageGenerator();
	~StageGenerator() {}
public:
	enum StageNumber {
		EnStageNumber_BattleStage1,	//スタンバイ
		EnStageNumber_BattleStage2,	//バトル1
		EnStageNumber_BattleStage3,	//バトル2
		EnStageNumber_BattleStage4,	//バトル3
		EnstageNumber_None,
		EnstageNumber_StageCount	//ステージ数
	};
public:
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// フェードを行う。
	/// <para>Update等の更新関数で呼び出すこと。</para>
	/// </summary>
	/// <param name="fadeIn">フェードイン？</param>
	/// <returns>フェードが終了したか。</returns>
	bool FadeProcess(const bool& fadeIn);
	/// <summary>
	/// 指定されたステージを作成。
	/// </summary>
	/// <param name="stageNum">生成するステージ番号。</param>
	void CreateStage(const StageNumber& stageNum);
	/// <summary>
	/// 現在生成しているステージを削除。
	/// </summary>
	void DeleteCurrentStage();
	/// <summary>
	/// プレイヤーを設定。
	/// </summary>
	/// <param name="pl"></param>
	void SetPlayer(GamePlayer* pl) {
		m_player = pl;
	}
	void SetPlayerRespawnPos();
	/// <summary>
	/// 現在のステージ番号を取得。
	/// </summary>
	/// <returns></returns>
	const StageNumber& GetCurrentStageNumber() const 
	{
		return m_currentStageNum;
	}
	/// <summary>
	/// バトルステージ1を取得。
	/// </summary>
	/// <returns></returns>
	Stage* GetBattleStage1()
	{
		//ポリモーフィズム利用で派生クラスに型変換。
		return reinterpret_cast<Stage*>(m_stage);
	}
private:
	//空用のファイルパス。
	std::wstring m_skyBoxFilePaths[EnstageNumber_StageCount]
	{
		L"Assets/modelData/nature/skyCubeMapNight_Toon_02.dds",	//S1:Night_Toon
		L"Assets/modelData/nature/skyCubeMapSpace_Toon_2.dds",	//S2:Evening_Toon
		L"Assets/modelData/nature/skyCubeMap.dds",				//S1:Day_Toon
		L"Assets/modelData/nature/skyCubeMapDay_Toon_03.dds"	//S2:Day_Toon こいつは没予定。
	};
	struct StageLigPram {
		Vector4 DirectionLightColor;	//DLigのカラー。
		Vector3 DirectionLightDir;		//DLigの方向。
		float SkySelfLuminous;			//スカイの自己発光param
		float IBLItensity;				//IBLの輝度。
		float MiddleGray;				//トーンマップミドルグレイ。
	};

	StageLigPram m_ligParams[EnstageNumber_StageCount];	//ステージ用ライトのパラメーター。
private:
	StageNumber m_currentStageNum = EnstageNumber_None;	//生成中ステージ番号。
	GamePlayer* m_player = nullptr;						//playerPtr。
	Fade* m_fade;										//fade。
	SoundSource* m_windSE = nullptr;					//風の音。						
	bool m_firstFadeCall = true;						//一回目のフェードのコール。
	bool m_isChangeStage = false;						//ステージを変えるか。
	bool m_isStageDeleteCall = false;					//ステージのデリートコールが呼ばれた
	SkyBox* m_sky = nullptr;							//空。
	IStage* m_stage = nullptr;							//ステージ。ポリモーフィズム利用。
};

