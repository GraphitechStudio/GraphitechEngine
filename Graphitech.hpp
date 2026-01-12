/**
 * @file Graphitech.hpp
 * @brief Core utilities, macros and common includes used across the project.
 * @defgroup CoreUtilities Core Utilities
 * Utilities and small helpers used across the engine (macros, common typedefs).
 * @{
 *
 * This header provides small, frequently used utilities:
 * - SAFE_RELEASE: safe delete + set to nullptr
 * - FROM_DATA: pass an expression plus its byte size
 * - gId_t: global identifier type
 * - ENUM_FLAG_OPERATORS: enable bitwise operators for enum types
 *
 * The examples below show typical usage patterns.
 */

#ifndef GRAPHITECH_HPP
#define GRAPHITECH_HPP

#include <cstdint>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <future>
#include <algorithm>
#include <functional>

#if __has_include(<glad/glad.h>)
#include <glad/glad.h>
#define GRAPHITECH_HAS_GLAD 1
#else
#warning "GLAD not found - compile without OpenGL loader; GL functions may be unavailable"
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Safely delete a pointer and set it to nullptr.
 *
 * Usage notes:
 * - Only use with pointers allocated by `new` (not with `new[]`).
 * - The macro checks for null before deleting and guarantees the pointer is set to `nullptr` afterwards.
 *
 * @param RESOURCE variable name of the pointer to release.
 *
 * @code{.cpp}
 * Foo *p = new Foo();
 * SAFE_RELEASE(p); // p is deleted and set to nullptr
 * // SAFE_RELEASE(nullptr) is safe and does nothing
 * @endcode
 */
#define SAFE_RELEASE(RESOURCE) if ((RESOURCE) != nullptr) { delete (RESOURCE); (RESOURCE) = nullptr; }

/**
 * @brief Passes an expression and its size in bytes.
 *
 * The macro expands to `(DATA), sizeof((DATA))` and is useful for APIs that expect a pointer/data
 * together with the length in bytes. Note that if DATA is a pointer, `sizeof` gives the pointer size
 * (not the buffer length).
 *
 * @param DATA expression (array, object or pointer).
 *
 * @warning Do NOT use this macro to infer the length of dynamically allocated buffers.
 *
 * @code{.cpp}
 * char buffer[] = "hello"; // size == 6 including '\0'
 * sendBuffer(FROM_DATA(buffer)); // expands to buffer, sizeof(buffer)
 *
 * char *p = buffer;
 * sendBuffer(FROM_DATA(p)); // expands to p, sizeof(p) -> sizeof(char*) (not the string length)
 * @endcode
 */
#define FROM_DATA(DATA) (DATA), sizeof((DATA))

/**
 * @brief Global identifier type used across the project.
 *
 * Use `gId_t` for objects that require a stable 64-bit id.
 *
 * @code{.cpp}
 * gId_t userId = 0x1234567890abcdefULL;
 * std::unordered_map<gId_t, std::string> names;
 * @endcode
 */
typedef uint64_t gId_t;

/**
 * @brief Enable bitwise operators for an enum type `T`.
 *
 * Adds inline overloads for `~`, `|`, `&`, `^` and the compound-assignment forms `|=`, `&=`, `^=`.
 * Each operator converts the enum value to its underlying integral type, performs the bit operation,
 * and casts the result back to the enum type.
 *
 * Requirements:
 * - `T` must be an enumeration with an integral underlying type (for example `enum class Flags : uint32_t`).
 *
 * Notes:
 * - Operators are `inline` to avoid multiple-definition linker errors when included in multiple translation units.
 * - Prefer `std::underlying_type_t<T>` conversions rather than integer hacks.
 *
 * @par Example
 * @code{.cpp}
 * enum class KeyModifiers : uint32_t {
 *     None  = 0,
 *     Shift = 1 << 0,
 *     Ctrl  = 1 << 1,
 *     Alt   = 1 << 2
 * };
 *
 * ENUM_FLAG_OPERATORS(KeyModifiers);
 *
 * // Combine flags
 * KeyModifiers m = KeyModifiers::Shift | KeyModifiers::Ctrl;
 *
 * // Test flags
 * if ((m & KeyModifiers::Ctrl) == KeyModifiers::Ctrl) {
 *     // ctrl is pressed
 * }
 *
 * // Toggle flags
 * m ^= KeyModifiers::Shift; // remove shift if it was set
 *
 * // Compound assignment
 * m |= KeyModifiers::Alt;
 * @endcode
 */
#define ENUM_FLAG_OPERATORS(T) \
    inline T operator~ (T a) \
        { using U = std::underlying_type_t<T>; return static_cast<T>( ~static_cast<U>(a) ); } \
    inline T operator| (T a, T b) \
        { using U = std::underlying_type_t<T>; return static_cast<T>( static_cast<U>(a) | static_cast<U>(b) ); } \
    inline T operator& (T a, T b) \
        { using U = std::underlying_type_t<T>; return static_cast<T>( static_cast<U>(a) & static_cast<U>(b) ); } \
    inline T operator^ (T a, T b) \
        { using U = std::underlying_type_t<T>; return static_cast<T>( static_cast<U>(a) ^ static_cast<U>(b) ); } \
    inline T& operator|= (T& a, T b) \
        { using U = std::underlying_type_t<T>; a = static_cast<T>( static_cast<U>(a) | static_cast<U>(b) ); return a; } \
    inline T& operator&= (T& a, T b) \
        { using U = std::underlying_type_t<T>; a = static_cast<T>( static_cast<U>(a) & static_cast<U>(b) ); return a; } \
    inline T& operator^= (T& a, T b) \
        { using U = std::underlying_type_t<T>; a = static_cast<T>( static_cast<U>(a) ^ static_cast<U>(b) ); return a; }

#endif // GRAPHITECH_HPP

/** @} */
