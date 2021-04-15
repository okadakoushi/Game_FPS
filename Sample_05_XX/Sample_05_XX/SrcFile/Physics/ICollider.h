#pragma once

class btCollisionShape;

class ICollider {
public:
	/// <summary>
	/// ボディ取得。
	/// </summary>
	/// <returns></returns>
	virtual btCollisionShape* GetBody() const = 0;
};