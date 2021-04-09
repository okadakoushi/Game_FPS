#pragma once

/// <summary>
/// フィジックスワールド。
/// </summary>
class PhysicsWorld
{
	std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfig;
};

