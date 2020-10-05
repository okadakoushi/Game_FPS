#pragma once
class Game : public IGameObject
{
public:
	Game();
	~Game();
	
	bool Start() override;
	void Update() override;
private:
	Model m_animSampleModel;					//アニメーションさせるサンプルモデル。
	Vector3 m_pos = Vector3::Zero;				//座標。
	Vector3 m_scale = Vector3::One;				//拡大率
	Quaternion m_rot = Quaternion::Identity;	//回転。
	Animation m_animation;						//アニメーション。
	int initState = 0;							//初期化ステップ。
	bool m_inited = false;						//初期化終わった？。
};

