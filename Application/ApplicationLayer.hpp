/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** ApplicationLayer.hpp
*/
#ifndef APPLICATIONLAYER_HPP
#define APPLICATIONLAYER_HPP

/**
 * @file ApplicationLayer.hpp
 * @brief Declaration of the ApplicationLayer class which represents an application layer.
 *
 * This header defines the configuration descriptor and the base ApplicationLayer class
 * used by the engine to manage layer lifecycle: enable/disable, one-time start,
 * per-frame update/render and resource release.
 */

#include <Application/IApplicationLayer.hpp>

namespace Graphitech {
    /**
     * @brief Configuration descriptor for an ApplicationLayer.
     *
     * Provides initial flags for a layer instance (enabled and visible).<
     */
    struct ApplicationLayerDescriptor final
    {
        /** @brief Whether the layer is enabled by default. */
        bool _enabled = true;
        /** @brief Whether the layer is visible by default. */
        bool _visible = true;
    };

    /**
     * @ingroup Core
     * @brief Base class representing an application layer.
     *
     * Inherits from
     * IApplicationLayer and provides the standard lifecycle operations (Enable/Disable/Show/Hide/Release).
     * Derived classes should override the protected On* callbacks to implement custom behavior.
     */
    class ApplicationLayer : public IApplicationLayer
    {
        public:
            /**
             * @brief Construct an ApplicationLayer.
             * @param desc Descriptor that initializes the layer flags.
             */
            explicit ApplicationLayer(const ApplicationLayerDescriptor &desc = {}) noexcept;

            /**
             * @brief Enable the layer (final).
             *
             * Calls internal callbacks as required. This method is final and should not be overridden.
             */
            virtual void Enable() final;

            /**
             * @brief Disable the layer (final).
             */
            virtual void Disable() final;

            /**
             * @brief Make the layer visible (final).
             */
            virtual void Show() final;

            /**
             * @brief Hide the layer (final).
             */
            virtual void Hide() final;

            /**
             * @brief Release the layer and its resources (final).
             */
            virtual void Release() final;

            /**
             * @brief Return the current descriptor (read-only).
             * @return const ApplicationLayerDescriptor& constant reference to the descriptor.
             */
            virtual const ApplicationLayerDescriptor& GetDescriptor() const noexcept final;

        protected:
            /**
             * @name Protected callbacks
             * Methods called by the implementation. Override these in derived classes
             * to implement initialization, per-frame update and rendering logic.
             */
            void OnStart() override;
            void OnEnable() override;
            void OnDisable() override;
            void OnShow() override;
            void OnHide() override;
            void OnUpdate(float deltaTime) override;
            void OnRender() override;
            void OnRelease() override;

        private:
            /**
             * @brief Start the layer (final) — called by the engine.
             */
            virtual void Start() final;

            /**
             * @brief Final update called by the engine (final).
             * @param deltaTime Time elapsed.
             */
            virtual void Update(float deltaTime) final;

            /**
             * @brief Final render called by the engine (final).
             */
            virtual void Render() final;

            /** @brief Descriptor for this layer. */
            ApplicationLayerDescriptor _desc;

            friend class Engine;
    };
}


#endif //APPLICATIONLAYER_HPP