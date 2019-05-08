#pragma once

#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <CommonStates.h>
#include <Model.h>
#include "StepTimer.h"

class ModelObject
{
public:
	ModelObject();
	~ModelObject();

public:
	// 更新
	void Update(DX::StepTimer const& timer);
	// 後始末
	void Lost();
	// 作成
	void Create(DX::DeviceResources* deviceResources);
	// 描画 (ビュー行列とプロジェクション行列を外部から受け取る)
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

private:
	DX::DeviceResources* m_deviceResources;

	DirectX::SimpleMath::Matrix m_world;
	//DirectX::SimpleMath::Matrix m_view;
	//DirectX::SimpleMath::Matrix m_proj;

	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
	std::unique_ptr<DirectX::Model> m_model;
};