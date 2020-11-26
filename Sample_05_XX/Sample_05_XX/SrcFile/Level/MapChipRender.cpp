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
	//レンダー初期化。
	m_modelRender = NewGO<SkinModelRender>(0);
	//パス作成。
	char filePath[256];
	//頭のファイルパスを持ってくる。
	//todo:ここ型がおかしい。直す。
	sprintf_s(filePath, "modelData/%s.tkm", m_renderObjDatas[0].name);

	/*
	todo:ここにインスタンシング描画の処理を実装する。
	インスタンシング描画と通常描画の差別化もしてね！
	*/

	//通常描画
	m_modelRender->Init(filePath);
	m_modelRender->SetPosition(m_renderObjDatas[0].position);
	m_modelRender->SetRotation(m_renderObjDatas[0].rotatatin);
	m_modelRender->SetScale(m_renderObjDatas[0].scale);
}
