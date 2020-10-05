#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "MiniEngine.h"

//ノンコピー
#include "SrcFile/util/Noncopyable.h"

//ゲームオブジェクト。
#include "SrcFile/GameObject/IGameObject.h"
#include "SrcFile/GameObject/GameObjectManager.h"

using namespace std;

const float DELTA_TIME = 1.0f / 60.0f;	//更新時間。後でGameTimeクラス作ること。
const int NUM_DIRECTIONAL_LIGHT = 4;	//ディレクションライトの数。
const int NUM_SHADOW_MAP = 3;			//カスケードシャドウマップの数。

/// <summary>
/// ディレクションライト。
/// </summary>
struct DirectionalLight {
	Vector3 direction;	//ライトの方向。
	float pad0;			//パディング。
	Vector4 color;		//ライトのカラー。
};
/// <summary>
/// ライト構造体。
/// </summary>
struct Light {
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];	//ディレクションライト。
	Vector3 eyePos;					//カメラの位置。
	float specPow;					//スペキュラの絞り。
	Vector3 ambinetLight;			//環境光。
};
