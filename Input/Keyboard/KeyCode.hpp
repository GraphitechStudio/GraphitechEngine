/*
** EPITECH PROJECT, 2025
** GraphitechEngine
** File description:
** KeyCode.hpp
*/

#ifndef KEYCODE_HPP
#define KEYCODE_HPP

#include <Graphitech.hpp>

namespace Graphitech {

    /**
     * @brief Enumeration of all supported keyboard key codes.
     *
     * @details KeyCode defines constants for all keyboard keys supported by
     * the engine. The values correspond to GLFW key codes for cross-platform
     * compatibility. Includes printable keys, function keys, navigation keys,
     * and numpad keys.
     */
    enum class KeyCode {
        Unknown = GLFW_KEY_UNKNOWN,  /**< Unknown/invalid key */

        // Printable Keys
        Space = GLFW_KEY_SPACE,           /**< Space bar */
        Apostrophe = GLFW_KEY_APOSTROPHE, /**< ' key */
        Comma = GLFW_KEY_COMMA,           /**< , key */
        Minus = GLFW_KEY_MINUS,           /**< - key */
        Period = GLFW_KEY_PERIOD,         /**< . key */
        Slash = GLFW_KEY_SLASH,           /**< / key */
        Key0 = GLFW_KEY_0,                /**< 0 key */
        Key1 = GLFW_KEY_1,                /**< 1 key */
        Key2 = GLFW_KEY_2,                /**< 2 key */
        Key3 = GLFW_KEY_3,                /**< 3 key */
        Key4 = GLFW_KEY_4,                /**< 4 key */
        Key5 = GLFW_KEY_5,                /**< 5 key */
        Key6 = GLFW_KEY_6,                /**< 6 key */
        Key7 = GLFW_KEY_7,                /**< 7 key */
        Key8 = GLFW_KEY_8,                /**< 8 key */
        Key9 = GLFW_KEY_9,                /**< 9 key */
        Semicolon = GLFW_KEY_SEMICOLON,   /**< ; key */
        Equal = GLFW_KEY_EQUAL,           /**< = key */
        A = GLFW_KEY_A,                   /**< A key */
        B = GLFW_KEY_B,                   /**< B key */
        C = GLFW_KEY_C,                   /**< C key */
        D = GLFW_KEY_D,                   /**< D key */
        E = GLFW_KEY_E,                   /**< E key */
        F = GLFW_KEY_F,                   /**< F key */
        G = GLFW_KEY_G,                   /**< G key */
        H = GLFW_KEY_H,                   /**< H key */
        I = GLFW_KEY_I,                   /**< I key */
        J = GLFW_KEY_J,                   /**< J key */
        K = GLFW_KEY_K,                   /**< K key */
        L = GLFW_KEY_L,                   /**< L key */
        M = GLFW_KEY_M,                   /**< M key */
        N = GLFW_KEY_N,                   /**< N key */
        O = GLFW_KEY_O,                   /**< O key */
        P = GLFW_KEY_P,                   /**< P key */
        Q = GLFW_KEY_Q,                   /**< Q key */
        R = GLFW_KEY_R,                   /**< R key */
        S = GLFW_KEY_S,                   /**< S key */
        T = GLFW_KEY_T,                   /**< T key */
        U = GLFW_KEY_U,                   /**< U key */
        V = GLFW_KEY_V,                   /**< V key */
        W = GLFW_KEY_W,                   /**< W key */
        X = GLFW_KEY_X,                   /**< X key */
        Y = GLFW_KEY_Y,                   /**< Y key */
        Z = GLFW_KEY_Z,                   /**< Z key */
        LeftBracket = GLFW_KEY_LEFT_BRACKET,   /**< [ key */
        Backslash = GLFW_KEY_BACKSLASH,        /**< \ key */
        RightBracket = GLFW_KEY_RIGHT_BRACKET, /**< ] key */
        GraveAccent = GLFW_KEY_GRAVE_ACCENT,   /**< ` key */

