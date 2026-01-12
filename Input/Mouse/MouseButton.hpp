/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** MouseButton.hpp
*/

#ifndef MOUSEBUTTON_HPP
#define MOUSEBUTTON_HPP

namespace Graphitech {

    /**
     * @brief Enumeration of mouse button identifiers.
     *
     * @details MouseButton defines constants for all mouse buttons
     * supported by the engine, including standard buttons and
     * extended buttons found on some mice.
     */
    enum class MouseButton {
        Unknown,  /**< Unknown/invalid button */
        Left,     /**< Left mouse button */
        Right,    /**< Right mouse button */
        Middle,   /**< Middle mouse button (wheel) */
        X1,       /**< First extended button */
        X2,       /**< Second extended button */

        Count     /**< Total number of mouse buttons */
    };
}

#endif //MOUSEBUTTON_HPP