#ifndef UICONTEXT_HPP
#define UICONTEXT_HPP

/**
 * @file UIContext.hpp
 * @brief Global context for UI system
 */


#include "UIRenderer.hpp"
#include <memory>

namespace Graphitech {
namespace UI {

/**
 * @brief Global UI context singleton
 */
class UIContext {
public:
    static void Initialize(UIRenderer* renderer);
    static void Shutdown();
    
    static UIRenderer* GetRenderer() { return renderer_; }

    // Tooltip API
    static void ShowTooltip(const std::string& text, float x, float y);
    static void HideTooltip();
    static bool HasTooltip();
    static std::string GetTooltipText();
    static glm::vec2 GetTooltipPos();

    // Mouse position (set by Graphitech on mouse move)
    static void SetMousePosition(float x, float y);
    static glm::vec2 GetMousePosition();

private:
    static UIRenderer* renderer_;
    static std::string tooltipText_;
    static glm::vec2 tooltipPos_;
    static bool tooltipVisible_;
    static glm::vec2 mousePos_;
};

} // namespace UI
} // namespace Graphitech

#endif // UICONTEXT_HPP