        // Function Keys
        Escape = GLFW_KEY_ESCAPE,        /**< Escape key */
        Return = GLFW_KEY_ENTER,         /**< Enter/Return key */
        Tab = GLFW_KEY_TAB,              /**< Tab key */
        Backspace = GLFW_KEY_BACKSPACE,  /**< Backspace key */
        Insert = GLFW_KEY_INSERT,        /**< Insert key */
        Delete = GLFW_KEY_DELETE,        /**< Delete key */
        Right = GLFW_KEY_RIGHT,          /**< Right arrow key */
        Left = GLFW_KEY_LEFT,            /**< Left arrow key */
        Down = GLFW_KEY_DOWN,            /**< Down arrow key */
        Up = GLFW_KEY_UP,                /**< Up arrow key */
        PageUp = GLFW_KEY_PAGE_UP,       /**< Page Up key */
        PageDown = GLFW_KEY_PAGE_DOWN,   /**< Page Down key */
        Home = GLFW_KEY_HOME,            /**< Home key */
        End = GLFW_KEY_END,              /**< End key */
        CapsLock = GLFW_KEY_CAPS_LOCK,   /**< Caps Lock key */
        ScrollLock = GLFW_KEY_SCROLL_LOCK, /**< Scroll Lock key */
        NumLock = GLFW_KEY_NUM_LOCK,     /**< Num Lock key */
        PrintScreen = GLFW_KEY_PRINT_SCREEN, /**< Print Screen key */
        Pause = GLFW_KEY_PAUSE,          /**< Pause key */
        F1 = GLFW_KEY_F1,                /**< F1 function key */
        F2 = GLFW_KEY_F2,                /**< F2 function key */
        F3 = GLFW_KEY_F3,                /**< F3 function key */
        F4 = GLFW_KEY_F4,                /**< F4 function key */
        F5 = GLFW_KEY_F5,                /**< F5 function key */
        F6 = GLFW_KEY_F6,                /**< F6 function key */
        F7 = GLFW_KEY_F7,                /**< F7 function key */
        F8 = GLFW_KEY_F8,                /**< F8 function key */
        F9 = GLFW_KEY_F9,                /**< F9 function key */
        F10 = GLFW_KEY_F10,              /**< F10 function key */
        F11 = GLFW_KEY_F11,              /**< F11 function key */
        F12 = GLFW_KEY_F12,              /**< F12 function key */
        F13 = GLFW_KEY_F13,              /**< F13 function key */
        F14 = GLFW_KEY_F14,              /**< F14 function key */
        F15 = GLFW_KEY_F15,              /**< F15 function key */
        F16 = GLFW_KEY_F16,              /**< F16 function key */
        F17 = GLFW_KEY_F17,              /**< F17 function key */
        F18 = GLFW_KEY_F18,              /**< F18 function key */
        F19 = GLFW_KEY_F19,              /**< F19 function key */
        F20 = GLFW_KEY_F20,              /**< F20 function key */
        F21 = GLFW_KEY_F21,              /**< F21 function key */
        F22 = GLFW_KEY_F22,              /**< F22 function key */
        F23 = GLFW_KEY_F23,              /**< F23 function key */
        F24 = GLFW_KEY_F24,              /**< F24 function key */
        F25 = GLFW_KEY_F25,              /**< F25 function key */
        Num0 = GLFW_KEY_KP_0,            /**< Numpad 0 */
        Num1 = GLFW_KEY_KP_1,            /**< Numpad 1 */
        Num2 = GLFW_KEY_KP_2,            /**< Numpad 2 */
        Num3 = GLFW_KEY_KP_3,            /**< Numpad 3 */
        Num4 = GLFW_KEY_KP_4,            /**< Numpad 4 */
        Num5 = GLFW_KEY_KP_5,            /**< Numpad 5 */
        Num6 = GLFW_KEY_KP_6,            /**< Numpad 6 */
        Num7 = GLFW_KEY_KP_7,            /**< Numpad 7 */
        Num8 = GLFW_KEY_KP_8,            /**< Numpad 8 */
        Num9 = GLFW_KEY_KP_9,            /**< Numpad 9 */
        Decimal = GLFW_KEY_KP_DECIMAL,   /**< Numpad decimal point */
        Divide = GLFW_KEY_KP_DIVIDE,     /**< Numpad divide */
        Multiply = GLFW_KEY_KP_MULTIPLY, /**< Numpad multiply */
        Subtract = GLFW_KEY_KP_SUBTRACT, /**< Numpad subtract */
        Add = GLFW_KEY_KP_ADD,           /**< Numpad add */
        Enter = GLFW_KEY_KP_ENTER,       /**< Numpad enter */
        Equals = GLFW_KEY_KP_EQUAL,      /**< Numpad equals */
        LeftShift = GLFW_KEY_LEFT_SHIFT,     /**< Left Shift key */
        LeftControl = GLFW_KEY_LEFT_CONTROL, /**< Left Control key */
        LeftAlt = GLFW_KEY_LEFT_ALT,         /**< Left Alt key */
        LeftSuper = GLFW_KEY_LEFT_SUPER,     /**< Left Super/Windows key */
        RightShift = GLFW_KEY_RIGHT_SHIFT,   /**< Right Shift key */
        RightControl = GLFW_KEY_RIGHT_CONTROL, /**< Right Control key */
        RightAlt = GLFW_KEY_RIGHT_ALT,       /**< Right Alt key */
        RightSuper = GLFW_KEY_RIGHT_SUPER,   /**< Right Super/Windows key */
        Menu = GLFW_KEY_MENU,                 /**< Menu key */

        Count = 119  /**< Total number of key codes */
    };

    /**
     * @brief Enumeration of keyboard modifier key flags.
     *
     * @details KeyModifiers defines bit flags for modifier keys that can be
     * combined using bitwise operations. These flags indicate which modifier
     * keys were active during keyboard events.
     */
    enum class KeyModifiers {
        None = 0x0000,        /**< No modifiers */
        Shift = GLFW_MOD_SHIFT,     /**< Shift key */
        Control = GLFW_MOD_CONTROL, /**< Control key */
        Alt = GLFW_MOD_ALT,         /**< Alt key */
        Super = GLFW_MOD_SUPER,     /**< Super/Windows key */
        CapsLock = GLFW_MOD_CAPS_LOCK, /**< Caps Lock active */
        NumLock = GLFW_MOD_NUM_LOCK,   /**< Num Lock active */
    };

    ENUM_FLAG_OPERATORS(KeyModifiers);
}

#endif //KEYCODE_HPP
