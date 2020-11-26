#pragma once

#include "MapChipRender.h"

struct LevelObjectData;
class MapChipRender;


/// <summary>
/// マップチップ。
/// </summary>
class MapChip : Noncopyable
{
public:
	MapChip(const LevelObjectData& objData, MapChipRender* mapChipRender);
	~MapChip()
	{
	}
	/// <summary>
	/// モデルレンダー取得。
	/// </summary>
	SkinModelRender* GetSkinModelRender()
	{
		if (m_mapChipRender == nullptr) {
			//マップチップレンダーないからいらない。
			return nullptr;
		}
		//todo:レンダー取得。
		return m_mapChipRender->GetModelRender();
	}
private:
	MapChipRender* m_mapChipRender = nullptr;		//マップチップレンダー。
};

