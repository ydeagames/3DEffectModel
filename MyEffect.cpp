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


void MyEffect::Create(DX::DeviceResources* deviceResources, ID3D11ShaderResourceView* texture, AlphaTestEffect* batchEffect, DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* batch, ID3D11InputLayout* inputLayout)
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

	m_isLoop = true;
}

void MyEffect::Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity)
{
	m_startPosition = m_position = pos;
	//velocity.Normalize();
	//velocity *= .04f;
	m_startVelocity = m_velocity = velocity;
	m_startLife = m_life = life;
}

void MyEffect::Update(DX::StepTimer timer)
{
	float time = float(m_timer.GetElapsedSeconds());
	m_timer = timer;


	m_position += m_velocity;
	if (m_isGravity)
		m_velocity += Vector3::Down * .0098f;

	m_life -= time;

	if (m_isLoop && m_life < 0)
	{
		Restart();
		return;
	}

	//Vector3 length = m_position - m_startPosition;
	//if (length.LengthSquared() > 4 * 4)
	//{
	//	Restart();
	//}
}

void MyEffect::Restart()
{
	m_position = m_startPosition;
	m_velocity = m_startVelocity;
	m_life = m_startLife;
}

void MyEffect::Render()
{
	if (m_life < 0) return;
	//auto sizeOverLifetime = 1 - (1 - m_life / m_startLife) * (1 - m_life / m_startLife);
	//Vector3 length = m_position - m_startPosition;
	//auto sizeOverLength = 1 - length.LengthSquared() / (4 * 4);
	//m_world = Matrix::CreateScale(1)* Matrix::CreateRotationY(XMConvertToRadians(180)) * Matrix::CreateBillboard(m_position, Vector3::Zero, Vector3::UnitY) ;
	//m_world = Matrix::CreateBillboard(m_position, m_camera, Vector3::UnitY) ;
	m_world = Matrix::CreateBillboard(m_position, Vector3(m_position.x, m_position.y, m_camera.z > 0 ? 5 : -5), Vector3::UnitY);
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
	ID3D11BlendState* blendstate = m_states->Additive();

	CD3D11_DEFAULT default_state;

	CD3D11_BLEND_DESC bddesc(default_state);

	blendstate->GetDesc(&bddesc);
	bddesc.AlphaToCoverageEnable = TRUE;
	m_deviceResources->GetD3DDevice()->CreateBlendState(&bddesc, &blendstate);

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	// �J�����O�͍�����
	context->RSSetState(m_states->CullCounterClockwise());
	// �s�����̂ݕ`�悷��ݒ�
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_batchEffect->SetReferenceAlpha(0);
	float lifeprogress = 1 - m_life / m_startLife;
	float lifesin = (-std::cos(lifeprogress * 3 * XM_2PI) + 1) / 2.f;
	float halfcirc = std::sinf(lifeprogress * XM_PI);
	//std::cout << lifeprogress << " : " << lifesin << " : " << halfcirc << std::endl;
	m_batchEffect->SetAlpha((lifesin + halfcirc) / 4.f + 0.4f);
	//m_batchEffect->SetAlpha(.6f);
	m_batchEffect->SetWorld(m_world);
	m_batchEffect->SetView(m_view);
	m_batchEffect->SetProjection(m_proj);
	m_batchEffect->SetTexture(m_texture);
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout);

	// ������������`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();
}
