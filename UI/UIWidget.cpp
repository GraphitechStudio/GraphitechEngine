/**
 * @file UIWidget.cpp
 * @brief Implementation of UIWidget base class
 */

#include "UIWidget.hpp"

namespace Graphitech {
namespace UI {

uint32_t UIWidget::nextId_ = 1;

bool UIWidget::Contains(float x, float y) const {
    return x >= position_.x && x <= position_.x + size_.x &&
           y >= position_.y && y <= position_.y + size_.y;
}

bool UIWidget::HandleMouseMove(float x, float y) {
    if (!visible_ || !enabled_) return false;
    
    bool wasHovered = hovered_;
    hovered_ = Contains(x, y);
    
    return hovered_ || wasHovered;
}

bool UIWidget::HandleMouseDown(float x, float y, int button) {
    if (!visible_ || !enabled_) return false;
    
    if (Contains(x, y)) {
        pressed_ = true;
        return true;
    }
    
    return false;
}

bool UIWidget::HandleMouseUp(float x, float y, int button) {
    if (!visible_ || !enabled_) return false;
    
    bool wasPressed = pressed_;
    pressed_ = false;
    
    return wasPressed && Contains(x, y);
}

} // namespace UI
} // namespace Graphitech
