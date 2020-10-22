#include "stdafx.h"
#include "DirectionalShadowMap.h"

void DirectionalShadowMap::Init(int w, int h, float lightHeight)
{
	//シャドウマップの解像度。
	int wh[NUM_SHADOW_MAP][2] = {
		{w,h},				//近距離。
		{w >> 1, h >> 1},	//中距離。
		{w >> 1, h >> 1}	//遠距離。
	};
	//シャドウ番号。
	int shadowMapNo = 0;
	//クリアカラー。
	float clearColor[] = {
		1.0f, 1.0f, 1.0f, 1.0f
	};
	//シャドウマップを作成。
	for (auto& sm : m_shadowMaps) {
		sm.Create(
			wh[shadowMapNo][0],
			wh[shadowMapNo][1],
			0,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
		m_shadowMaps[shadowMapNo].SetName(L"SHADOW MAP");
		//次のシャドウマップへ。
		shadowMapNo++;
	}
	
	//ライトの高さ。
	m_lightHeight = lightHeight;
	//CB作成
	m_shadowConstantBuffer.Init(sizeof(SShadowCb), nullptr);
}

void DirectionalShadowMap::Update()
{
	//シーンをレンダリングしているカメラを使って、ライトカメラの回転を計算
	//シーンカメラの前方向取得
	auto cameraDir = g_camera3D->GetForward();
	//fabs = 絶対値  FLT_EPSLIONでfloat特有の誤差を考慮しない。
	//x,z軸が0に近いなら
	if (fabs(cameraDir.x) < FLT_EPSILON && fabsf(cameraDir.z) < FLT_EPSILON) {
		//ほぼ真上を向いてる
		return;
	}

	//ライトビュー行列の回転成分を計算

	//ライトビューの前方向 下向き
	Vector3 lightViewForward = m_lightDirection;
	//ライトビューの上方向
	Vector3 lightViewUp;
	if (fabsf(lightViewForward.y) > 0.999f) {
		//ほぼ真上
		lightViewUp.Cross(lightViewForward, Vector3::Right);
	}
	else {
		lightViewUp.Cross(lightViewForward, Vector3::Up);
	}
	//正規化
	lightViewUp.Normalize();

	//ライトビューの横方向
	Vector3 lightViewRight;
	lightViewRight.Cross(lightViewUp, lightViewForward);
	lightViewRight.Normalize();

	//ライトビューに回転を設定していく
	Matrix lightViewRot;
	//ライトビューの横を設定
	lightViewRot.m[0][0] = lightViewRight.x;
	lightViewRot.m[0][1] = lightViewRight.y;
	lightViewRot.m[0][2] = lightViewRight.z;
	lightViewRot.m[0][3] = 0.0f;
	//ライトビューの上を設定
	lightViewRot.m[1][0] = lightViewUp.x;
	lightViewRot.m[1][1] = lightViewUp.y;
	lightViewRot.m[1][2] = lightViewUp.z;
	lightViewRot.m[1][3] = 0.0f;
	//ライトビューの前を設定する。
	lightViewRot.m[2][0] = lightViewForward.x;
	lightViewRot.m[2][1] = lightViewForward.y;
	lightViewRot.m[2][2] = lightViewForward.z;
	lightViewRot.m[2][3] = 0.0f;

	//視錐台を分割する比率
	float shadowAreaTbl[] = {
		m_lightHeight * 0.4f,
		m_lightHeight * 0.8f,
		m_lightHeight * 1.2f
	};

	//ライトビューの高さを計算
	float lightHeight = g_camera3D->GetTarget().y + m_lightHeight;

	//シャドウの構造体
	SShadowCb shadowCB;
	//近平面
	float nearPlaneZ = 0.0f;
	//遠平面
	float farPlaneZ;
	//メインカメラの上方向
	Vector3 cameraUp;
	//外積で上方向を計算
	cameraUp.Cross(g_camera3D->GetRight(), g_camera3D->GetForward());
	//cameraUp = g_camera3D.GetUp();

	//視錐台を分割するようにライトビュープロジェクション行列を計算
	for (int i = 0; i < NUM_SHADOW_MAP; i++) {
		//遠平面を計算
		farPlaneZ = nearPlaneZ + shadowAreaTbl[i];
		//ライトビュー
		Matrix mLightView = Matrix::Identity;
		//視錐台の片方ずつ(上、下)計算していくのでアングルは半分
		float halfViewAngle = g_camera3D->GetViewAngle() * 0.5f;
		//視錐台の8頂点をライト空間に変換してAABB(視錐台の中にある直方体)を求めて、正射影の幅と高さを求める。
		float w, h;
		float far_z = -1.0f;
		//8頂点
		Vector3 v[8];
		{
			//隣返が対返に対して何倍か
			float t = tan(halfViewAngle);
			//近平面と遠平面の上ベクトル
			Vector3 toUpperNear, toUpperFar;
			//対辺を求める。カスケードシャドウのtoUpper.png参照。
			//nearPlaneZが隣返
			toUpperNear = cameraUp * t * nearPlaneZ;
			toUpperFar = cameraUp * t * farPlaneZ;
			//アスペクト比を乗算
			//いままではまっすぐ前の視錐台の線のみ計算だったのが
			//ここからは縦横の情報がはいるため　詳細はaspect.pngを参照。
			t *= g_camera3D->GetAspect();
			//近平面の中央座標を計算
			//資料8vertex.pngを参照。
			auto nearPlaneCenterPos = g_camera3D->GetPosition() + cameraDir * nearPlaneZ;

			//v[0] = 視錐台の近平面で右上にある四角形の頂点。
			v[0] = nearPlaneCenterPos + g_camera3D->GetRight() * t * nearPlaneZ + toUpperNear;
			//v[1] = 視錐台の近平面で右下にある四角形の頂点
			v[1] = v[0] - toUpperNear * 2.0f;

			//v[2] = 視錐台の近平面で左上にある四角形の頂点
			v[2] = nearPlaneCenterPos + g_camera3D->GetRight() * -t * nearPlaneZ + toUpperNear;
			//v[3] = 視錐台の近平面で左下にある四角形の頂点
			v[3] = v[2] - toUpperNear * 2.0f;

			//遠平面の中央座標を計算。
			auto farPlaneCenterPos = g_camera3D->GetPosition() + cameraDir * farPlaneZ;

			//v[4] = 視錐台の遠平面で右上にある四角形の頂点
			v[4] = farPlaneCenterPos + g_camera3D->GetRight() * t * farPlaneZ + toUpperFar;
			//v[5] = 視錐台の遠平面で右下にある四角形の頂点
			v[5] = v[4] - toUpperFar * 2.0f;

			//v[6] = 視錐台の遠平面で左上にある四角形の頂点
			v[6] = farPlaneCenterPos + g_camera3D->GetRight() * -t * farPlaneZ + toUpperFar;
			//v[7] = 視錐台の遠平面で左下にある四角形の頂点
			v[7] = v[6] - toUpperFar * 2.0f;

			//ライト行列を作成
			//視錐台の真ん中を計算。
			auto viewFrustumCenterPosition = (nearPlaneCenterPos + farPlaneCenterPos) * 0.5f;
			//ライトの位置を計算。
			auto lightPos = CalcLightPosition(lightHeight, viewFrustumCenterPosition);

			//ライトの回転
			mLightView = lightViewRot;

			//ライトの位置,回転
			mLightView.m[3][0] = lightPos.x;
			mLightView.m[3][1] = lightPos.y;
			mLightView.m[3][2] = lightPos.z;
			mLightView.m[3][3] = 1.0f;

			//ライトビュー(ライト座標軸）完成
			//逆行列うんぬんかぬん忘れている場合はGameProgに解説あり。
			mLightView.Inverse(mLightView);

			//視錐台を構成する８頂点の計算ができたので、ライト空間に変換して、AABBを求める。
			//最大頂点座標
			Vector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
			//最小頂点座標
			Vector3 vMin = { FLT_MAX,FLT_MAX ,FLT_MAX };

			//8頂点をライトの座標軸に変換する。
			for (auto& vInLight : v) {
				mLightView.Mul(vInLight);
				vMax.Max(vInLight);
				vMin.Min(vInLight);
			}
			//ビューの幅と高さを求める。
			w = vMax.x - vMin.x;
			h = vMax.y - vMin.y;
			far_z = vMax.z;
		}
		//プロジェクション行列
		Matrix proj;
		//作成
		proj.MakeOrthoProjectionMatrix(
			w,
			h,
			far_z / 100.0f,
			far_z 
		);
		m_lightViewMatrix[i] = mLightView;
		m_lightProjMatirx[i] = proj;
		//ライト座標軸に変換
		m_LVPMatrix[i] = mLightView * proj;
		//エンティティに積む
		m_shadowCBEntity.mLVP[i] = m_LVPMatrix[i];
		//どこまで影を落とすか(きわきわだと変な境界が発生するので少し狭める。)
		m_shadowCBEntity.shadowAreaDepthInViewSpace[i] = farPlaneZ * 0.8f;
		//次の近平面は今の遠平面。
		nearPlaneZ = farPlaneZ;
		//VRAMにコピー。
		m_shadowConstantBuffer.CopyToVRAM(m_shadowCBEntity);
	}
}

Vector3 DirectionalShadowMap::CalcLightPosition(float lightHeight, Vector3 viewFrustomCenterPosition) const 
{
	/*
	ライトの高さ = 視錐台の中心座標.y + ライトの方向 * a
	一次方程式なので a = に直して
	a = (ライトの高さ - 視点の中心座標.y) / ライトの方向.y
	となる。
	*/
	auto alpha = (lightHeight - viewFrustomCenterPosition.y) / m_lightDirection.y;
	/*
	aを使ってライト座標を求める。
	ライトの座標 = 視錐台の中心座標 + ライトの方向 * a
	*/
	auto lightPos = viewFrustomCenterPosition + m_lightDirection * alpha;
	//ライトの位置
	return lightPos;
}

void DirectionalShadowMap::RenderToShadowMap()
{
	//レンコン。
	auto& rc = g_graphicsEngine->GetRenderContext();
	if (m_isEnable == true) {
		//シャドウマップ有効だった。
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			if (!m_Inited[i]) {
				//レンダリングターゲットが設定できるようになるまで待機。
				rc.WaitUntilToPossibleSetRenderTarget(m_shadowMaps[i]);
				m_Inited[i] = true;
			}
			//レンダーターゲットのセット。
			rc.SetRenderTarget(m_shadowMaps[i], m_shadowMaps[i].GetDSVCpuDescriptorHandle());
			//ケシケシ
			rc.ClearRenderTargetView(m_shadowMaps[i].GetRTVCpuDescriptorHandle(), m_shadowMaps[i].GetRTVClearColor());
			rc.ClearDepthStencilView(m_shadowMaps[i].GetDSVCpuDescriptorHandle(), 1.0f);
			for (auto& caster : m_shadowCasters) {
				//カキカキ
				caster->Draw(rc, m_lightViewMatrix[i], m_lightProjMatirx[i], enRenderMode_DrawShadow);
				//DX12の仕様上、定数バッファの値を変更する場合はコマンドリストにのっかっている描画処理を
				//一旦描画して、コマンドリストを初期化。
				g_graphicsEngine->EndRender(false);
				//RTVはここでmainRenderTargetに戻る。
				g_graphicsEngine->BeginRender();
			}
		}

		//カキカキ終わったらレンダーターゲット戻す。
		//rc.SetRenderTarget(g_graphicsEngine->GetCurrentFrameBuffuerRTV(), g_graphicsEngine->GetCurrentFrameBufferDSV());
	}

}