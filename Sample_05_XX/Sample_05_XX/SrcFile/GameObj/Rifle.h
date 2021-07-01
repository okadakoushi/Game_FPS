#pragma once

class GamePlayer;

class Rifle : public IGameObject
{
	//~Rifle();

public:
	/// <summary>
	/// �X�^�[�g�B
	/// </summary>
	/// <returns></returns>
	bool Start() override;
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update() override;
	/// <summary>
	/// �{�[�����W���Q�Ƃ��郌���_�[��ݒ�B
	/// </summary>
	void SetRefBoneRender(SkinModelRender* render)
	{
		m_refBoneRender = render;
	}
	/// <summary>
	/// �ʒu���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPos() const 
	{
		return m_pos;
	}
	/// <summary>
	/// ��]���擾�B
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation() const 
	{
		return m_rot;
	}
private:
	//���f����b�p�����[�^�[�B
	SkinModelRender* m_render = nullptr;
	Vector3 m_pos = g_vec3Zero;
	Quaternion m_rot = g_quatIdentity;
	Vector3 m_scale = g_vec3One;	
	Bone* m_rHandBone = nullptr;					//�E��{�[���B
	SkinModelRender* m_refBoneRender = nullptr;		//�{�[�����Q�Ƃ��郌���_�[�B
	GamePlayer* pl = nullptr;
};

