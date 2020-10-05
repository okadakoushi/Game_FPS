#include "stdafx.h"
#include "DirectionalShadowMap.h"

void DirectionalShadowMap::Init(int w, int h, float lightHeight)
{
	//シャドウマップの解像度。
	int wh[NUM_SHADOW_MAP][2] = {
		{w,h},				//近距離。
		{w >> 2, h >> 2},	//中距離。
		{w >> 2, h >> 2}	//遠距離。
	};
	//シャドウ番号。
	int shadowMapNo = 0;
	//クリアカラー。
	float clearColor[] = {
		1.0f,1.0f,1.0f,1.0f
	};
	//シャドウマップを作成。
	for (auto& sm : m_shadowMaps) {
		sm.Create(
			wh[shadowMapNo][0],
			wh[shadowMapNo][1],
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32_FLOAT,
			clearColor
		);
		//次のシャドウマップへ。
		shadowMapNo++;
	}
	//定数バッファ初期化。
	m_shadowCB.Init(sizeof(m_shadowCBEntity), nullptr);
}

void DirectionalShadowMap::RenderToShadowMap(RenderContext& rc)
{
	if (m_isEnable == true) {
		//シャドウマップ有効だった。
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			//レンダーターゲットのセット。
			rc.SetRenderTarget(m_shadowMaps[i].GetRTVCpuDescriptorHandle(), m_shadowMaps[i].GetDSVCpuDescriptorHandle());
			for (auto& caster : m_shadowCasters) {
				caster->Draw(rc, m_LVPMatrix, g_matIdentity);
			}
		}
	}

}