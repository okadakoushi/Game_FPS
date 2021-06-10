#pragma once

/// <summary>
/// 箱。AABB。
/// </summary>
class Box
{
public:
	Box();
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="halfSize"></param>
	void Init(const Vector3& halfSize);
	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="worldMat"></param>
	void Update(const Matrix& worldMat);
	/// <summary>
	/// バウンティングボックスを描画。
	/// </summary>
	void DrawBox();
	/// <summary>
	/// 指定されたボックスの頂点座標を取得。
	/// </summary>
	/// <param name="vertNo">頂点番号。</param>
	/// <returns>ボックスの頂点座標。</returns>
	const Vector3& GetVertexPosition(unsigned int vertNo)const
	{
		if (vertNo > 8) {
			MessageBoxA(nullptr, "その頂点は存在しません。頂点番号を確認してください。", "Box::GetVertexPosition", MB_OKCANCEL);
		}
		return m_vertexPosition[vertNo];
	}

private:
	Vector3 m_halfSize = g_vec3Zero;	//ハーフサイズ。
	Vector3 m_vertexPosition[8];		//ボックスを構成する8頂点。
};

