/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** Engine.cpp
*/
#include <Graphitech.hpp>
#include <Graphitech/Engine/Engine.hpp>
#include <Window/Window.hpp>
#include <glad/glad.h>
#include <Application/ApplicationLayer.hpp>
#include <Debugger/Debug.hpp>
#include <Event/EventDispatcher.hpp>
#include <Event/InternalEvents.hpp>

#include "Input/Input.hpp"

MOD("Core.Engine");

namespace Graphitech {
    int32_t Engine::Run(const std::function<ApplicationLayer *()> &initCallback)
    {
        _eventDispatcher = new EventDispatcher();
        
        _applicationLayer = initCallback();
        
        _window = new Window();
        Graphitech::Debug::Enable();

        if (!glfwInit()) {
            DEBUG_CRIT("Failed to initialize GLFW library!");
            Release();
            return EXIT_FAILURE;
        }

        if (_window->Create() != EXIT_SUCCESS) {
            DEBUG_CRIT("Engine: Failed to create window; aborting.");
            Release();
            return EXIT_FAILURE;
        }

        if (!gladLoadGL(glfwGetProcAddress)) {
            DEBUG_CRIT("Failed to initialize GLAD!");
            Release();
            return EXIT_FAILURE;
        }

        InternalEvents::Register(_window->_internal);
        Input::RegisterDevices();

//         _renderer->Setup();
        _applicationLayer->Start();

        while (!_window->ShouldClose()) {
            Update();

//             _renderer->ClearScreen();
            _applicationLayer->Render();
            _window->SwapBuffers();
        }

        Release();
        return EXIT_SUCCESS;
    }

    float Engine::GetElapsedSeconds() { return static_cast<float>(glfwGetTime()); }

    void Engine::Update()
    {
        using namespace std::chrono;

        _eventDispatcher->DispatchEvents();

        const steady_clock::time_point currentTime = steady_clock::now();

        duration<int64_t, std::nano> diff = currentTime - _prevTime;

        if (diff > std::chrono::milliseconds(1)) {
            if (const auto newDiff = milliseconds(static_cast<int64_t>(1000.f / 24.f)); diff > newDiff)
                diff = newDiff;
            _prevTime = currentTime;
            const float deltaTime = static_cast<float>(duration_cast<microseconds>(diff).count()) / 1000000.f;

            _applicationLayer->Update(deltaTime);

//             auto tickEvent = std::make_unique<TickEvent>(EventId::Tick);
//             tickEvent->args.deltaTime = deltaTime;
//             _eventDispatcher->DispatchEvent(std::move(tickEvent));
        }

        Input::Update();
        _window->Update();
    }

    void Engine::Release()
    {
        _applicationLayer->Release();
        Input::ReleaseDevices();
        _window->Destroy();

        SAFE_RELEASE(_eventDispatcher);
//         SAFE_RELEASE(_renderer);
        SAFE_RELEASE(_window);
        SAFE_RELEASE(_applicationLayer);
    }
}
