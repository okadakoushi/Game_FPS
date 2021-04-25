#pragma once

#include "SrcFile/Shape/Box.h"

/// <summary>
/// カリング処理のインターフェースクラス。
/// </summary>
class IObjectCulling
{
public:
	IObjectCulling() {};
	virtual ~IObjectCulling() {};
	/// <summary>
	/// 更新。
	/// </summary>
	/// <param name="aabb"></param>
	virtual void Excute(const Box& aabb, const Matrix& viewProj) = 0;
	/// <summary>
	/// カリングされているかの判定。
	/// </summary>
	/// <returns>カリングフラグ。</returns>
	bool IsCulling() const
	{
		return m_isCulling;
	}

protected:
	/// <summary>
	/// カリングフラグを設定。
	/// </summary>
	/// <param name="flag">フラグ。</param>
	void SetCullingFlag(bool flag)
	{
		m_isCulling = flag;
	}
private:
	bool m_isCulling = false;	//カリングフラグ。
};

