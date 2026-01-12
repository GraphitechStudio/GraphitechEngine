/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** Engine.hpp
*/

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Graphitech.hpp"

namespace Graphitech {

    class Window;
    class Renderer;
    class ApplicationLayer;
    class EventDispatcher;

    /**
     * @brief Main engine class that coordinates all game systems.
     *
     * @details The Engine class is a singleton-like static class that manages
     * the entire game lifecycle. It handles the main game loop, timing,
     * and provides global access to core systems. The engine is designed
     * to be initialized once and run until the application exits.
     *
     * @ingroup Core
     */
    class Engine final
    {
        public:
            /**
             * @brief Starts the main game loop.
             *
             * @details This method initializes all engine systems and enters
             * the main game loop. It takes a callback function that creates
             * and returns the application layer. The method will not return
             * until the application exits.
             *
             * @param initCallback Function that creates the application layer.
             * @return Exit code (0 for success, non-zero for errors).
             */
            static int32_t Run(const std::function<ApplicationLayer *()> &initCallback);

            /**
             * @brief Gets the elapsed time since the last frame in seconds.
             *
             * @details Returns the delta time between the current and previous
             * frame, which is used for frame-rate independent updates.
             *
             * @return Elapsed time in seconds as a float.
             */
            static float GetElapsedSeconds();

            /** @brief Global window instance. */
            inline static Window* _window = nullptr;
            /** @brief Global renderer instance. */
            // inline static Renderer* _renderer = nullptr; // Moved to game layer
            /** @brief Global application layer instance. */
            inline static ApplicationLayer* _applicationLayer = nullptr;
            /** @brief Global event dispatcher instance. */
            inline static EventDispatcher* _eventDispatcher = nullptr;

        private:
            /**
             * @brief Updates all engine systems for the current frame.
             *
             * @details Called once per frame to update timing, handle events,
             * and update the application layer.
             */
            static void Update();

            /**
             * @brief Releases all engine resources.
             *
             * @details Cleans up all allocated resources and shuts down
             * engine systems. Called when the application exits.
             */
            static void Release();

            /** @brief Timestamp of the previous frame for delta time calculation. */
            inline static std::chrono::steady_clock::time_point _prevTime;
    };
}


#endif //ENGINE_HPP