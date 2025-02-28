#ifndef IMGUICOMPONENT
#define IMGUICOMPONENT

#include "RenderComponent.h"

namespace dae
{
    class ImGuiComponent : public RenderComponent
    {
    public:
        void RenderImGui() const override;

        ImGuiComponent(dae::GameObject* object);
        virtual ~ImGuiComponent() = default;
        ImGuiComponent(const ImGuiComponent& other) = delete;
        ImGuiComponent(ImGuiComponent&& other) = delete;
        ImGuiComponent& operator= (const ImGuiComponent& other) = delete;
        ImGuiComponent& operator=(ImGuiComponent&& other) = delete;

    private:
    };
}

#endif