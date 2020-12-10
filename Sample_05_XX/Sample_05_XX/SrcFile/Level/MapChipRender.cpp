#include "stdafx.h"
#include "MapChipRender.h"
#include "Level.h"
#include <mbstring.h>

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
	m_modelRender = NewGO<SkinModelRender>(EnPriority_Render);
	//�p�X�쐬�B
	wchar_t filePath[256];
	swprintf_s(filePath, L"Assets/modelData/levelTkm/%s.tkm", m_renderObjDatas[0].name);

	//�}���`�o�C�g�ɕϊ��B
	//�x�[�X�̕�����B
	size_t origsize = wcslen(filePath) + 1;
	//�ϊ�����������B
	size_t convertedChars = 0;
	//�}���`�o�C�g�i�[�p�B
	char strCon[] = "";
	//�T�C�Y�B
	size_t strConSize = (strlen(strCon) + 1) * 2;
	//�ϊ���̃T�C�Y�B
	const size_t newsize = origsize * 2;
	//�A����̃T�C�Y�B
	char* nstirng = new char[newsize + strConSize];
	//�}���`�o�C�g�ɕϊ��B_TRUNCATE ���肫��Ȃ��̂͐؂�̂āB
	wcstombs_s(&convertedChars, nstirng, newsize, filePath, _TRUNCATE);
	//������A���B
	_mbscat_s((unsigned char*)nstirng, newsize + strConSize, (unsigned char*)strCon);

	//���f���̃Z�b�g�B
	m_modelRender->Init(nstirng);
	m_modelRender->SetPosition(m_renderObjDatas[0].position);
	m_modelRender->SetRotation(m_renderObjDatas[0].rotatatin);
	m_modelRender->SetScale(m_renderObjDatas[0].scale);
	//if (m_renderObjDatas[0].isShadowCaster) {
		//�V���h�E�L���X�^�[����I
	m_modelRender->SetShadwoCaster(true);
	//}
	//if (m_renderObjDatas[0].isShadowReceiver) {
		//�V���h�E���V�[�o�[����I
	m_modelRender->SetShadowReciever(true);
	//}


	/*
	todo:�����ɃC���X�^���V���O�`��̏�������������B
	�C���X�^���V���O�`��ƒʏ�`��̍��ʉ������ĂˁI
	*/

	//�ʏ�`��
	//m_modelRender->Init(filePath);
	//m_modelRender->SetPosition(m_renderObjDatas[0].position);
	//m_modelRender->SetRotation(m_renderObjDatas[0].rotatatin);
	//m_modelRender->SetScale(m_renderObjDatas[0].scale);
}
