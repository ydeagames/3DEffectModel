#include "pch.h"
#include "EffectManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void EffectManager::Create(DX::DeviceResources * deviceResources, const std::wstring& image)
{
	auto device = deviceResources->GetD3DDevice();
	auto context = deviceResources->GetD3DDeviceContext();

	//バッチエフェクトの作成
	m_batchEffect = std::make_unique<AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(VertexPositionTexture::InputElements, VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());

	//プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
	
	//テクスチャのロード
	CreateWICTextureFromFile(device, image.c_str(), nullptr, m_texture.GetAddressOf());
	
	// エフェクトクリア
	m_effectList.clear();

	for (int i = 0; i < 100; i++)
	{
		// エフェクト
		auto effect = new MyEffect();
		effect->Create(deviceResources,m_texture.Get(),m_batchEffect.get(),m_batch.get(),m_inputLayout.Get());
		// エフェクト登録
		m_effectList.push_back(effect);
	}
}

void EffectManager::Initialize(float life,Vector3 pos)
{
	int range = 100;
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); ++itr)
	{
		Vector3 vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
		while (vel.Length() < 0.03f)
		{
			vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
		}

		//life,pos,vel の値でm_effectを初期化する
		(*itr)->Initialize(life, pos, vel);
	}
}


void EffectManager::Update(DX::StepTimer timer)
{
	//timerを渡してm_effectの更新処理を行う
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); ++itr)
		(*itr)->Update(timer);
}

void EffectManager::Render()
{
	//m_effectを描画する
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); ++itr)
		(*itr)->Render();
}

void EffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//camera,view,proj,の値をm_effectに渡す
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); ++itr)
		(*itr)->SetRenderState(camera, view, proj);
}
