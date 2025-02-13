#ifndef TEXTURECOMPONENT
#define TEXTURECOMPONENT

#include "RenderComponent.h"
#include <string>
#include <memory>
#include "Texture2D.h"
#include <vector>

namespace dae
{
	class TextureComponent : public RenderComponent
	{
    public:
        void Render() const override;

        void AddTexture(const std::string& filename);
        void AddTexture(const std::shared_ptr<Texture2D>& texture);

        int GetCurrentIndex() const { return m_CurrentSpriteIndex; }
        void SetCurrentIndex(int const newIndex);

        glm::ivec2 GetSize() const { return m_pTextures[m_CurrentSpriteIndex]->GetSize(); }

        TextureComponent(dae::GameObject* object);
        //TextureComponent(dae::GameObject* object, const std::string& filename);
        virtual ~TextureComponent() override = default;
        TextureComponent(const TextureComponent& other) = delete;
        TextureComponent(TextureComponent&& other) = delete;
        TextureComponent& operator= (const TextureComponent& other) = delete;
        TextureComponent& operator=(TextureComponent&& other) = delete;

    private:
        std::vector<std::shared_ptr<Texture2D>> m_pTextures{};
        int m_CurrentSpriteIndex{};
	};
}

#endif