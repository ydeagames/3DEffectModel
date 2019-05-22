#pragma once

#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <CommonStates.h>
#include <Model.h>
#include "StepTimer.h"

class Transform
{
public:
	std::shared_ptr<Transform> parent;
	std::function<DirectX::SimpleMath::Matrix(DX::StepTimer const&)> transform;

public:
	Transform(const std::shared_ptr<Transform>& parent, const std::function<DirectX::SimpleMath::Matrix(DX::StepTimer const&)>& transform)
		: parent(parent)
		, transform(transform) {}

public:
	inline DirectX::SimpleMath::Matrix GetMatrix(DX::StepTimer const& timer) const
	{
		DirectX::SimpleMath::Matrix world = transform(timer);
		if (parent)
			world *= parent->GetMatrix(timer);
		return world;
	}
};

class ModelObject
{
private:
	class SharedModel
	{
	public:
		DX::DeviceResources* m_deviceResources;

		std::unique_ptr<DirectX::CommonStates> m_states;
		std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
		std::unique_ptr<DirectX::Model> m_model;

	public:
		SharedModel(DX::DeviceResources* deviceResources, const std::wstring& modelName);
		~SharedModel();
	};

public:
	ModelObject();
	~ModelObject();

public:
	// 更新
	void Update(DX::StepTimer const& timer);
	// 後始末
	void Lost();
	// 作成
	void Create(DX::DeviceResources* deviceResources, const std::wstring& modelName);
	// 描画 (ビュー行列とプロジェクション行列を外部から受け取る)
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

public:
	std::shared_ptr<Transform> transform;

private:

	DirectX::SimpleMath::Matrix m_world;
	//DirectX::SimpleMath::Matrix m_view;
	//DirectX::SimpleMath::Matrix m_proj;

	std::shared_ptr<std::unique_ptr<SharedModel>> m_sharedModel;
};
