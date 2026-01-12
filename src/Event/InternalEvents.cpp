/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** InternalEvent.cpp
*/
#include <Event/InternalEvents.hpp>
#include <Window/Window.hpp>
#include <Graphitech/Engine/Engine.hpp>
#include <Input/Input.hpp>
#include <Input/Keyboard/Keyboard.hpp>
#include <Input/Mouse/Mouse.hpp>
#include <Debugger/Debug.hpp>
#include <sstream>

MOD("Core.Event.Internal");

void Graphitech::InternalEvents::Register(GLFWwindow *internalWindow)
{
    DEBUG_INFO("Registering internal events");
    glfwSetFramebufferSizeCallback(internalWindow, FrameBufferSizeCallback);

    glfwSetInputMode(internalWindow, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
    glfwSetKeyCallback(internalWindow, KeyCallback);
    glfwSetMouseButtonCallback(internalWindow, MouseButtonCallback);
    glfwSetCursorPosCallback(internalWindow, MouseMotionCallback);
    glfwSetScrollCallback(internalWindow, MouseScrollCallback);
}

void Graphitech::InternalEvents::FrameBufferSizeCallback(GLFWwindow* /*windowHandle*/, int width, int height)
{
    Engine::_window->HandleResize(width, height);
}

void Graphitech::InternalEvents::KeyCallback(GLFWwindow* /*internalWindow*/, int32_t key, int32_t /*scanCode*/, int32_t action,
                                             int32_t modifiers)
{
    switch (action) {
        case GLFW_PRESS: {
            KeyCode k = static_cast<KeyCode>(key);
            k = Input::RemapKeyForLayout(k);
            Input::_keyboard->HandleKeyPress(k, static_cast<KeyModifiers>(modifiers));
            break;
        }
        case GLFW_RELEASE: {
            KeyCode k = static_cast<KeyCode>(key);
            k = Input::RemapKeyForLayout(k);
            Input::_keyboard->HandleKeyRelease(k, static_cast<KeyModifiers>(modifiers));
            break;
        }
        default:
            break;
    }
}

void Graphitech::InternalEvents::MouseButtonCallback(GLFWwindow *internalWindow, int32_t button, int32_t action,
                                                     int32_t modifiers)
{
    double xPosition = 0.0;
    double yPosition = 0.0;
    glfwGetCursorPos(internalWindow, &xPosition, &yPosition);
    // Convert window coordinates to framebuffer coordinates to account for HiDPI scaling
    int fbW = 0, fbH = 0;
    int winW = 0, winH = 0;
    glfwGetFramebufferSize(internalWindow, &fbW, &fbH);
    glfwGetWindowSize(internalWindow, &winW, &winH);
    if (winW > 0 && winH > 0) {
        double sx = static_cast<double>(fbW) / static_cast<double>(winW);
        double sy = static_cast<double>(fbH) / static_cast<double>(winH);
        if (sx != 1.0 || sy != 1.0) {
            std::ostringstream oss; oss << "MouseButtonCallback: window=("<< winW <<"," << winH <<") fb=("<< fbW<<","<<fbH<<") raw=("<< xPosition <<","<< yPosition <<") sx="<< sx <<" sy="<< sy; DEBUG_INFO(oss.str()); }
        xPosition *= sx; yPosition *= sy;
    }

    auto mouseButton = static_cast<MouseButton>(button + 1);
    if (mouseButton == MouseButton::Right && modifiers & GLFW_MOD_ALT) { mouseButton = MouseButton::Middle; }

    switch (action) {
        case GLFW_PRESS:
            Input::_mouse->HandleButtonPress(static_cast<MouseButton>(mouseButton), static_cast<float>(xPosition),
                                             static_cast<float>(yPosition));
            break;
        case GLFW_RELEASE:
            Input::_mouse->HandleButtonRelease(static_cast<MouseButton>(mouseButton), static_cast<float>(xPosition),
                                               static_cast<float>(yPosition));
            break;
        default:
            break;
    }
}

void Graphitech::InternalEvents::MouseMotionCallback(GLFWwindow *internalWindow, double xPosition, double yPosition)
{
    // Convert to framebuffer coords similarly for motion events
    { int fbW = 0, fbH = 0; int winW = 0, winH = 0; glfwGetFramebufferSize(internalWindow, &fbW, &fbH); glfwGetWindowSize(internalWindow, &winW, &winH); if (winW > 0 && winH > 0) { double sx = static_cast<double>(fbW) / static_cast<double>(winW); double sy = static_cast<double>(fbH) / static_cast<double>(winH); if (sx != 1.0 || sy != 1.0) { std::ostringstream oss; oss << "MouseMotionCallback: window=("<< winW <<"," << winH <<") fb=("<< fbW<<","<<fbH<<") raw=("<< xPosition <<","<< yPosition <<") sx="<< sx <<" sy="<< sy; DEBUG_INFO(oss.str()); } xPosition *= sx; yPosition *= sy; } }
    Input::_mouse->HandleMotion(static_cast<float>(xPosition), static_cast<float>(yPosition));
}

void Graphitech::InternalEvents::MouseScrollCallback(GLFWwindow *internalWindow, double xScroll, double yScroll)
{
    double xPosition = 0.0;
    double yPosition = 0.0;
    glfwGetCursorPos(internalWindow, &xPosition, &yPosition);
    int fbW = 0, fbH = 0; int winW = 0, winH = 0; glfwGetFramebufferSize(internalWindow, &fbW, &fbH); glfwGetWindowSize(internalWindow, &winW, &winH); if (winW > 0 && winH > 0) { double sx = static_cast<double>(fbW) / static_cast<double>(winW); double sy = static_cast<double>(fbH) / static_cast<double>(winH); if (sx != 1.0 || sy != 1.0) { std::ostringstream oss; oss << "MouseScrollCallback: window=("<< winW <<"," << winH <<") fb=("<< fbW<<","<<fbH<<") raw=("<< xPosition <<","<< yPosition <<") sx="<< sx <<" sy="<< sy; DEBUG_INFO(oss.str()); } xPosition *= sx; yPosition *= sy; }

    Input::_mouse->HandleScroll(static_cast<float>(xScroll), static_cast<float>(yScroll), static_cast<float>(xPosition),
                                static_cast<float>(yPosition));
}
