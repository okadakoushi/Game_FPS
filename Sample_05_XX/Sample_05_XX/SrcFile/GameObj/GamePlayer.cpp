#include "stdafx.h"
#include "GamePlayer.h"
#include "GameCamera.h"
#include "Bullet.h"
#include "Rifle.h"
#include "Enemy/RifleEnemy.h"

void GamePlayer::OnDestroy()
{
	DeleteGO(m_reticle);
	DeleteGO(m_unityChan);
}

bool GamePlayer::Start()
{	
	//unitychan初期化。
	m_unityChan = NewGO<SkinModelRender>(EnPriority_3DModel, "Unity");
	const char* tkaFilePaths[] = {
		"Assets/animData/Enemy/Rifle/Idle.tka",
		"Assets/animData/Enemy/Rifle/Walk.tka",
		"Assets/animData/Enemy/Rifle/WalkShoot.tka"
	};
	m_unityChan->Init("Assets/modelData/Chara/Enemy.tkm", tkaFilePaths);
	//m_unityChan->Init("Assets/modeldata/unityChan.tkm", "Assets/animData/unityChan/test.tka");
	//シャドウキャスター。
	m_unityChan->SetShadwoCaster(true);
	m_unityChan->SetShadowReciever(true);
	m_unityChan->SetScale({ 1.5f, 1.5f, 1.5f });

	//SkinModelRenderのアニメーション再生が終わったタイミングで呼ばれる処理を設定。
	m_unityChan->SetPostAnimationProgressFunc([&] {	OnPostAnimationProgress(); });
	//位置初期化。
	m_pos = m_unityChan->GetPosition();
	//キャラコン初期化。
	m_cCon.Init(25.0f, 60.0f, m_pos);
	//腰のボーン取得。
	int spineBoneNo = m_unityChan->GetSkelton().FindBoneID(L"mixamorig:Spine");
	m_spineBone = m_unityChan->GetSkelton().GetBone(spineBoneNo);
	//頭のボーン取得。
	int headNo = m_unityChan->GetSkelton().FindBoneID(L"mixamorig:Head");;
	m_headBone = m_unityChan->GetSkelton().GetBone(headNo);
	//武器。
	m_wepon = NewGO<Rifle>(EnPriority_3DModel, "Wepon");
	m_wepon->SetRefBoneRender(m_unityChan);	

	//カメラFind。
	m_camera = FindGO<GameCamera>("GameCamera");

	//レティクル初期化。
	m_reticle = NewGO<SpriteRender>(EnPriority_UI);
	SpriteInitData testInitData;
	//ddsファイル初期化。
	testInitData.m_ddsFilePath[0] = "Assets/sprite/reticle.dds";
	testInitData.m_width = 50.0f;
	testInitData.m_height = 50.0f;
	testInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_reticle->Init(testInitData);
	m_reticle->SetPos({ 0.0f, m_pos.y, 0.0f });

	//エフェクト生成。
	m_effect = NewGO<myEngine::Effect>(EnPriority_3DModel);
	m_effect->SetScale({ 3.0f, 3.0f, 3.0f });

	m_pos = { 0.0f, 0.0f, 0.0f };

	return true;
}

void GamePlayer::PostUpdate()
{

	if (m_playerState != EnPlayerState_Deth) {
		//死んでない。
		//回転。
		Rotation();
		//移動。
		Move();
		//射撃。
		Shot();
	}

	switch (m_playerState)
	{
	case EnPlayerState_Idle:
		m_unityChan->PlayAnimation(EnPlayerState_Idle, 0.5f);
		break;
	case EnPlayerState_Walk:
		m_unityChan->PlayAnimation(EnPlayerState_Walk, 0.5f);
		break;
	case EnPlayerState_Run:
		m_unityChan->PlayAnimation(EnPlayerState_Walk, 0.5f);
		break;
	case EnPlayerState_Buck:
		m_unityChan->PlayAnimation(EnPlayerState_Walk, 0.5f);
		break;
	case EnPlayerState_Shot:
		m_unityChan->PlayAnimation(EnPlayerState_Shot, 0.2f);
		break;
	case EnPlayerState_Reload:
		break;
	case EnPlayerState_Deth:
		break;
	case EnPlayerState_Num:
		break;
	}
	m_unityChan->SetPosition(m_pos);
}


void GamePlayer::OnPostAnimationProgress()
{
	Quaternion qRot;
	//X軸周りの回転を計算。
	Vector3 to = GraphicsEngineObj()->GetCamera3D().GetForward();
	to.Normalize();
	//腰への回転は少し強めに取る。
	qRot.SetRotation(g_vec3AxisX, atan2f(to.y * -1.2f, g_vec3Front.z));
	Matrix local = m_spineBone->GetLocalMatrix();

	Matrix trans;
	trans.MakeRotationFromQuaternion(qRot);
	local *= trans;
	//腰のボーンに回転を適用させる。
	m_spineBone->SetLocalMatrixFromUser(local);
}
void GamePlayer::Rotation()
{	
	Vector3 toPos = GraphicsEngineObj()->GetCamera3D().GetTarget() - GraphicsEngineObj()->GetCamera3D().GetPosition();
	toPos.Normalize();
	float angle = atan2(toPos.x, toPos.z);
	m_rot.SetRotation(g_vec3AxisY, angle);
	m_unityChan->SetRotation(m_rot);
}

