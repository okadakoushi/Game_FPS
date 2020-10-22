#include "stdafx.h"
#include "DirectionalShadowMap.h"

void DirectionalShadowMap::Init(int w, int h, float lightHeight)
{
	//�V���h�E�}�b�v�̉𑜓x�B
	int wh[NUM_SHADOW_MAP][2] = {
		{w,h},				//�ߋ����B
		{w >> 1, h >> 1},	//�������B
		{w >> 1, h >> 1}	//�������B
	};
	//�V���h�E�ԍ��B
	int shadowMapNo = 0;
	//�N���A�J���[�B
	float clearColor[] = {
		1.0f, 1.0f, 1.0f, 1.0f
	};
	//�V���h�E�}�b�v���쐬�B
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
		//���̃V���h�E�}�b�v�ցB
		shadowMapNo++;
	}
	
	//���C�g�̍����B
	m_lightHeight = lightHeight;
	//CB�쐬
	m_shadowConstantBuffer.Init(sizeof(SShadowCb), nullptr);
}

void DirectionalShadowMap::Update()
{
	//�V�[���������_�����O���Ă���J�������g���āA���C�g�J�����̉�]���v�Z
	//�V�[���J�����̑O�����擾
	auto cameraDir = g_camera3D->GetForward();
	//fabs = ��Βl  FLT_EPSLION��float���L�̌덷���l�����Ȃ��B
	//x,z����0�ɋ߂��Ȃ�
	if (fabs(cameraDir.x) < FLT_EPSILON && fabsf(cameraDir.z) < FLT_EPSILON) {
		//�قڐ^��������Ă�
		return;
	}

	//���C�g�r���[�s��̉�]�������v�Z

	//���C�g�r���[�̑O���� ������
	Vector3 lightViewForward = m_lightDirection;
	//���C�g�r���[�̏����
	Vector3 lightViewUp;
	if (fabsf(lightViewForward.y) > 0.999f) {
		//�قڐ^��
		lightViewUp.Cross(lightViewForward, Vector3::Right);
	}
	else {
		lightViewUp.Cross(lightViewForward, Vector3::Up);
	}
	//���K��
	lightViewUp.Normalize();

	//���C�g�r���[�̉�����
	Vector3 lightViewRight;
	lightViewRight.Cross(lightViewUp, lightViewForward);
	lightViewRight.Normalize();

	//���C�g�r���[�ɉ�]��ݒ肵�Ă���
	Matrix lightViewRot;
	//���C�g�r���[�̉���ݒ�
	lightViewRot.m[0][0] = lightViewRight.x;
	lightViewRot.m[0][1] = lightViewRight.y;
	lightViewRot.m[0][2] = lightViewRight.z;
	lightViewRot.m[0][3] = 0.0f;
	//���C�g�r���[�̏��ݒ�
	lightViewRot.m[1][0] = lightViewUp.x;
	lightViewRot.m[1][1] = lightViewUp.y;
	lightViewRot.m[1][2] = lightViewUp.z;
	lightViewRot.m[1][3] = 0.0f;
	//���C�g�r���[�̑O��ݒ肷��B
	lightViewRot.m[2][0] = lightViewForward.x;
	lightViewRot.m[2][1] = lightViewForward.y;
	lightViewRot.m[2][2] = lightViewForward.z;
	lightViewRot.m[2][3] = 0.0f;

	//������𕪊�����䗦
	float shadowAreaTbl[] = {
		m_lightHeight * 0.4f,
		m_lightHeight * 0.8f,
		m_lightHeight * 1.2f
	};

	//���C�g�r���[�̍������v�Z
	float lightHeight = g_camera3D->GetTarget().y + m_lightHeight;

	//�V���h�E�̍\����
	SShadowCb shadowCB;
	//�ߕ���
	float nearPlaneZ = 0.0f;
	//������
	float farPlaneZ;
	//���C���J�����̏����
	Vector3 cameraUp;
	//�O�ςŏ�������v�Z
	cameraUp.Cross(g_camera3D->GetRight(), g_camera3D->GetForward());
	//cameraUp = g_camera3D.GetUp();

	//������𕪊�����悤�Ƀ��C�g�r���[�v���W�F�N�V�����s����v�Z
	for (int i = 0; i < NUM_SHADOW_MAP; i++) {
		//�����ʂ��v�Z
		farPlaneZ = nearPlaneZ + shadowAreaTbl[i];
		//���C�g�r���[
		Matrix mLightView = Matrix::Identity;
		//������̕Е�����(��A��)�v�Z���Ă����̂ŃA���O���͔���
		float halfViewAngle = g_camera3D->GetViewAngle() * 0.5f;
		//�������8���_�����C�g��Ԃɕϊ�����AABB(������̒��ɂ��钼����)�����߂āA���ˉe�̕��ƍ��������߂�B
		float w, h;
		float far_z = -1.0f;
		//8���_
		Vector3 v[8];
		{
			//�וԂ��ΕԂɑ΂��ĉ��{��
			float t = tan(halfViewAngle);
			//�ߕ��ʂƉ����ʂ̏�x�N�g��
			Vector3 toUpperNear, toUpperFar;
			//�Εӂ����߂�B�J�X�P�[�h�V���h�E��toUpper.png�Q�ƁB
			//nearPlaneZ���ו�
			toUpperNear = cameraUp * t * nearPlaneZ;
			toUpperFar = cameraUp * t * farPlaneZ;
			//�A�X�y�N�g�����Z
			//���܂܂ł͂܂������O�̎�����̐��̂݌v�Z�������̂�
			//��������͏c���̏�񂪂͂��邽�߁@�ڍׂ�aspect.png���Q�ƁB
			t *= g_camera3D->GetAspect();
			//�ߕ��ʂ̒������W���v�Z
			//����8vertex.png���Q�ƁB
			auto nearPlaneCenterPos = g_camera3D->GetPosition() + cameraDir * nearPlaneZ;

			//v[0] = ������̋ߕ��ʂŉE��ɂ���l�p�`�̒��_�B
			v[0] = nearPlaneCenterPos + g_camera3D->GetRight() * t * nearPlaneZ + toUpperNear;
			//v[1] = ������̋ߕ��ʂŉE���ɂ���l�p�`�̒��_
			v[1] = v[0] - toUpperNear * 2.0f;

			//v[2] = ������̋ߕ��ʂō���ɂ���l�p�`�̒��_
			v[2] = nearPlaneCenterPos + g_camera3D->GetRight() * -t * nearPlaneZ + toUpperNear;
			//v[3] = ������̋ߕ��ʂō����ɂ���l�p�`�̒��_
			v[3] = v[2] - toUpperNear * 2.0f;

			//�����ʂ̒������W���v�Z�B
			auto farPlaneCenterPos = g_camera3D->GetPosition() + cameraDir * farPlaneZ;

			//v[4] = ������̉����ʂŉE��ɂ���l�p�`�̒��_
			v[4] = farPlaneCenterPos + g_camera3D->GetRight() * t * farPlaneZ + toUpperFar;
			//v[5] = ������̉����ʂŉE���ɂ���l�p�`�̒��_
			v[5] = v[4] - toUpperFar * 2.0f;

			//v[6] = ������̉����ʂō���ɂ���l�p�`�̒��_
			v[6] = farPlaneCenterPos + g_camera3D->GetRight() * -t * farPlaneZ + toUpperFar;
			//v[7] = ������̉����ʂō����ɂ���l�p�`�̒��_
			v[7] = v[6] - toUpperFar * 2.0f;

			//���C�g�s����쐬
			//������̐^�񒆂��v�Z�B
			auto viewFrustumCenterPosition = (nearPlaneCenterPos + farPlaneCenterPos) * 0.5f;
			//���C�g�̈ʒu���v�Z�B
			auto lightPos = CalcLightPosition(lightHeight, viewFrustumCenterPosition);

			//���C�g�̉�]
			mLightView = lightViewRot;

			//���C�g�̈ʒu,��]
			mLightView.m[3][0] = lightPos.x;
			mLightView.m[3][1] = lightPos.y;
			mLightView.m[3][2] = lightPos.z;
			mLightView.m[3][3] = 1.0f;

			//���C�g�r���[(���C�g���W���j����
			//�t�s�񂤂�ʂ񂩂ʂ�Y��Ă���ꍇ��GameProg�ɉ������B
			mLightView.Inverse(mLightView);

			//��������\������W���_�̌v�Z���ł����̂ŁA���C�g��Ԃɕϊ����āAAABB�����߂�B
			//�ő咸�_���W
			Vector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
			//�ŏ����_���W
			Vector3 vMin = { FLT_MAX,FLT_MAX ,FLT_MAX };

			//8���_�����C�g�̍��W���ɕϊ�����B
			for (auto& vInLight : v) {
				mLightView.Mul(vInLight);
				vMax.Max(vInLight);
				vMin.Min(vInLight);
			}
			//�r���[�̕��ƍ��������߂�B
			w = vMax.x - vMin.x;
			h = vMax.y - vMin.y;
			far_z = vMax.z;
		}
		//�v���W�F�N�V�����s��
		Matrix proj;
		//�쐬
		proj.MakeOrthoProjectionMatrix(
			w,
			h,
			far_z / 100.0f,
			far_z 
		);
		m_lightViewMatrix[i] = mLightView;
		m_lightProjMatirx[i] = proj;
		//���C�g���W���ɕϊ�
		m_LVPMatrix[i] = mLightView * proj;
		//�G���e�B�e�B�ɐς�
		m_shadowCBEntity.mLVP[i] = m_LVPMatrix[i];
		//�ǂ��܂ŉe�𗎂Ƃ���(���킫�킾�ƕςȋ��E����������̂ŏ������߂�B)
		m_shadowCBEntity.shadowAreaDepthInViewSpace[i] = farPlaneZ * 0.8f;
		//���̋ߕ��ʂ͍��̉����ʁB
		nearPlaneZ = farPlaneZ;
		//VRAM�ɃR�s�[�B
		m_shadowConstantBuffer.CopyToVRAM(m_shadowCBEntity);
	}
}

