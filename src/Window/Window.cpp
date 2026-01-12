#include <Graphitech.hpp>
#include <Window/Window.hpp>
#include <glad/glad.h>
#include <Debugger/Debug.hpp>
#include <Graphitech/Engine/Engine.hpp>
#include <Event/EventDispatcher.hpp>

MOD("Core.Window");

namespace Graphitech {
    Window::Window(WindowDescriptor desc) noexcept : _desc{std::move(desc)} {}

    int32_t Window::Create()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

        _internal = glfwCreateWindow
        (
            static_cast<int32_t>(_desc.width),
            static_cast<int32_t>(_desc.height),
            _desc.title.c_str(),
            nullptr,
            nullptr
        );

        if (_internal == nullptr) {
            DEBUG_CRIT("Failed to open GLFW Window!");
            Destroy();
            return EXIT_FAILURE;
        }

        glfwMakeContextCurrent(_internal);

        return EXIT_SUCCESS;
    }

    void Window::Update()
    {
        WindowEvent e;

        while (true) {
            std::unique_lock<std::mutex> lock{this->_windowEventQueue.mutex};

            if (this->_windowEventQueue.queue.empty())
                break;

            e = std::move(_windowEventQueue.queue.front());
            this->_windowEventQueue.queue.pop();
            lock.unlock();
            HandleEvent(e);
        }
    }

    void Window::Destroy() { glfwTerminate(); }

    bool Window::ShouldClose() { return glfwWindowShouldClose(_internal); }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(_internal);
        glfwPollEvents();
    }

    const WindowDescriptor &Window::GetDescriptor() const noexcept { return _desc; }
    // GetHandle defined inline in header
    void Window::SubscribeEvent(const WindowEvent &e)
    {
        std::lock_guard<std::mutex> lock{this->_windowEventQueue.mutex};
        this->_windowEventQueue.queue.push(e);
    }
    void Window::HandleEvent(const WindowEvent &e)
    {
        switch (e.getId()) {
            case EventId::Window_Size_Change: {
                this->_desc.width = e.args.width;
                this->_desc.height = e.args.height;

                auto sizeChangeEvent = std::make_unique<WindowEvent>(EventId::Window_Size_Change);
                sizeChangeEvent->window = this;
                sizeChangeEvent->args.width = e.args.width;
                sizeChangeEvent->args.height = e.args.height;

                Engine::_eventDispatcher->DispatchEvent(std::move(sizeChangeEvent));
                break;
            }
            default: {
                DEBUG_WARN("Unhandled Event!");
            }
        }
    }
    void Window::HandleResize(int32_t width, int32_t height)
    {
        WindowEvent windowEvent{EventId::Window_Size_Change};

        windowEvent.args.width = width;
        windowEvent.args.height = height;

        SubscribeEvent(windowEvent);
    }
}
