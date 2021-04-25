#pragma once
class PlayerPoint
{
public:
	PlayerPoint();
	~PlayerPoint();
	//void Update() override;
	/// <summary>
	/// ���f���̈ʒu��ݒ�B
	/// <para>�X�v���C�g�̈ʒu�ł͂Ȃ����ߒ��ӁB</para>
	/// </summary>
	/// <param name="pos">�ʒu�B</param>
	void Set3DModelPos(const Vector3 pos) {
		m_modelPos = pos;
	}
	/// <summary>
	/// ���f���̈ʒu���擾�B
	/// <para>�X�v���C�g�̈ʒu�ł͂Ȃ����ߒ��ӁB</para>
	/// </summary>
	/// <returns></returns>
	const Vector3& Get3DModelPos() const
	{
		return m_modelPos;
	}
	/// <summary>
	/// �ʒu��ݒ�B
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(const Vector3& pos)
	{
		m_spriteRender->SetPos(pos);
	}

private:
	SpriteRender* m_spriteRender = nullptr;
	Vector3 m_modelPos = g_vec3Zero;
};

