//----------------------------------------
// MyEffect.cpp
//----------------------------------------

#include "pch.h"
#include "MyEffect.h"

#include "DeviceResources.h"
#include "StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;


void MyEffect::Create(DX::DeviceResources* deviceResources,ID3D11ShaderResourceView* texture,AlphaTestEffect* batchEffect, DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* batch, ID3D11InputLayout* inputLayout)
{
	m_deviceResources = deviceResources;
	auto device = m_deviceResources->GetD3DDevice();
	// �G�t�F�N�g�̎擾
	m_batchEffect = batchEffect;
	// ���̓��C�A�E�g�擾
	m_inputLayout = inputLayout;

	// �v���~�e�B�u�o�b�`�̎擾
	m_batch = batch;

	m_states = std::make_unique<CommonStates>(device);
	// �e�N�X�`���̎擾
	m_texture = texture;
}

void MyEffect::Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity)
{
	m_position = pos;
	m_velocity = velocity;
	m_life =  life;
	
}

void MyEffect::Update(DX::StepTimer timer)
{
	float time = float(m_timer.GetElapsedSeconds());
	m_timer = timer;
	

	m_position += m_velocity;

	m_life -= time;
}

void MyEffect::Render()
{
	if (m_life < 0) return;
	m_world = Matrix::CreateBillboard(m_position, m_camera, Vector3::UnitY);
	Draw();

}

void MyEffect::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_camera = camera;
	m_view = view;
	m_proj = proj;
}

void MyEffect::Draw()
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	// TODO: Add your rendering code here.
	context;


	// ���_���
	VertexPositionTexture vertex[4] =
	{	
		VertexPositionTexture(Vector3(0.5f, 0.5f, 0.0f), Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(-0.5f, 0.5f, 0.0f), Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(-0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(0.5f,-0.5f, 0.0f), Vector2(0.0f, 1.0f)),
	};
	// �e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j
	ID3D11SamplerState* samplers[1] = { m_states->LinearClamp() };
	context->PSSetSamplers(0, 1, samplers);
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	CD3D11_DEFAULT default_state;

	CD3D11_BLEND_DESC bddesc(default_state);

	blendstate->GetDesc(&bddesc);
	bddesc.AlphaToCoverageEnable = TRUE;
	m_deviceResources->GetD3DDevice()->CreateBlendState(&bddesc, &blendstate);

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// �J�����O�͍�����
	context->RSSetState(m_states->CullCounterClockwise());
	// �s�����̂ݕ`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_batchEffect->SetReferenceAlpha(0);
	m_batchEffect->SetWorld(m_world);
	m_batchEffect->SetView(m_view);
	m_batchEffect->SetProjection(m_proj);
	m_batchEffect->SetTexture(m_texture);
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout);

	// ������������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0],vertex[1],vertex[2],vertex[3]);
	m_batch->End();
}