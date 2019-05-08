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
{
}

ModelObject::~ModelObject()
{
}

void ModelObject::Update(DX::StepTimer const& timer)
{
	Matrix scale = Matrix::CreateScale(Vector3::One * 5.f);

	// ��̓���
	float time = float(timer.GetTotalSeconds());
	m_world = scale
		* Matrix::CreateRotationY(XMConvertToRadians(90))
		* Matrix::CreateRotationX(time + XMConvertToRadians(90))
		* Matrix::CreateTranslation(Vector3::Forward * 5.f)
		* Matrix::CreateRotationY(time)
		;
}

void ModelObject::Lost()
{
	m_states.reset();
	m_fxFactory.reset();
	m_model.reset();
}

void ModelObject::Create(DX::DeviceResources* deviceResources)
{
	m_deviceResources = deviceResources;

	ID3D11Device1* device = m_deviceResources->GetD3DDevice();

	// �R�����X�e�[�g���쐬����
	m_states = std::make_unique<CommonStates>(device);
	// �G�t�F�N�g�t�@�N�g���[���쐬����
	m_fxFactory = std::make_unique<EffectFactory>(device);
	// CMO��ǂݍ���Ń��f�����쐬����
	m_model = Model::CreateFromCMO(device, L"cup.cmo", *m_fxFactory);
	// ���[���h�s����쐬����B
	m_world = Matrix::Identity;

	//���f���I�u�W�F�N�g�ł��ׂ��ł͂Ȃ�
	//m_view = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
	// Vector3::Zero, Vector3::UnitY);
	//m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
	//	float(backBufferWidth) / float(backBufferHeight), 0.1f, 10.f);
}

void ModelObject::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// �����Ńr���[�s��ƃv���W�F�N�V�����s������炤
	ID3D11DeviceContext1* context = m_deviceResources->GetD3DDeviceContext();
	m_model->Draw(context, *m_states, m_world, view, proj);
}