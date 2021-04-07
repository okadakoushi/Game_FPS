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
private:
	//���f����b�p�����[�^�[�B
	SkinModelRender* m_render = nullptr;
	Vector3 m_pos = g_vec3Zero;
	Quaternion m_rot = g_quatIdentity;
	Vector3 m_scale = g_vec3One;	
	//�n���h�̃p�����[�^�[�B
	Vector3 m_Rpos = g_vec3Zero;
	Quaternion m_Rrot = g_quatIdentity;
	Vector3 m_Lpos = g_vec3Zero;
	Quaternion m_Lrot = g_quatIdentity;
	
	GamePlayer* m_player = nullptr;	//�v���C���[�B
	Bone* m_rHandBone = nullptr;	//�E��{�[���B
	Bone* m_lHandBone = nullptr;	//�E��{�[���B
};

