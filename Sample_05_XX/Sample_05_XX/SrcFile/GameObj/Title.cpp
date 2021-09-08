#include "stdafx.h"
#include "Title.h"
#include "SrcFile/GameObj/Stages/StageGenerator.h"
#include "SrcFile/GameObj/Stages/Stage.h"
#include "SrcFile/GameObj/GamePlayer.h"
#include "SrcFile/GameObj/GameCamera.h"
#include "SrcFile/nature/SkyBox.h"
#include "SrcFile/Game.h"
#include "SrcFile/GameObj/PlayerUIs.h"
#include "SrcFile/2D/FontRender.h"

Title::Title()
{

}

Title::~Title()
{

}

void Title::OnDestroy()
{
	DeleteGO(m_titleSprite);
	DeleteGO(m_pressFont);
}



bool Title::Start()
{
	//player。EnemyとPlayerの結合度高すぎるから、タイトル時点ではPlayer必要ないが
	//NewGOしておく。ただし更新等は行わない。
	m_player = NewGO<GamePlayer>(EnPriority_3DModel, "Player");
	//遥か上空で待機。title消したらActiveと位置戻すこと。
	m_player->SetPos({ 0.0f, 1000000.0f, 0.0f });
	//上と同じ理由でゲームカメラも。
	m_camera = NewGO<GameCamera>(Enpriority_Camera, "GameCamera");
	//ステージジェネレーターを作成。
	m_stageGenerator = NewGO<StageGenerator>(EnPriority_Generator, "StageGenerator");
	m_stageGenerator->SetPlayer(m_player);
	//ステージ1を生成。
	m_stageGenerator->CreateStage(StageGenerator::EnStageNumber_BattleStage1);
#ifdef MASTER
#endif

	//タイトル初期化。
	SpriteInitData initData;
	initData.m_ddsFilePath[0] = "Assets/sprite/title.dds";
	initData.m_fxFilePath = "Assets/shader/sprite.fx";
	initData.m_width = FRAME_BUFFER_W;
	initData.m_height = FRAME_BUFFER_H;
	m_titleSprite = NewGO<SpriteRender>(EnPriority_2DRender);
	m_titleSprite->Init(initData);
	m_titleSprite->SetPos({ 0.0f,100.0f,0.0f });

	//PressButtonGuide。
	m_pressFont = NewGO<myEngine::CFontRender>(EnPriority_2DRender);
	m_pressFont->SetTextUnsafe(m_pressButtonChar);
	m_pressFont->SetPivot({ 0.5f, 0.5f });
	m_pressFont->SetPosition({ 0.0f, -250.0f });
	m_pressFont->SetShadowParam(true, 3.0f, { 0.0f, 0.0f, 0.0f, 1.0f });

	return true;
}

void Title::Update()
{
	if (m_isonce && m_player->IsStart()) {
		//NewGOした直後フレームで行うとNewGOの仕様上望まない挙動になる。
		m_cameraPos = m_stageGenerator->GetBattleStage1()->GetSecurityCameraPosList().at(m_currentCameraCount);
		m_cameraRot = m_stageGenerator->GetBattleStage1()->GetSecurityCameraRotList().at(m_currentCameraCount);
		for (auto& pos : m_stageGenerator->GetBattleStage1()->GetSecurityCameraPosList()) {
			m_securityCameraCount++;
		}
		//アクティブじゃなくする。
		m_player->SetActive(false);
		m_camera->SetActive(false);
		m_player->GetPlayerUIs()->ActiveProcess(false);
		m_isonce = false;;
	}

	m_changeCameraPosTimer += GameTime().GetFrameDeltaTime();

	if (m_changeCameraPosTimer > CHANGE_CAMERAPOS_TIMER) {
		//指定時間経過。
		if (m_currentCameraCount < m_securityCameraCount - 1) {
			//次のカメラがある。
			m_currentCameraCount++;
		}
		else {
			//なし。
			m_currentCameraCount = 0;
		}
		//位置、回転を取得。
		m_cameraPos = m_stageGenerator->GetBattleStage1()->GetSecurityCameraPosList().at(m_currentCameraCount);
		m_cameraRot = m_stageGenerator->GetBattleStage1()->GetSecurityCameraRotList().at(m_currentCameraCount);
		printf("現在のカメラカウントは %d です。\n", m_securityCameraCount);
		//タイマーリセット。
		m_changeCameraPosTimer = 0.0f;
	}

	Quaternion addRot;
	float angle = m_addAngle;
	m_angle += angle;
	addRot.SetRotationDegY(angle);

	if (m_angle > 60.0f || m_angle < - 60.0f) {
		//指定以上ならアングルを反転。
		m_addAngle *= -1;
	}

	//注視点も計算。
	Vector3 target = g_vec3Left;
	m_cameraRot.Multiply(addRot);
	m_cameraRot.Apply(target);
	target += m_cameraPos;

	GraphicsEngineObj()->GetCamera3D().SetPosition(m_cameraPos);
	GraphicsEngineObj()->GetCamera3D().SetTarget(target);

	if (GetAsyncKeyState(MK_LBUTTON)) {
		m_player->SetActive(true);
		m_camera->SetActive(true);
		m_player->GetPlayerUIs()->ActiveProcess(true);
		m_stageGenerator->GetBattleStage1()->GetPlayerPos();
		DeleteGO(this);
		NewGO<Game>(EnPriority_Game);
	}
}
