/*
** EPITECH PROJECT, 2025
** G_CPP_500_LYN_5_2_rtype_7
** File description:
** Debug.hpp
*/
#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "Graphitech.hpp"

/**
 * @file Debug.hpp
 * @brief Simple debugging / logging utilities for Graphitech.
 *
 * This header exposes a static Debug class used to enable/disable logging
 * and to write messages at different severity levels. It also exposes a
 * small set of convenience macros to tag module names and send logs.
 *
 */

namespace Graphitech
{
    /**
     * @ingroup Debugger
     * @brief Static helper providing logging functions.
     *
     * The Debug class is a small, static-only utility that centralises
     * logging for the engine. It writes messages to console and optionally
     * to a file. Logging can be enabled or disabled globally.
     *
     * Usage example:
     * @code{.cpp}
     * #include "Debugger/Debug.hpp"
     *
     * MOD("Engine"); // define a module name used by macros
     * Graphitech::Debug::Enable();
     * DEBUG_INFO("Starting engine");
     * DEBUG_WARN("A non-fatal issue");
     * Graphitech::Debug::Disable();
     * @endcode
     */
    class Debug final
    {
        public:
            /**
             * @brief Enable logging output.
             *
             * After calling Enable(), subsequent logging calls (Info/Warn/Crit/Todo)
             * will output to the configured destinations (console/file) unless the
             * write flags are disabled.
             */
            static void Enable();

            /**
             * @brief Disable logging output.
             *
             * This function is noexcept and will stop logging until Enable() is
             * called again.
             */
            static void Disable() noexcept;

            /**
             * @brief Log an informational message.
             *
             * @param msg The message to log.
             * @param mod Optional module name to tag the message. If empty, the
             *            currently defined module via MOD() macros may be used.
             */
            static void Info(const std::string &msg, const std::string &mod = "");

            /**
             * @brief Log a warning message.
             *
             * @param msg The message to log.
             * @param mod Optional module name.
             */
            static void Warn(const std::string &msg, const std::string &mod = "");

            /**
             * @brief Log a critical error message.
             *
             * @param msg The message to log.
             * @param mod Optional module name.
             */
            static void Crit(const std::string &msg, const std::string &mod = "");

            /**
             * @brief Log a TODO/debug reminder message.
             *
             * @param msg The message to log.
             * @param mod Optional module name.
             */
            static void Todo(const std::string &msg, const std::string &mod = "");

            /**
             * @brief Write a raw log message without level prefix.
             *
             * @param msg The raw message to append.
             */
            static void Log(const std::string& msg);

            /**
             * @brief Returns true if logging is enabled.
             *
             * This function is noexcept.
             * @return true if enabled, false otherwise
             */
            static bool IsEnabled() noexcept;

            /**
             * @brief Disable writing to console when true.
             *
             * Set to true to avoid console output (useful in unit tests).
             */
            inline static bool _disableWriteToConsole = false;

            /**
             * @brief Disable writing to file when true.
             */
            inline static bool _disableWriteToFile = false;

        private:
            /**
             * @brief Internal helper to write text to the configured outputs.
             *
             * @param text The final formatted text to output.
             */
            static void WriteText(const std::string &text);

            /**
             * @brief Timestamp prefix used when logging starts.
             */
            inline static std::string _enableStartTime = "";

            /**
             * @brief Internal enabled flag.
             */
            inline static bool _enabled = false;
    };
}

/**
 * @defgroup DebuggerMacros Debug helper macros
 * Convenience macros to tag module names and emit logs with the Debug class.
 * @{
 */

/**
 * @brief Define the current module name used by the DEBUG_* macros.
 *
 * Example:
 * @code{.cpp}
 * MOD("Renderer");
 * DEBUG_INFO("Initialized renderer");
 * @endcode
 */
#define MOD(NAME) static std::string modName = (NAME)

/**
 * @brief Log an informational message using the current module name.
 *
 * Expands to call Graphitech::Debug::Info(msg, modName).
 */
#define DEBUG_INFO(MSG) Graphitech::Debug::Info((MSG), modName)

/**
 * @brief Log a warning message using the current module name.
 */
#define DEBUG_WARN(MSG) Graphitech::Debug::Warn((MSG), modName)

/**
 * @brief Log a critical error using the current module name.
 */
#define DEBUG_CRIT(MSG) Graphitech::Debug::Crit((MSG), modName)

/**
 * @brief Log a TODO / reminder using the current module name.
 */
#define DEBUG_TODO(MSG) Graphitech::Debug::Todo((MSG), modName)

/** @} */ // endgroup DebuggerMacros

#endif //DEBUG_HPP
