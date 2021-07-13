#include "stdafx.h"
#include "MapChipRender.h"
#include "Level.h"
#include <mbstring.h>
#include "SrcFile/Physics/CollisionAttr.h"

MapChipRender::MapChipRender()
{
}

MapChipRender::~MapChipRender()
{
	Release();
}

void MapChipRender::Release()
{
	DeleteGO(m_modelRender);
	m_physicsStaticObject.Release();
}

void MapChipRender::Update()
{
}

void MapChipRender::InitRenderObject()
{
	//レンダー初期化。
	m_modelRender = NewGO<SkinModelRender>(EnPriority_3DModel);
	//パス作成。
	wchar_t filePath[256];
	swprintf_s(filePath, L"Assets/modelData/levelTkm/%s.tkm", m_renderObjDatas[0].name);

	//マルチバイトに変換。
	//ベースの文字列。
	size_t origsize = wcslen(filePath) + 1;
	//変換した文字列。
	size_t convertedChars = 0;
	//マルチバイト格納用。
	char strCon[] = "";
	//サイズ。
	size_t strConSize = (strlen(strCon) + 1) * 2;
	//変換後のサイズ。
	const size_t newsize = origsize * 2;
	//連結後のサイズ。
	char* nstirng = new char[newsize + strConSize];
	//マルチバイトに変換。_TRUNCATE 入りきらないのは切り捨て。
	wcstombs_s(&convertedChars, nstirng, newsize, filePath, _TRUNCATE);
	//文字列連結。
	_mbscat_s((unsigned char*)nstirng, newsize + strConSize, (unsigned char*)strCon);

	//モデルのセット。
	m_modelRender->Init(nstirng);
	m_modelRender->SetPosition(m_renderObjDatas[0].position);
	m_modelRender->SetRotation(m_renderObjDatas[0].rotatatin);
	m_modelRender->SetScale(m_renderObjDatas[0].scale);
	//if (m_renderObjDatas[0].isShadowCaster) {
		//シャドウキャスターじゃ！
	m_modelRender->SetShadwoCaster(true);
	//}
	//if (m_renderObjDatas[0].isShadowReceiver) {
		//シャドウレシーバーじゃ！
	m_modelRender->SetShadowReciever(true);
	//}
	//当たり判定追加。
	m_physicsStaticObject.CreateMesh(m_modelRender->GetPosition(), m_modelRender->GetRotation(), m_modelRender->GetScale(), m_modelRender);
	m_physicsStaticObject.GetRigidBody().GetBody()->setUserIndex(enCollisionAttr_StaticObject);

	/*
	todo:ここにインスタンシング描画の処理を実装する。
	インスタンシング描画と通常描画の差別化もしてね！
	*/

	//通常描画
	//m_modelRender->Init(filePath);
	//m_modelRender->SetPosition(m_renderObjDatas[0].position);
	//m_modelRender->SetRotation(m_renderObjDatas[0].rotatatin);
	//m_modelRender->SetScale(m_renderObjDatas[0].scale);
}
