#pragma once

#include "IObjectCulling.h"

/// <summary>
/// 視錐台カリング。
/// </summary>
class ObjectFrustumCulling : public IObjectCulling
{
public:
	ObjectFrustumCulling();
	~ObjectFrustumCulling();
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="ViewProj">ビュープロジェ。</param>
	void Init()
	{
		m_isAvtive = true;
	}
	/// <summary>
	/// カリング実行。
	/// </summary>
	/// <param name="box">AABB。</param>
	/// <param name="viewProj">viewProj。</param>
	void Excute(const Box& box, const Matrix& viewProj) override;
private:
	bool m_isAvtive = false;		//有効かどうか。
};

