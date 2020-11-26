#pragma once

#include "Level.h"

/// <summary>
/// �}�b�v�`�b�v�����_�[�B
/// </summary>
class MapChipRender : public IGameObject
{
public:
	MapChipRender();
	~MapChipRender();
	/// <summary>
	/// �f�X�g���C�B
	/// </summary>
	void OnDestroy() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �`�悷��I�u�W�F�N�g�̐���ǉ��B
	/// </summary>
	/// <param name="objData">�I�u�W�F�N�g�f�[�^�[�B</param>
	void AddRenderObject(const LevelObjectData& objData)
	{
		m_renderObjDatas.push_back(objData);
	}
	/// <summary>
	/// �������B
	/// </summary>
	void InitRenderObject();
	/// <summary>
	/// ���f�������_�[�擾�B
	/// </summary>
	/// <returns></returns>
	SkinModelRender* GetModelRender()
	{
		return m_modelRender;
	}
	/// <summary>
	/// �`��I�u�W�F�N�g�ɃN�G�����s���B
	/// </summary>
	/// <param name="queryFunc">�֐��I�u�W�F�N�g�B</param>
	void QueryRenderObjDatas(std::function<void(const LevelObjectData& objData)> queryFunc)
	{
		for (const auto& renderObjData : m_renderObjDatas) {
			queryFunc(renderObjData);
		}
	}
private:
	int m_numRenderCount = 0;						//�`�悷��I�u�W�F�N�g�̐��B
	SkinModelRender* m_modelRender = nullptr;		//�`�悷��I�u�W�F�N�g�̃f�[�^�[�z��B
	std::vector<LevelObjectData> m_renderObjDatas;	//���f�������_�[�B
};

