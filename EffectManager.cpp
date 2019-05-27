#include "pch.h"
#include "EffectManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void EffectManager::Create(DX::DeviceResources * deviceResources)
{
	auto device = deviceResources->GetD3DDevice();
	auto context = deviceResources->GetD3DDeviceContext();

	//�o�b�`�G�t�F�N�g�̍쐬
	m_batchEffect = std::make_unique<AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(VertexPositionTexture::InputElements, VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());

	//�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
	
	//�e�N�X�`���̃��[�h
	CreateWICTextureFromFile(device, L"Resources/Textures/image01.png", nullptr, m_texture.GetAddressOf());
	

	m_effect = new MyEffect();
	m_effect->Create(deviceResources,m_texture.Get(),m_batchEffect.get(),m_batch.get(),m_inputLayout.Get());
		
}

void EffectManager::Initialize(float life,Vector3 pos)
{
	int range = 100;
	Vector3 vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
	while (vel.Length() < 0.03f)
	{
		vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
	}

	//life,pos,vel �̒l��m_effect������������
	m_effect->Initialize(life, pos, vel);
}


void EffectManager::Update(DX::StepTimer timer)
{
	//timer��n����m_effect�̍X�V�������s��
	m_effect->Update(timer);
}

void EffectManager::Render()
{
	//m_effect��`�悷��
	m_effect->Render();
}

void EffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//camera,view,proj,�̒l��m_effect�ɓn��
	m_effect->SetRenderState(camera, view, proj);
}
