#pragma once

#include "MapChipRender.h"

struct LevelObjectData;
class MapChipRender;


/// <summary>
/// �}�b�v�`�b�v�B
/// </summary>
class MapChip : Noncopyable
{
public:
	MapChip(const LevelObjectData& objData, MapChipRender* mapChipRender);
	~MapChip()
	{
	}
	/// <summary>
	/// ���f�������_�[�擾�B
	/// </summary>
	SkinModelRender* GetSkinModelRender()
	{
		if (m_mapChipRender == nullptr) {
			//�}�b�v�`�b�v�����_�[�Ȃ����炢��Ȃ��B
			return nullptr;
		}
		//todo:�����_�[�擾�B
		return m_mapChipRender->GetModelRender();
	}
private:
	MapChipRender* m_mapChipRender = nullptr;		//�}�b�v�`�b�v�����_�[�B
};

