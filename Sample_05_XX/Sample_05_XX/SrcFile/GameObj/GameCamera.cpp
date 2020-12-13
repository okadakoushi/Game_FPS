#include "stdafx.h"
#include "GameCamera.h"
#include "GamePlayer.h"

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//キャラ映らないように近平面を設定。
	//todo:なんかいい方法あるんだろうか。
	GraphicsEngineObj()->GetCamera3D().SetNear(5.0f);
	m_player = FindGO<GamePlayer>("Player");
	return true;
}

void GameCamera::Update()
{
	if (m_isFPS) {
		MoveCameraOnFPS();
	}
	else {
		MoveCameraOnTPS();
	}
	//1フレームずれる対策でここでRotationさせる。
	//todo:NotFindGO
	m_player->Rotation();
}

void GameCamera::MoveCameraOnFPS()
{
	//視点。
	Vector3 pos = m_playerPos;
	pos.y += 93.0f;
	//古い。
	Vector3 totPosOld = m_toPos;
	//入力に応じて回す。
	//todo:マウスとかでとったりもしてみたいなぁ。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y軸周りの回転作成。
	Quaternion qRot;
	qRot.SetRotationDeg(g_vec3AxisY, HORIZON * x);
	qRot.Apply(m_toPos);

	//X軸周りの回転作成。
	Vector3 axisX;
	axisX.Cross(g_vec3AxisY, m_toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX * -1.0f, VERTICAL * y);
	qRot.Apply(m_toPos);

	//回転上限のチェック。
	Vector3 toTargetDir = m_toPos;
	toTargetDir.Normalize();
	if (toTargetDir.y < -0.5f) {
		//上向きすぎ。
		m_toPos = totPosOld;
	}
	else if (toTargetDir.y > 0.8f) {
		//下向きすぎ。
		m_toPos = totPosOld;
	}

	//カメラの位置補正。MNum
	Vector3 fix = toTargetDir * 20.0f;

	//視点を計算。
	Vector3 target = pos + m_toPos;

	//メインカメラに設定。
	GraphicsEngineObj()->GetCamera3D().SetTarget(target);
	GraphicsEngineObj()->GetCamera3D().SetPosition(pos + fix);
	//更新。
	//GraphicsEngineObj()->GetCamera3D().Update();
}

void GameCamera::MoveCameraOnTPS()
{
	//注視点。
	Vector3 target = m_playerPos;
	target.y += 80.0f;
	//古い。
	Vector3 toPosOld = m_toPos;
	//入力に応じて回す。
	//todo:マウスとかでとったりもしてみたいなぁ。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y軸周りの回転作成。
	Quaternion qRot;
	qRot.SetRotationDeg(g_vec3AxisY, HORIZON * x);
	qRot.Apply(m_toPos);

	//X軸周りの回転作成。
	Vector3 axisX;
	axisX.Cross(g_vec3AxisY, m_toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX * -1.0f, VERTICAL * y);
	qRot.Apply(m_toPos);

	//回転上限のチェック。
	Vector3 toTargetDir = m_toPos;
	toTargetDir.Normalize();
	if (toTargetDir.y < -0.5f) {
		//上向きすぎ。
		m_toPos = toPosOld;
	}
	else if (toTargetDir.y > 0.8f) {
		//下向きすぎ。
		m_toPos = toPosOld;
	}

	//視点を計算。
	Vector3 pos = target + m_toPos;
	//メインカメラに設定。
	GraphicsEngineObj()->GetCamera3D().SetTarget(target);
	GraphicsEngineObj()->GetCamera3D().SetPosition(pos);
	//更新。
	GraphicsEngineObj()->GetCamera3D().Update();
}

