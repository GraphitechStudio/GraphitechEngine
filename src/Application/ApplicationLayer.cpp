#include <Application/ApplicationLayer.hpp>

namespace Graphitech {
    ApplicationLayer::ApplicationLayer(const ApplicationLayerDescriptor &desc) noexcept
        : _desc(desc) {}

    void ApplicationLayer::Start()
    {
        OnStart();

        if (this->_desc._enabled)
            Enable();

        if (this->_desc._visible)
            Show();
    }

    void ApplicationLayer::Enable()
    {
        this->_desc._enabled = true;
        OnEnable();
    }

    void ApplicationLayer::Disable()
    {
        this->_desc._enabled = false;
        OnDisable();
    }

    void ApplicationLayer::Show()
    {
        this->_desc._visible = true;
        OnShow();
    }

    void ApplicationLayer::Hide()
    {
        this->_desc._visible = false;
        OnHide();
    }

    void ApplicationLayer::Update(float deltaTime)
    {
        if (this->_desc._enabled)
            OnUpdate(deltaTime);
    }

    void ApplicationLayer::Render()
    {
        if (this->_desc._visible)
            OnRender();
    }

    void ApplicationLayer::Release() { OnRelease(); }
    const ApplicationLayerDescriptor &ApplicationLayer::GetDescriptor() const noexcept { return this->_desc; }
    void ApplicationLayer::OnStart() {}
    void ApplicationLayer::OnEnable() {}
    void ApplicationLayer::OnDisable() {}
    void ApplicationLayer::OnShow() {}
    void ApplicationLayer::OnHide() {}
    void ApplicationLayer::OnUpdate(float deltaTime) {}
    void ApplicationLayer::OnRender() {}
    void ApplicationLayer::OnRelease() {}
}
