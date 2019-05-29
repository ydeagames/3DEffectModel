//-----------------------------------
//	�G�t�F�N�g�Ǘ��N���X	
//	EffectManager.h
//-----------------------------------
#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

#include <SimpleMath.h>
#include <list>
#include <WICTextureLoader.h>

#include "Myeffect.h"

class EffectManager
{
public:
	void Create(DX::DeviceResources* deviceResources, const std::wstring& image);
	//Create()�őS�������͒����̂ŕ���
	void Initialize(float life,DirectX::SimpleMath::Vector3 pos);

	void Update(DX::StepTimer timer);
	void Render();

	void SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

private:
	//�o�b�`�G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;


	//�G�t�F�N�g�̃|�C���^
	//MyEffect*	m_effect;
	//�G�t�F�N�g���X�g
	std::list<MyEffect*> m_effectList;

	//�e�N�X�`���f�[�^
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;

};