#pragma once

#include "SrcFile/Fade.h"

class Stage;
class BattleStage1;
class GamePlayer;
class SkyBox;

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
		EnstageNumber_None,			//なにもなし
		EnStageNumber_BattleStage1,	//スタンバイ
		EnStageNumber_BattleStage2,	//バトル1
		EnStageNumber_BattleStage3,	//バトル2
		EnStageNumber_BattleStage4,	//バトル3
		EnstageNumber_StageCount	//ステージ数
	};
public:
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
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
	/// バトルステージ1を取得「。
	/// </summary>
	/// <returns></returns>
	Stage* GetBattleStage1()
	{
		return m_stanbyStage;
	}
private:
	StageNumber m_currentStageNum = EnstageNumber_None;			//生成中ステージ番号。
	Stage* m_stanbyStage = nullptr;				//スタンバイステージ。
	BattleStage1* m_battleStage = nullptr;		//バトルステージ。
	GamePlayer* m_player = nullptr;
	Fade* m_fade;
	SoundSource* m_windSE = nullptr;						//風の音。
	Vector3 m_playerPos;
	bool m_firstFadeCall = true;					//一回目のフェードのコール。
	bool m_isChangeStage = false;					//ステージを変えるか。
	bool m_isStageDeleteCall = false;				//ステージのデリートコールが呼ばれた
	SkyBox* m_sky = nullptr;			//空。
};

