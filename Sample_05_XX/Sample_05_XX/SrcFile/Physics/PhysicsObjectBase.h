#pragma once
class PhysicsObjectBase
{
public:
	/// <summary>
	/// 解放。
	/// </summary>
	virtual void Release() = 0;
private:
	/// <summary>
	/// 静的物理オブジェクト作成処理の共通処理。
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	virtual void CreateCommon(Vector3 pos, Quaternion rot) = 0;
public:
	PhysicsObjectBase()
	{
	}
	virtual ~PhysicsObjectBase()
	{
	}
	/// <summary>
	/// ボックス形状のゴーストオブジェクトを作成。
	/// </summary>
	/// <param name="pos">座標。</param>
	/// <param name="rot">回転。</param>
	/// <param name="size">サイズ。</param>
	void CreateBox(Vector3 pos, Quaternion rot, Vector3 size);
	/// <summary>
	/// メッシュからゴーストオブジェクトを作成。
	/// </summary>
	/// <param name="pos">位置。</param>
	/// <param name="rot">回転。</param>
	/// <param name="tkmfile">各パラメーター。tkm/model/modelRender</param>
	void CreateMesh(Vector3 pos, Quaternion rot, const TkmFile& tkmfile);
	void CreateMesh(Vector3 pos, Quaternion rot, Vector3 scale, const Model& model);
	void CreateMesh(Vector3 pos, Quaternion rot, Vector3 scale, SkinModelRender* modelRender);
protected:
	std::unique_ptr<ICollider> m_collider;		//コライダー。
};

