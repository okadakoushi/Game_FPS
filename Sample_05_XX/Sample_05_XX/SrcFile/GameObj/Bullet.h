#pragma once

class GamePlayer;

/// <summary>
/// �e�ۃN���X�B
/// �F�X�e��肾����C���K�v�B
/// </summary>
class Bullet : public IGameObject
{
public:
	/// <summary>
	/// �f�X�g���C�B
	/// </summary>
	void OnDestroy() override;
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
	/// �ʒu��ݒ�B
	/// <para>NewGO����ꍇ�A��ΌĂяo�����ƁB</para>
	/// </summary>
	/// <param name="v">�ʒu�B</param>
	void SetPos(const Vector3& v)
	{
		m_pos = v;
	}
private:
	GamePlayer* m_player = nullptr;
	Vector3 m_pos = g_vec3Zero;		//�ʒu�B
	Vector3 m_toTarget = g_vec3Zero;//�����_�Ɍ������x�N�g���B
	float m_LengthOnNew = 0.0f;		//NewGO���̒����B
	const float m_speed = 5.0f;		//�����B
	SkinModelRender* m_render = nullptr;
};

