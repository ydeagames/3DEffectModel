#include "pch.h"
#include "EffectManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void EffectManager::Create(DX::DeviceResources * deviceResources, const std::wstring& image)
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
	CreateWICTextureFromFile(device, image.c_str(), nullptr, m_texture.GetAddressOf());

	// �G�t�F�N�g�N���A
	m_effectList.clear();

	for (int i = 0; i < 300; i++)
	{
		// �G�t�F�N�g
		auto effect = new MyEffect();
		effect->Create(deviceResources, m_texture.Get(), m_batchEffect.get(), m_batch.get(), m_inputLayout.Get());
		// �G�t�F�N�g�o�^
		m_effectList.push_back(effect);
	}
}

void EffectManager::Initialize(float life, Vector3 pos)
{
	//const float RAD = XM_PI * 2;

	int num = 0;
	int range = 100;
	//int sqrSize = static_cast<int>(std::sqrtf(m_effectList.size()));
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); ++itr)
	{
		Vector3 vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
		while (vel.Length() < 0.03f)
		{
			vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
		}

		//int size = sqrSize;

		//float alpha = RAD * (num / size) / sqrSize;
		//float phi = RAD * (num % size) / sqrSize;
		////Vector3 vel = Vector3(cos(alpha), sin(alpha), 0.f);
		//Vector3 vel = Vector3(sin(alpha) * cos(phi), sin(alpha) * sin(phi), cos(alpha));

		//life,pos,vel �̒l��m_effect������������
		(*itr)->Initialize(life, pos, vel);
		num++;
	}
}

void EffectManager::InitializeAverage(float life, Vector3 pos)
{
	const float RAD = XM_PI * 2;

	int num = 0;
	//int range = 100;
	int sqrSize = static_cast<int>(std::sqrtf(m_effectList.size()));
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); ++itr)
	{
		//Vector3 vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
		//while (vel.Length() < 0.03f)
		//{
		//	vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
		//}

		int size = sqrSize;

		float alpha = RAD * (num / size) / sqrSize;
		float phi = RAD * (num % size) / sqrSize;
		//Vector3 vel = Vector3(cos(alpha), sin(alpha), 0.f);
		Vector3 vel = Vector3(sin(alpha) * cos(phi), sin(alpha) * sin(phi), cos(alpha));

		//life,pos,vel �̒l��m_effect������������
		(*itr)->Initialize(life, pos, vel);
		num++;
	}
}

void EffectManager::InitializeCone(float life, Vector3 pos, Vector3 dir)
{
	const float RAD = XM_PI * 2;

	dir.Normalize();
	dir *= .04f;

	auto cross0 = Vector3(dir.y, -dir.x, 0); // ��������x�N�g��

	int num = 0;
	int range = 100;
	//int sqrSize = static_cast<int>(std::sqrtf(m_effectList.size()));
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); ++itr)
	{
		auto d = dir;
		d *= sinf(rand()) + 1.f;

		auto cross = cross0;
		cross *= sinf(rand());

		d += cross;
		d *= .1f;

		//life,pos,vel �̒l��m_effect������������
		(*itr)->Initialize(life, pos, d);
		num++;
	}
}

void EffectManager::InitializeRandom(float life, Vector3 centerPos)
{
	int num = 0;
	int range = 100;
	//int sqrSize = static_cast<int>(std::sqrtf(m_effectList.size()));
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); ++itr)
	{
		auto pos = Vector3(((rand() % (range * 2)) - range) / (float)range, ((rand() % (range * 2)) - range) / (float)range, 0) * 5.f;
		auto vel = (centerPos - pos) / life;
		vel *= ((rand() % range) / (float)range) * .0055f + .001f;

		//life,pos,vel �̒l��m_effect������������
		(*itr)->Initialize(life + (((rand() % (range * 2)) - range) / (float)range) * .25f, pos + Vector3::Backward * (((rand() % (range * 2)) - range) / (float)range * .3f), vel);
		(*itr)->m_life = (*itr)->m_startLife * ((rand() % range) / (float)range);
		(*itr)->SetGravity(false);
		(*itr)->SetLoop(true);
		num++;
	}
}

void EffectManager::InitializeBoom(float life, DirectX::SimpleMath::Vector3 centerPos)
{
	int num = 0;
	int range = 100;
	//int sqrSize = static_cast<int>(std::sqrtf(m_effectList.size()));
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); ++itr)
	{
		auto pos = (*itr)->GetPosition();
		auto vel = - (centerPos - pos) / life;
		vel *= ((rand() % range) / (float)range) * .5f;
		vel += Vector3::Backward * (((rand() % (range * 2)) - range) / (float)range) * .15f;

		//life,pos,vel �̒l��m_effect������������
		(*itr)->Initialize(life + (((rand() % (range * 2)) - range) / (float)range) * .25f, pos + Vector3::Backward * (((rand() % (range * 2)) - range) / (float)range * .3f), vel);
		(*itr)->m_life = (*itr)->m_startLife * ((rand() % range) / (float)range);
		(*itr)->SetGravity(true);
		(*itr)->SetLoop(false);
		num++;
	}
}


void EffectManager::Update(DX::StepTimer timer)
{
	//timer��n����m_effect�̍X�V�������s��
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); ++itr)
		(*itr)->Update(timer);
}

void EffectManager::Render()
{
	//m_effect��`�悷��
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); ++itr)
		(*itr)->Render();
}

void EffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//camera,view,proj,�̒l��m_effect�ɓn��
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); ++itr)
		(*itr)->SetRenderState(camera, view, proj);
}
