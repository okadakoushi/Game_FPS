#pragma once

/// <summary>
/// �X�e�[�W�N���X�B
/// </summary>
class Stage : public IGameObject
{
public:
	~Stage();
	/// <summary>
	/// �X�^�[�g�B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
private:
	SkinModelRender* m_bg = nullptr;			//�X�e�[�W�B
	Level m_level;								//���x���B
};

