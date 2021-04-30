#include "stdafx.h"
#include "GameCamera.h"
#include "GamePlayer.h"

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//�L�����f��Ȃ��悤�ɋߕ��ʂ�ݒ�B
	//todo:�Ȃ񂩂������@����񂾂낤���B
	GraphicsEngineObj()->GetCamera3D().SetNear(1.0f);
	GraphicsEngineObj()->GetCamera3D().SetViewAngle(70.0f);
	m_player = FindGO<GamePlayer>("Player");
	//���̃{�[���擾�B
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
	//�Â��B
	Vector3 totPosOld = m_toPos;
	//���͂ɉ����ĉ񂷁B
	//todo:�}�E�X�Ƃ��łƂ���������Ă݂����Ȃ��B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y������̉�]�쐬�B
	Quaternion qRot;
	qRot.SetRotationDeg(g_vec3AxisY, HORIZON * x);
	qRot.Apply(m_toPos);

	//X������̉�]�쐬�B
	Vector3 axisX;
	axisX.Cross(g_vec3AxisY, m_toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX * -1.0f, VERTICAL * y);
	qRot.Apply(m_toPos);

	//��]����̃`�F�b�N�B
	Vector3 toTargetDir = m_toPos;
	toTargetDir.Normalize();
	if (toTargetDir.y < -0.9f) {
		//����������B
		m_toPos = totPosOld;
	}
	else if (toTargetDir.y > 0.9f) {
		//�����������B
		m_toPos = totPosOld;
	}

	//���_���v�Z�B
	Vector3 target = m_playerPos + m_toPos;
	//pos.y -= 25.0f;
	//���_�̈ʒu��␳�B
	Vector3 fix = toTargetDir * 15.0f;
	//todo:����


	//���C���J�����ɐݒ�B
	GraphicsEngineObj()->GetCamera3D().SetTarget(target);
	GraphicsEngineObj()->GetCamera3D().SetPosition(m_playerPos + fix);
}

void GameCamera::MoveCameraOnTPS()
{
	//�����_�B
	Vector3 target = m_playerPos;
	target.y += 80.0f;
	//�Â��B
	Vector3 toPosOld = m_toPos;
	//���͂ɉ����ĉ񂷁B
	//todo:�}�E�X�Ƃ��łƂ���������Ă݂����Ȃ��B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y������̉�]�쐬�B
	Quaternion qRot;
	qRot.SetRotationDeg(g_vec3AxisY * 1.0f, HORIZON * x);
	qRot.Apply(m_toPos);

	//X������̉�]�쐬�B
	Vector3 axisX;
	axisX.Cross(g_vec3AxisY, m_toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX * -1.0f, VERTICAL * y);
	qRot.Apply(m_toPos);

	//��]����̃`�F�b�N�B
	Vector3 toTargetDir = m_toPos;
	toTargetDir.Normalize();
	if (toTargetDir.y < -0.5f) {
		//����������B
		m_toPos = toPosOld;
	}
	else if (toTargetDir.y > 0.8f) {
		//�����������B
		m_toPos = toPosOld;
	}

	//���_���v�Z�B
	Vector3 pos = target + m_toPos;
	//���C���J�����ɐݒ�B
	GraphicsEngineObj()->GetCamera3D().SetTarget(target);
	GraphicsEngineObj()->GetCamera3D().SetPosition(pos);
	//�X�V�B
	GraphicsEngineObj()->GetCamera3D().Update();
}

