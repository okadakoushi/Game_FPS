#include "stdafx.h"
#include "DirectionalShadowMap.h"

void DirectionalShadowMap::Init(int w, int h, float lightHeight)
{
	//�V���h�E�}�b�v�̉𑜓x�B
	int wh[NUM_SHADOW_MAP][2] = {
		{w,h},				//�ߋ����B
		{w >> 2, h >> 2},	//�������B
		{w >> 2, h >> 2}	//�������B
	};
	//�V���h�E�ԍ��B
	int shadowMapNo = 0;
	//�N���A�J���[�B
	float clearColor[] = {
		1.0f,1.0f,1.0f,1.0f
	};
	//�V���h�E�}�b�v���쐬�B
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
		//���̃V���h�E�}�b�v�ցB
		shadowMapNo++;
	}
	//�萔�o�b�t�@�������B
	m_shadowCB.Init(sizeof(m_shadowCBEntity), nullptr);
}

void DirectionalShadowMap::RenderToShadowMap(RenderContext& rc)
{
	if (m_isEnable == true) {
		//�V���h�E�}�b�v�L���������B
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			//�����_�[�^�[�Q�b�g�̃Z�b�g�B
			rc.SetRenderTarget(m_shadowMaps[i].GetRTVCpuDescriptorHandle(), m_shadowMaps[i].GetDSVCpuDescriptorHandle());
			for (auto& caster : m_shadowCasters) {
				caster->Draw(rc, m_LVPMatrix, g_matIdentity);
			}
		}
	}

}