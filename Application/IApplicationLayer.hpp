/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** ApplicationLayer.hpp
*/

/**
 * @file `include/Core/Application/IApplicationLayer.hpp`
 * @brief Interface for application layers (lifecycle hooks).
 * @ingroup Application
 *
 * Provides the lifecycle hooks for application layers: start, enable, disable,
 * show, hide, update, render and release.
 *
 * Example usage: @example examples/ApplicationLayerExample.cpp
 */
#ifndef IAPPLICATIONLAYER_HPP
#define IAPPLICATIONLAYER_HPP


namespace Graphitech {
    /**
     * @class IApplicationLayer
     * @brief Abstract interface for an application layer.
     * @ingroup Application
     *
     * Implement this interface to create layers that the application host can
     * manage through their lifecycle (start, enable, update, render, release).
     */
    class IApplicationLayer
    {
        public:
            /**
             * @brief Virtual default destructor.
             */
            virtual ~IApplicationLayer() = default;

        protected:
            /**
             * @brief Called once when the layer is started.
             *
             * Perform initialization here.
             */
            virtual void OnStart() = 0;
            /**
             * @brief Called when the layer is enabled (made active).
             */
            virtual void OnEnable() = 0;
            /**
             * @brief Called when the layer is disabled (made inactive).
             */
            virtual void OnDisable() = 0;
            /**
             * @brief Called when the layer should become visible.
             */
            virtual void OnShow() = 0;
            /**
             * @brief Called when the layer should be hidden.
             */
            virtual void OnHide() = 0;
            /**
             * @brief Called every frame to update layer state.
             * @param deltaTime Time in seconds since last update.
             */
            virtual void OnUpdate(float deltaTime) = 0;
            /**
             * @brief Called every frame to perform rendering for the layer.
             */
            virtual void OnRender() = 0;
            /**
             * @brief Called when the layer is being released / destroyed.
             *
             * Clean up resources here.
             */
            virtual void OnRelease() = 0;
    };
}


#endif //APPLICATIONLAYER_HPP