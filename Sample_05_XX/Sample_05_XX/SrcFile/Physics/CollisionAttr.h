#pragma once

/// <summary>
/// コリジョン属性。
/// </summary>
enum EnCollisionAttr {
	enCollisionAttr_Ground,
	enCollisionAttr_Character,
	enCollisionAttr_StaticObject,
	enCollisionAttr_Enemy,
	enCollisionAttr_User,		//ユーザー定義コリジョンを追加。
};