#include "stdafx.h"
#include "CEffectEngine.h"

CEffectEngine::CEffectEngine()
{
}

CEffectEngine::~CEffectEngine()
{
	//���
	Release();
}

void CEffectEngine::Release()
{
	//�}�l�[�W���[�Ƃ��̓G�t�F�N�V�A���ŃX�}�[�g�|�C���^�Ƃ��ĊǗ�����Ă邩��A
	//��������Ȃ��đ��v�Ȃ͂��B
}

void CEffectEngine::Init()
{
	auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//�����_���[���������B
	m_effekseerRenderer = EffekseerRendererDX12::Create(
		GraphicsEngineObj()->GetD3DDevice(),			//D3D�f�o�C�X�B
		GraphicsEngineObj()->GetCommandQueue(),			//�R�}���h�L���[�B
		3,												//�X���b�v�o�b�t�@�[�J�E���g�B
		&format,										//�t�H�[�}�b�g�J���[�B
		1,												//RT�̐��B
		DXGI_FORMAT_D32_FLOAT,							//�f�v�X�t�H�[�}�b�g�B
		false,											//�f�v�X���]�H�H
		2000											//�|���̍ő吔�B
	);
	//auto tex = EffekseerRendererDX12::CreateTexture(m_effekseerRenderer, GraphicsEngineObj()->GetDepthTextrue()
	////�f�v�X���쐬�B
	//m_effekseerRenderer->SetDepth();
	// Create a memory pool
	// �������v�[���̍쐬
	m_sfMemoryPoolEfk = EffekseerRendererDX12::CreateSingleFrameMemoryPool(m_effekseerRenderer);
	// Create a command list
	// �R�}���h���X�g�̍쐬
	m_commandListEfk = EffekseerRendererDX12::CreateCommandList(m_effekseerRenderer, m_sfMemoryPoolEfk);
	
	//�G�t�F�N�g�}�l�[�W�����������B
	m_manager = Effekseer::Manager::Create(10000);

	// �`��p�C���X�^���X����`��@�\��ݒ�
	m_manager->SetSpriteRenderer(m_effekseerRenderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(m_effekseerRenderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(m_effekseerRenderer->CreateRingRenderer());
	m_manager->SetTrackRenderer(m_effekseerRenderer->CreateTrackRenderer());
	m_manager->SetModelRenderer(m_effekseerRenderer->CreateModelRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	m_manager->SetTextureLoader(m_effekseerRenderer->CreateTextureLoader());
	m_manager->SetModelLoader(m_effekseerRenderer->CreateModelLoader());
	m_manager->SetMaterialLoader(m_effekseerRenderer->CreateMaterialLoader());
}

void CEffectEngine::Update()
{
	//�t���[���J�n���ɌĂԁB
	m_sfMemoryPoolEfk->NewFrame();
	// �R�}���h���X�g���J�n����B
	EffekseerRendererDX12::BeginCommandList(m_commandListEfk, GraphicsEngineObj()->GetCommandList());
	//�R�}���h���X�g�ݒ�B
	m_effekseerRenderer->SetCommandList(m_commandListEfk);
	//�G�t�F�N�T�[�̃J�����ݒ�
	//m_effekseerRenderer->SetCameraMatrix(GraphicsEngineObj()->GetCamera3D().GetViewMatrix());
	//m_effekseerRenderer->SetProjectionMatrix(GraphicsEngineObj()->GetCamera3D().GetViewMatrix());
	//�X�V
	m_manager->Update();
}

void CEffectEngine::Render()
{
	//g_camera����J�����s��ƃv���W�F�N�V�����s����R�s�[
	Effekseer::Matrix44 efCameraMat;
	GraphicsEngineObj()->GetCamera3D().GetViewMatrix().CopyTo(efCameraMat);
	Effekseer::Matrix44 efProjMat;
	GraphicsEngineObj()->GetCamera3D().GetProjectionMatrix().CopyTo(efProjMat);

	//�J�����s��ƃv���W�F�N�V�����s������_�ɐݒ�B
	//tkEngine���̓J�����̒����ɏo��悤�ɂȂ��Ă���K�X
	m_effekseerRenderer->SetCameraMatrix(efCameraMat);
	m_effekseerRenderer->SetProjectionMatrix(efProjMat);

	//Effekseer���X�V�B
	m_manager->Update();

	//�G�t�F�N�g�`��p�̃����_�����O�ݒ�B
	m_effekseerRenderer->BeginRendering();
	//�G�t�F�N�g�̕`�揈��
	m_manager->Draw();
	//�����_�����O�ݒ�����Ƃɖ߂��B
	m_effekseerRenderer->EndRendering();
}

Effekseer::EffectRef CEffectEngine::CreateEffect(const wchar_t* filepath)
{
	//�쐬
	return Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)filepath);
}

Effekseer::Handle CEffectEngine::Play(Effekseer::EffectRef& effect)
{
	//Effect��Update�Ń��[���h���W�͌v�Z
	return m_manager->Play(effect, 0, 0, 0);
}