//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

#include <SimpleMath.h>
#include <Effects.h>
#include <CommonStates.h>
#include <Model.h>

#include "Trigger.h"

//class ModelObject;
class EffectManager;
class GridFloor;
class DebugCamera;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify
{
public:

	Game() noexcept(false);
	~Game() noexcept(false);

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

	//DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	//std::unique_ptr<DirectX::CommonStates> m_states;
	//std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
	//std::unique_ptr<DirectX::Model> m_model;

	//std::vector<std::unique_ptr<ModelObject>> m_modelObjects;

	// マウス
	std::unique_ptr<DirectX::Mouse>			m_pMouse;
	// キーボード
	std::unique_ptr<DirectX::Keyboard>		m_pKeyboard;

	std::unique_ptr<DirectX::CommonStates> m_commonStates;

	std::unique_ptr<EffectManager> m_effectManager;

	std::unique_ptr<DebugCamera> m_debugCamera;
	std::unique_ptr<GridFloor> m_gridFloor;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;

	HWND m_window;

public:
	static Trigger<bool> s_exitrequest;
	static Trigger<bool> s_exitaccept;
};