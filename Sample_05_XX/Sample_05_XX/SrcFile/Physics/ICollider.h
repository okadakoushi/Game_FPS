#pragma once

class btCollisionShape;

class ICollider {
public:
	/// <summary>
	/// �{�f�B�擾�B
	/// </summary>
	/// <returns></returns>
	virtual btCollisionShape* GetBody() const = 0;
};