void GamePlayer::Shot()
{
	//ポストにしないとアニメーションの更新終わってないのでガタブルする。
	Quaternion a;
	m_headBone->CalcWorldTRS(m_headPos, a);
	m_headPos.y += 4.0f;
	m_camera->SetEyePos(m_headPos);

	//レイのコールバック。
	RayTestCallBack::PlayerRayTestResult rayCallBack;
	//レイテストを行う。
	Vector3 toDir = GraphicsEngineObj()->GetCamera3D().GetTarget() - m_headPos;
	toDir.Normalize();
	toDir *= RAY_RANGE;
	PhysicObj().RayTest(m_headPos, toDir + m_headPos, rayCallBack);

	m_flame++;
	if (GetAsyncKeyState(MK_LBUTTON)) {
		if (m_flame >= 20) {
			Bullet* bullet = NewGO<Bullet>(EnPriority_3DModel);
			bullet->SetPos(m_wepon->GetPos());
			bullet->SetRot(m_rot);
			bullet->SetToTarget(toDir);
			m_flame = 0;
			//printf("StaticObjectDist = %f\n", rayCallBack.StaticObjectDist);
			//printf("CharacterObjectDist = %f\n", rayCallBack.CharacterObjectDist);
			if (rayCallBack.hasHit() && rayCallBack.StaticObjectDist > rayCallBack.CharacterObjectDist) {
				//敵にレイが命中。
				//生ポインタから敵に強制キャスト。
				RifleEnemy* enemy = reinterpret_cast<RifleEnemy*>(rayCallBack.m_collisionObject->getUserPointer());
				if (enemy != nullptr) {
					enemy->SetState(EnEnemyState_Damage);
				}
			}
			else if(rayCallBack.hasHit() && rayCallBack.StaticObjectDist < rayCallBack.CharacterObjectDist){
				//なんかのメッシュに命中。
				//命中した地点からエフェクトを再生。
				Vector3 effectPos = m_headPos + toDir * rayCallBack.StaticObjectDist;
				Quaternion effectRot;
				effectRot.SetRotation(g_vec3AxisY ,atan2f(g_vec3Front.z, toDir.x));
				m_effect->SetPosition(effectPos);
				m_effect->SetRotation(effectRot);
				//Y成分はいらない。
				printf("%f\n", rayCallBack.StaticObjectDist);
				printf("%f, %f, %f\n", toDir.x /* rayCallBack.StaticObjectDist*/, toDir.y /* rayCallBack.StaticObjectDist*/, toDir.z /* rayCallBack.StaticObjectDist*/);
				m_effect->Play(L"Assets/effect/aaaa.efk");
			}
			else if (!rayCallBack.hasHit()) {
				printf("%f\n", rayCallBack.StaticObjectDist);
			}
		}
		m_playerState = EnPlayerState_Shot;
	}
}

void GamePlayer::Move()
{
	float deltaTime = GameTime().GetFrameDeltaTime();
	//カメラの向き情報。
	Vector3 camForward = GraphicsEngineObj()->GetCamera3D().GetForward();
	Vector3 camRight = GraphicsEngineObj()->GetCamera3D().GetRight();
	camForward.y = 0.0f;
	camForward.Normalize();
	camRight.y = 0.0f;
	camRight.Normalize();
	
	//加速度。
	Vector3 acc;

	//移動処理。
	//todo:Pad対応？キャラコン対応？慣性？
	if (GetAsyncKeyState('W')) {
		m_playerState = EnPlayerState_Walk;
		acc += camForward * m_speed;
	}
	if (GetAsyncKeyState('S')) {
		m_playerState = EnPlayerState_Buck;
		acc -= camForward * m_speed;
	}
	if (GetAsyncKeyState('D')) {
		m_playerState = EnPlayerState_Walk;
		acc += camRight * m_speed;
	}
	if (GetAsyncKeyState('A')) {
		m_playerState = EnPlayerState_Walk;
		acc -= camRight * m_speed;
	}
	if (GetAsyncKeyState(VK_SHIFT) && m_playerState != EnPlayerState_Shot && m_playerState != EnPlayerState_Buck) {
		//ダッシュ。
		acc *= 3.0f;
	}
	if (m_cCon.IsOnGround()) {
		if (GetAsyncKeyState(VK_SPACE)) {
			//ジャンプ。
			acc += {0, m_JUMPFORSE, 0};
		}
	}
	else {
		//重力。
#ifdef MASTER
		acc += {0, -m_GRAVITY, 0};
#endif
	}



	m_move += acc * 2.0;

	if (m_move.Length() >= 300.0f && m_playerState != EnPlayerState_Buck) {
		m_playerState = EnPlayerState_Walk;
	}
	if (m_move.Length() >= 700.0f) {
		m_playerState = EnPlayerState_Run;
	}
	if(m_move.Length() == 0.0f){
		m_playerState = EnPlayerState_Idle;
	}

	if (m_move.Length() != 0) {
		//printf("%f, %f, %f \n", m_pos.x,m_pos.y, m_pos.z);
	}


	//摩擦。
	m_move.x += m_move.x * -0.5f;
	m_move.z += m_move.z * -0.5f;
	m_move.y += m_move.y * -0.025f;
	m_pos = m_cCon.Execute(m_move);
}

