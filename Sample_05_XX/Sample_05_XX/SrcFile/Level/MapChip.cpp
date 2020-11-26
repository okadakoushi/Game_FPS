#include "stdafx.h"
#include "MapChip.h"
#include "MapChipRender.h"
#include "Level.h"

MapChip::MapChip(const LevelObjectData& objData, MapChipRender* mapChipRender)
{
	char objName[256];
	wcstombs(objName, objData.name, 256);

	//ファイルパス作成。
	wchar_t filePath[256];
	swprintf_s(filePath, L"modelData/%s.tkm", objData.name);
	m_mapChipRender = mapChipRender;
}