Vector3 DirectionalShadowMap::CalcLightPosition(float lightHeight, Vector3 viewFrustomCenterPosition) const 
{
	/*
	���C�g�̍��� = ������̒��S���W.y + ���C�g�̕��� * a
	�ꎟ�������Ȃ̂� a = �ɒ�����
	a = (���C�g�̍��� - ���_�̒��S���W.y) / ���C�g�̕���.y
	�ƂȂ�B
	*/
	auto alpha = (lightHeight - viewFrustomCenterPosition.y) / m_lightDirection.y;
	/*
	a���g���ă��C�g���W�����߂�B
	���C�g�̍��W = ������̒��S���W + ���C�g�̕��� * a
	*/
	auto lightPos = viewFrustomCenterPosition + m_lightDirection * alpha;
	//���C�g�̈ʒu
	return lightPos;
}

void DirectionalShadowMap::RenderToShadowMap()
{
	//�����R���B
	auto& rc = g_graphicsEngine->GetRenderContext();
	if (m_isEnable == true) {
		//�V���h�E�}�b�v�L���������B
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			if (!m_Inited[i]) {
				//�����_�����O�^�[�Q�b�g���ݒ�ł���悤�ɂȂ�܂őҋ@�B
				rc.WaitUntilToPossibleSetRenderTarget(m_shadowMaps[i]);
				m_Inited[i] = true;
			}
			//�����_�[�^�[�Q�b�g�̃Z�b�g�B
			rc.SetRenderTarget(m_shadowMaps[i], m_shadowMaps[i].GetDSVCpuDescriptorHandle());
			//�P�V�P�V
			rc.ClearRenderTargetView(m_shadowMaps[i].GetRTVCpuDescriptorHandle(), m_shadowMaps[i].GetRTVClearColor());
			rc.ClearDepthStencilView(m_shadowMaps[i].GetDSVCpuDescriptorHandle(), 1.0f);
			for (auto& caster : m_shadowCasters) {
				//�J�L�J�L
				caster->Draw(rc, m_lightViewMatrix[i], m_lightProjMatirx[i], enRenderMode_DrawShadow);
				//DX12�̎d�l��A�萔�o�b�t�@�̒l��ύX����ꍇ�̓R�}���h���X�g�ɂ̂������Ă���`�揈����
				//��U�`�悵�āA�R�}���h���X�g���������B
				g_graphicsEngine->EndRender(false);
				//RTV�͂�����mainRenderTarget�ɖ߂�B
				g_graphicsEngine->BeginRender();
			}
		}

		//�J�L�J�L�I������烌���_�[�^�[�Q�b�g�߂��B
		//rc.SetRenderTarget(g_graphicsEngine->GetCurrentFrameBuffuerRTV(), g_graphicsEngine->GetCurrentFrameBufferDSV());
	}

}