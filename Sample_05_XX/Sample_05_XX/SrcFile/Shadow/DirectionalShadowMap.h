#pragma once
#include "SrcFile/util/Noncopyable.h"
#include "Model.h"
#include "RenderTarget.h"

/// <summary>
/// �J�X�P�[�h�V���h�E�}�b�v�B
/// </summary>
/// <code>
/// --func call step-- *�����͏ȗ�*
/// 1.Init();
/// 2.(if Needed)SetShadowAreas();
/// 3.RegisterShadowCaster();
/// 4.SkinModel::RegisterShadowReciever();
/// 5.Update();	todo : Engine::Update()
/// 6.RenderToShadowMap() TODO : Engine::Update(); SkinModel�ɍ��Ȃ����ƁB������Draw�Ă΂��B
/// </code>
class DirectionalShadowMap : Noncopyable
{
	struct SShadowCb;
public:
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="w">���̉𑜓x�B</param>
	/// <param name="h">�c�̉𑜓x�B</param>
	/// <param name="lightHeight">���z�̍����B</param>
	void Init(int w, int h, float lightHeight);
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update();
	/// <summary>
	/// �V���h�E�}�b�v��`��
	/// </summary>
	/// <remarks>
	/// �����Ăяo���Ǝ����I�ɃV���h�E�}�b�v�p��RenderTarget�ɕύX����B
	/// �`�悪�I��莟��A���̕`�悵�Ă����t���[���o�b�t�@�ɖ߂�܂��B
	/// todo:�X�L�����f�������_�[�ŏ���ɌĂ΂�鏈���ɂ���B
	/// </remarks>
	/// <param name="rc"></param>
	void RenderToShadowMap();
	/// <summary>
	/// �V���h�E�L���X�^�[�Ƃ��ēo�^�B
	/// <para>�{�֐����Ăяo���ƁA�V���h�E�`�悪�����I��ON�ɂȂ�܂��B</para>
	/// </summary>
	/// <param name="model">�o�^���郂�f���B</param>
	void RegisterShadowCaster(Model* model)
	{
		m_shadowCasters.push_back(model);
		//�V���h�E�L���X�^�̃t���O�B
		model->SetShadwoCaster();
		//�V���h�E�����ŁI
		m_isEnable = true;
	}
	/// <summary>
	/// �����_�[�^�[�Q�b�g���擾�B
	/// todo : protected
	/// </summary>
	/// <param name="shadowNum">�V���h�E�}�b�v�B</param>
	/// <returns>�����_�[�^�[�Q�b�g�B</returns>
	RenderTarget& GetRenderTarget(int shadowNum)
	{
		return m_shadowMaps[shadowNum];
	}
	ConstantBuffer& GetShadowCB() 
	{
		return m_shadowConstantBuffer;
	}
	/// <summary>
	/// �V���h�E�͈̔͂�����B
	/// <para>�f�t�H���g�͂��ׂ�500�B</para>
	/// </summary>
	/// <param name="Area1">1����</param>
	/// <param name="Area2">2����</param>
	/// <param name="Area3">3����</param>
	void SetShadowAreas(float Area1, float Area2, float Area3)
	{
		m_shadowAreas[0] = Area1;
		m_shadowAreas[1] = Area2;
		m_shadowAreas[2] = Area3;
	}
	/// <summary>
	/// �V���h�E�̕`��͈͂��擾�B
	/// </summary>
	/// <param name="shadowNum">�}�b�v�̔ԍ��B</param>
	/// <returns></returns>
	float GetShadowArea(int& shadowNum) const
	{
		return m_shadowAreas[shadowNum];
	}
	/// <summary>
	/// �V���h�E�`�悷��H
	/// </summary>
	/// <param name="flag">�t���O�B</param>
	void SetShadowFlag(bool flag)
	{
		m_isEnable = flag;
	}
private://���[�U�[�����g���K�v�̂Ȃ��֐��B
	/// <summary>
	/// ���C�g�̈ʒu���v�Z�B
	/// </summary>
	/// <remarks>
	/// �����o�ł͂Ȃ��l��ԋp����֐��̂��߁A�Q�Ƃɕς��Ȃ����ƁB
	/// </remarks>
	/// <param name="lightHeight">���C�g�J�����̍����B</param>
	/// <param name="viewFrustomCenterPosition">������̒��S�B</param>
	/// <returns>������^�񒆂̃��C�g�J�����̈ʒu�B</returns>
	Vector3 CalcLightPosition(float lightHeight, Vector3 viewFrustomCenterPosition) const;
private:
	struct SShadowCb {
		Matrix mLVP[NUM_SHADOW_MAP];						//���C�g�r���[�v���W�F�N�V�����B
		float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//�J������Ԃł̉e�𗎂Ƃ��G���A�̐[�x�e�[�u���B
	};
	RenderTarget m_shadowMaps[NUM_SHADOW_MAP];	//�V���h�E�}�b�v�B
	std::vector<Model*> m_shadowCasters;		//�V���h�E�L���X�^�[
	Matrix m_LVPMatrix[NUM_SHADOW_MAP];			//���C�g�r���[�v���W�F�N�V�����s��B
	Matrix m_lightViewMatrix[NUM_SHADOW_MAP];	//���C�g�r���[�s��
	Matrix m_lightProjMatirx[NUM_SHADOW_MAP];	//���C�g�v���W�F�N�V�����s��
	SShadowCb m_shadowCBEntity;					//�V���h�E�̒l�B
	ConstantBuffer m_shadowConstantBuffer;		//�V���h�E�p�萔�o�b�t�@�B
	Vector3 m_lightDirection = { 0.0f, -1.0f, 0.0f };	//���C�g�̕����B
	float m_lightHeight;						//���C�g�̍����B
	bool m_isEnable = false;					//�V���h�E�}�b�v���L���H
	bool m_Inited[NUM_SHADOW_MAP] = {false};						//�V���h�E�̃��\�[�X�o���A�Ă΂ꂽ�H
	float m_shadowAreas[NUM_SHADOW_MAP] = { 500.0f,500.0f,500.0f };		//�e�̗�����͈́B
};

