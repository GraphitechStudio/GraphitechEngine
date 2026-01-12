/**
 * @file UIContext.cpp
 * @brief Implementation of UI context
 */

#include "UIContext.hpp"

namespace Graphitech {
namespace UI {

UIRenderer* UIContext::renderer_ = nullptr;
std::string UIContext::tooltipText_ = "";
glm::vec2 UIContext::tooltipPos_ = glm::vec2(0.0f, 0.0f);
bool UIContext::tooltipVisible_ = false;
glm::vec2 UIContext::mousePos_ = glm::vec2(0.0f, 0.0f);

void UIContext::Initialize(UIRenderer* renderer) {
    renderer_ = renderer;
}

void UIContext::Shutdown() {
    renderer_ = nullptr;
}

void UIContext::ShowTooltip(const std::string& text, float x, float y) {
    tooltipText_ = text;
    tooltipPos_ = glm::vec2(x, y);
    tooltipVisible_ = true;
}

void UIContext::HideTooltip() {
    tooltipVisible_ = false;
    tooltipText_.clear();
}

bool UIContext::HasTooltip() {
    return tooltipVisible_ && !tooltipText_.empty();
}

std::string UIContext::GetTooltipText() {
    return tooltipText_;
}

glm::vec2 UIContext::GetTooltipPos() {
    return tooltipPos_;
}

void UIContext::SetMousePosition(float x, float y) {
    mousePos_ = glm::vec2(x, y);
}

glm::vec2 UIContext::GetMousePosition() {
    return mousePos_;
}

} // namespace UI
} // namespace Graphitech
