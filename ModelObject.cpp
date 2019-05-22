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
	// �����Ńr���[�s��ƃv���W�F�N�V�����s������炤
	ID3D11DeviceContext1* context = (*m_sharedModel)->m_deviceResources->GetD3DDeviceContext();
	(*m_sharedModel)->m_model->Draw(context, *(*m_sharedModel)->m_states, m_world, view, proj);
}

ModelObject::SharedModel::SharedModel(DX::DeviceResources* deviceResources, const std::wstring& modelName)
{
	m_deviceResources = deviceResources;

	ID3D11Device1* device = m_deviceResources->GetD3DDevice();

	// �R�����X�e�[�g���쐬����
	m_states = std::make_unique<CommonStates>(device);
	// �G�t�F�N�g�t�@�N�g���[���쐬����
	m_fxFactory = std::make_unique<EffectFactory>(device);
	// CMO��ǂݍ���Ń��f�����쐬����
	m_model = Model::CreateFromCMO(device, modelName.c_str(), *m_fxFactory);
}

ModelObject::SharedModel::~SharedModel()
{
	m_states.reset();
	m_fxFactory.reset();
	m_model.reset();
}