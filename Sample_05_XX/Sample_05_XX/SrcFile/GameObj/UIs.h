#pragma once

/// <summary>
/// UI�n�Ǘ��I�u�W�F�N�g�B
/// </summary>
class UIs : public IGameObject
{
public:
	void OnDestroy() override;
	/// <summary>
	/// �������B
	/// </summary>
	bool Start() override;
private:
	SpriteRender* m_testSprite = nullptr;	//�e�X�g�����_�[�B
};

