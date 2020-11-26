#include "stdafx.h"
#include "MapChipRender.h"
#include "Level.h"

MapChipRender::MapChipRender()
{
}

MapChipRender::~MapChipRender()
{
}

void MapChipRender::OnDestroy()
{
	DeleteGO(m_modelRender);
}

void MapChipRender::Update()
{
}

void MapChipRender::InitRenderObject()
{
	//�����_�[�������B
	m_modelRender = NewGO<SkinModelRender>(0);
	//�p�X�쐬�B
	char filePath[256];
	//���̃t�@�C���p�X�������Ă���B
	//todo:�����^�����������B�����B
	sprintf_s(filePath, "modelData/%s.tkm", m_renderObjDatas[0].name);

	/*
	todo:�����ɃC���X�^���V���O�`��̏�������������B
	�C���X�^���V���O�`��ƒʏ�`��̍��ʉ������ĂˁI
	*/

	//�ʏ�`��
	m_modelRender->Init(filePath);
	m_modelRender->SetPosition(m_renderObjDatas[0].position);
	m_modelRender->SetRotation(m_renderObjDatas[0].rotatatin);
	m_modelRender->SetScale(m_renderObjDatas[0].scale);
}
