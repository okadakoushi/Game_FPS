#pragma once

/// <summary>
/// 兵士用コリジョンクラス。
/// ミクサモで出力してきたアニメーションで人型ならある程度はカバーできるかも。
/// todo:ミクサモで出力してきたアニメーションで人形なら全部対応できるようにする。
/// </summary>
class SoldierCollision
{
public:
	~SoldierCollision();
	/// <summary>
	/// 明示的解放。
	/// </summary>
	void Release();
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="render"></param>
	void Init(SkinModelRender* render);
	/// <summary>
	/// 更新。
	/// </summary>
	void Update();
	/// <summary>
	/// 本体ポインタを設定。
	/// <para>Initの前に呼び出すこと。</para>
	/// </summary>
	/// <param name="p"></param>
	void SetPointer(void* p)
	{
		m_pointer = p;
	}
	/// <summary>
	/// エネミーかどうかのフラグ。
	/// </summary>
	/// <param name="flag"></param>
	void SetEnemyFlag(const bool& flag)
	{
		m_isEnemy = flag;
	}
private:
	bool m_isInited = false;
	//当たり判定Param。
	static const unsigned int COLLISION_COUNT = 4;
	PhysicsStaticObject m_collision[COLLISION_COUNT];	//当たり判定。
	std::array<const wchar_t*, COLLISION_COUNT> m_bonePaths{	//各ボーンのファイルパス。
		L"mixamorig:Head",		//頭。
		L"mixamorig:Spine",		//腰。
		L"mixamorig:RightLeg",	//右足。
		L"mixamorig:LeftLeg"	//左足。
	};
	Vector3 m_collisionScales[COLLISION_COUNT]{		//コリジョンサイズ。
		{20.0f, 20.0f, 20.0f},	//頭。
		{40.0f, 45.0f, 25.0f},	//腰。
		{15.0f, 30.0f, 15.0f},	//右足.
		{15.0f, 38.0f, 15.0f}	//左足。
	};
	Bone* m_boneList[COLLISION_COUNT]; 					//ボーン。
	Vector3 m_bonePosition = g_vec3Zero;				//ボーンの位置。
	Quaternion m_boneRotation = g_quatIdentity;			//ボーンの回転。
	void* m_pointer = nullptr;
	bool m_isEnemy = true;
};

