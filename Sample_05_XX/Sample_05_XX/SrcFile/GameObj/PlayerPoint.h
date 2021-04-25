#pragma once
class PlayerPoint
{
public:
	PlayerPoint();
	~PlayerPoint();
	//void Update() override;
	/// <summary>
	/// モデルの位置を設定。
	/// <para>スプライトの位置ではないため注意。</para>
	/// </summary>
	/// <param name="pos">位置。</param>
	void Set3DModelPos(const Vector3 pos) {
		m_modelPos = pos;
	}
	/// <summary>
	/// モデルの位置を取得。
	/// <para>スプライトの位置ではないため注意。</para>
	/// </summary>
	/// <returns></returns>
	const Vector3& Get3DModelPos() const
	{
		return m_modelPos;
	}
	/// <summary>
	/// 位置を設定。
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

