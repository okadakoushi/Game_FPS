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
	GraphicsEngineObj()->GetCamera3D().SetNear(1.0f);
	GraphicsEngineObj()->GetCamera3D().SetViewAngle(70.0f);
	m_player = FindGO<GamePlayer>("Player");
	//頭のボーン取得。
	Skeleton& skeleton = m_player->GetRender()->GetSkelton();
	int headNo = skeleton.FindBoneID(L"mixamorig:Head");
	m_headBone = skeleton.GetBone(headNo);
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
}

void GameCamera::MoveCameraOnFPS()
{
	Quaternion a;
	Vector3 b;
	m_headBone->CalcWorldTRS(m_playerPos, a, b);
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
	if (toTargetDir.y < -0.9f) {
		//上向きすぎ。
		m_toPos = totPosOld;
	}
	else if (toTargetDir.y > 0.9f) {
		//下向きすぎ。
		m_toPos = totPosOld;
	}

	//視点を計算。
	Vector3 target = m_playerPos + m_toPos;
	//pos.y -= 25.0f;
	//視点の位置を補正。
	Vector3 fix = toTargetDir * 15.0f;
	//todo:反動


	//メインカメラに設定。
	GraphicsEngineObj()->GetCamera3D().SetTarget(target);
	GraphicsEngineObj()->GetCamera3D().SetPosition(m_playerPos + fix);
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
	qRot.SetRotationDeg(g_vec3AxisY * 1.0f, HORIZON * x);
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

