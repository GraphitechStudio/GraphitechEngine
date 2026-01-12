/*
** EPITECH PROJECT, 2025
** RType
** File description:
** DLLoader.hpp
*/

#ifndef DLLOADER_HPP
    #define DLLOADER_HPP

#include <iostream>
#include <dlfcn.h>

/**
 * @class DLLoader
 * @brief Template class for loading and accessing symbols from dynamic libraries
 *
 * DLLoader provides a type-safe interface for dynamically loading shared libraries
 * (.so files on Unix systems) and retrieving function pointers or data symbols from them.
 * This is commonly used for plugin systems, allowing runtime extension of application
 * functionality without recompilation.
 *
 * The class uses the dlopen/dlsym/dlclose family of functions and is designed to work
 * on Unix-like systems (Linux, macOS, etc.).
 *
 * @tparam T The type of symbol to retrieve (function pointer type or data type)
 *
 * @note This class is Unix-specific and will not work on Windows systems.
 * @warning Error handling is done via stderr output. Consider using exceptions for better error handling in production code.
 */
template<typename T>
class DLLoader {
    public:
        /**
         * @brief Construct a DLLoader and load the specified library
         * @param path Path to the shared library file (.so)
         *
         * Attempts to load the dynamic library at the given path using dlopen with RTLD_LAZY.
         * If loading fails, an error message is printed to stderr but the object remains
         * in a valid (but unusable) state.
         */
        DLLoader(const char *path) {
            this->_handle = dlopen(path, RTLD_LAZY);
            if (!this->_handle) {
                std::cerr << "Failed to load library: " << dlerror() << std::endl;
            }
        }

        /**
         * @brief Destructor that automatically closes the library
         *
         * Ensures proper cleanup by calling close() to unload the library
         * and free associated resources.
         */
        ~DLLoader() {
            this->close();
        }

        /**
         * @brief Manually close and unload the library
         *
         * Closes the library handle if it was successfully opened.
         * After calling this method, the loader becomes unusable until
         * a new library is loaded.
         *
         * @note Safe to call multiple times - subsequent calls are no-ops.
         */
        void close() {
            if (this->_handle) {
                dlclose(this->_handle);
                this->_handle = nullptr;
            }
        }

        /**
         * @brief Get a factory function that creates objects of type T
         * @param name Name of the symbol (function) to retrieve
         * @return Pointer to newly created object, or nullptr on failure
         *
         * Retrieves a function pointer from the loaded library that takes no parameters
         * and returns a pointer to T. The function is then called to create and return
         * a new instance of T.
         *
         * This method is typically used for factory functions that create objects
         * (e.g., "create_component" -> Component* (*)()).
         *
         * @note The returned pointer ownership depends on the factory function implementation.
         * @warning The symbol must be a function with signature T* (*)()
         */
        T *getEntry(const std::string &name) {
            if (!this->_handle) {
                std::cerr << "Error: Library handle is null" << std::endl;
                return nullptr;
            }

            auto create = reinterpret_cast<T *(*)()> (dlsym(this->_handle, name.c_str()));
            if (!create) {
                std::cerr << "Failed to get symbol: " << dlerror() << std::endl;
                return nullptr;
            }

            return create();
        }

        /**
         * @brief Get a symbol address directly from the library
         * @param name Name of the symbol to retrieve
         * @return The symbol value, or nullptr on failure
         *
         * Retrieves the address of a symbol (function or data) from the loaded library.
         * Unlike getEntry(), this method returns the symbol address directly without calling it.
         *
         * This method is useful for getting function pointers for extern "C" functions
         * or accessing global data exported by the library.
         *
         * @note For function pointers, T should be the appropriate function pointer type.
         * @note For data symbols, T should be the data type.
         */
        T getSymbol(const std::string &name) {
            if (!this->_handle) {
                std::cerr << "Error: Library handle is null" << std::endl;
                return nullptr;
            }

            dlerror(); // Clear previous errors
            T sym = reinterpret_cast<T>(dlsym(this->_handle, name.c_str()));
            const char* err = dlerror();
            if (err) {
                std::cerr << "Failed to get symbol '" << name << "': " << err << std::endl;
                return nullptr;
            }
            return sym;
        }

        /**
         * @brief Check if the library was loaded successfully
         * @return true if library is loaded, false otherwise
         *
         * Provides a convenient way to check if the DLLoader has a valid library handle.
         * This operator allows the DLLoader to be used in boolean contexts.
         */
        explicit operator bool() const noexcept {
            return _handle != nullptr;
        }

    protected:
    private:
        /**
         * @brief Handle to the loaded dynamic library
         *
         * Internal handle returned by dlopen(). nullptr if loading failed.
         */
        void *_handle;
};

#endif