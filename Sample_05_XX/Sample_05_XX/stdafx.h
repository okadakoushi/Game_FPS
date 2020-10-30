#pragma once
#define _CRT_SECURE_NO_WARNINGS

const float DELTA_TIME = 1.0f / 60.0f;	//�X�V���ԁB���GameTime�N���X��邱�ƁB
const int NUM_DIRECTIONAL_LIGHT = 4;	//�f�B���N�V�������C�g�̐��B
static const int NUM_SHADOW_MAP = 3;			//�J�X�P�[�h�V���h�E�}�b�v�̐��B

//�m���R�s�[
#include "SrcFile/util/Noncopyable.h"
//�Q�[���I�u�W�F�N�g�B
#include "SrcFile/GameObject/IGameObject.h"

#include "MiniEngine.h"

#include "SrcFile/GameObject/GameObjectManager.h"

//�X�L�����f���B
#include "SkinModelRender.h"

//�V���h�E�}�b�v�B
#include "SrcFile/Shadow/DirectionalShadowMap.h";

using namespace std;

/// <summary>
/// �f�B���N�V�������C�g�B
/// </summary>
struct DirectionalLight {
	Vector3 direction;	//���C�g�̕����B
	float pad0;			//�p�f�B���O�B
	Vector4 color;		//���C�g�̃J���[�B
};
/// <summary>
/// ���C�g�\���́B
/// </summary>
struct Light {
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];	//�f�B���N�V�������C�g�B
	Vector3 eyePos;					//�J�����̈ʒu�B
	float specPow;					//�X�y�L�����̍i��B
	Vector3 ambinetLight;			//�����B
};
