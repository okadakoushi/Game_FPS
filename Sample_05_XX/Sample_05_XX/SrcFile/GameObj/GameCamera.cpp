#include "stdafx.h"
#include "GameCamera.h"

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{

	return true;
}

void GameCamera::Update()
{
	GraphicsEngineObj()->GetCamera3D().GetPosition();
}

