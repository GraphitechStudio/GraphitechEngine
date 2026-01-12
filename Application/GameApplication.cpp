/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** GameApplication.cpp - Base class implementation
*/

#include "GameApplication.hpp"
#include <Rendering/SceneRenderer.hpp>
#include <iostream>

namespace Graphitech {

    void GameApplication::InitializeRenderer(int logicalWidth, int logicalHeight) {
        renderer_ = std::make_unique<SceneRenderer>();
        renderer_->setLogicalSize(logicalWidth, logicalHeight);
        std::cout << "[GameApplication] Renderer initialized (" << logicalWidth 
                  << "x" << logicalHeight << ")\n";
    }

    void GameApplication::RenderScene() {
        if (renderer_) {
            renderer_->render(scene_);
        }
    }

    void GameApplication::OnEnable() {
        std::cout << "[GameApplication] OnEnable\n";
    }

    void GameApplication::OnDisable() {
        std::cout << "[GameApplication] OnDisable\n";
        StopListening();
    }

    void GameApplication::OnRender() {
        RenderScene();
    }

} // namespace Graphitech
