#include "pch.h"
#include "ModelObject.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <CommonStates.h>
#include <Model.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

ModelObject::ModelObject()
	: m_sharedModel(std::make_shared<std::unique_ptr<SharedModel>>(std::unique_ptr<SharedModel>(nullptr)))
{
}

ModelObject::~ModelObject()
{
}

void ModelObject::Update(DX::StepTimer const& timer)
{
	Matrix scale = Matrix::CreateScale(Vector3::One * 5.f);

	m_world = transform->GetMatrix(timer);
}

void ModelObject::Lost()
{
	m_sharedModel = nullptr;
}

void ModelObject::Create(DX::DeviceResources* deviceResources, const std::wstring& modelName)
{
	std::unique_ptr<SharedModel>& sharedModel = *m_sharedModel;
	if (!sharedModel)
		sharedModel = std::make_unique<SharedModel>(deviceResources, modelName);
}

void ModelObject::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// 引数でビュー行列とプロジェクション行列をもらう
	ID3D11DeviceContext1* context = (*m_sharedModel)->m_deviceResources->GetD3DDeviceContext();
	(*m_sharedModel)->m_model->Draw(context, *(*m_sharedModel)->m_states, m_world, view, proj);
}

ModelObject::SharedModel::SharedModel(DX::DeviceResources* deviceResources, const std::wstring& modelName)
{
	m_deviceResources = deviceResources;

	ID3D11Device1* device = m_deviceResources->GetD3DDevice();

	// コモンステートを作成する
	m_states = std::make_unique<CommonStates>(device);
	// エフェクトファクトリーを作成する
	m_fxFactory = std::make_unique<EffectFactory>(device);
	// CMOを読み込んでモデルを作成する
	m_model = Model::CreateFromCMO(device, modelName.c_str(), *m_fxFactory);
}

ModelObject::SharedModel::~SharedModel()
{
	m_states.reset();
	m_fxFactory.reset();
	m_model.reset();
}