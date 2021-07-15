#pragma once

class MouseInput : public Noncopyable
{
public:
	MouseInput() {};
	void Init();
	void Update();
	void End();
	/// <summary>
	/// XY���̍������o�́B
	/// </summary>
	/// <returns></returns>
	Vector2& GetVal_XY()
	{
		return m_xyVal;
	}

	void SetX_0()
	{
		m_xyVal.x = 0;
	}
	/// <summary>
	/// ���x��ݒ�B
	/// </summary>
	/// <param name="kando"></param>
	void SetKando(const float& kando)
	{
		m_Kando = kando;
	}
private:
	Vector2 m_xyVal = g_vec2Zero;		//
	Vector2 DEFAULT_POSITION = { 1280 * 0.5f, 720 * 0.5f };
	float	m_Kando = 0.08f;
};

