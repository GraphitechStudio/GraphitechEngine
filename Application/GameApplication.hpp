/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** GameApplication.hpp - Base class for game applications
*/

#ifndef GAMEAPPLICATION_HPP
#define GAMEAPPLICATION_HPP

#include <Application/ApplicationLayer.hpp>
#include <Event/EventHandler.hpp>
#include <Graphitech/ECS/ECS.hpp>
#include <memory>

namespace Graphitech {

    class SceneRenderer;

    /**
     * @brief Base class for game applications built on the Graphitech Engine.
     *
     * @details Provides a standard structure for game applications with:
     * - ECS scene management
     * - Event handling (keyboard, mouse)
     * - Rendering pipeline integration
     * - Standard lifecycle hooks (OnStart, OnUpdate, OnRender)
     *
     * Games should inherit from this class and implement game-specific logic.
     *
     * @ingroup Engine
     */
    class GameApplication : public ApplicationLayer, public EventHandler {
    public:
        GameApplication() = default;
        virtual ~GameApplication() = default;

        // Disable copy
        GameApplication(const GameApplication&) = delete;
        GameApplication& operator=(const GameApplication&) = delete;

    protected:
        /**
         * @brief Gets the ECS scene.
         * @return Reference to the current scene
         */
        ECS::Scene& GetScene() noexcept { return scene_; }
        const ECS::Scene& GetScene() const noexcept { return scene_; }

        /**
         * @brief Gets the renderer.
         * @return Pointer to the scene renderer (may be nullptr if not initialized)
         */
        SceneRenderer* GetRenderer() noexcept { return renderer_.get(); }

        /**
         * @brief Initializes the renderer with logical viewport size.
         * @param logicalWidth Logical width for game world coordinates
         * @param logicalHeight Logical height for game world coordinates
         */
        void InitializeRenderer(int logicalWidth, int logicalHeight);

        /**
         * @brief Renders the current scene.
         * 
         * @details Called automatically in OnRender() if renderer is initialized.
         * Can be overridden for custom rendering logic.
         */
        virtual void RenderScene();

        // ApplicationLayer interface - can be overridden by derived classes
        void OnEnable() override;
        void OnDisable() override;
        void OnRender() override;

        // Pure virtual methods that must be implemented by game
        void OnStart() override = 0;
        void OnUpdate(float deltaTime) override = 0;

    private:
        ECS::Scene scene_;
        std::unique_ptr<SceneRenderer> renderer_;
    };

} // namespace Graphitech

#endif // GAMEAPPLICATION_HPP
