/**
 * @file UICheckbox.cpp
 * @brief Implementation of checkbox widget
 */

#include "UICheckbox.hpp"
#include "UIContext.hpp"

namespace Graphitech {
namespace UI {

void UICheckbox::Render() {
    if (!visible_) return;
    
    auto* renderer = UIContext::GetRenderer();
    if (!renderer) return;
    
    // Draw checkbox background
    renderer->DrawRect(position_.x, position_.y, size_.x, size_.y, backgroundColor_, true);
    
    // Draw border
    Color borderColor = hovered_ ? Color::White() : Color::Gray(0.5f);
    renderer->DrawRect(position_.x, position_.y, size_.x, size_.y, borderColor, false);
    
    // Draw checkmark if checked
    if (checked_) {
        float padding = size_.x * 0.2f;
        renderer->DrawLine(
            position_.x + padding, 
            position_.y + size_.y / 2.0f,
            position_.x + size_.x / 2.0f, 
            position_.y + size_.y - padding,
            2.0f, 
            checkColor_
        );
        renderer->DrawLine(
            position_.x + size_.x / 2.0f, 
            position_.y + size_.y - padding,
            position_.x + size_.x - padding, 
            position_.y + padding,
            2.0f, 
            checkColor_
        );
    }
    
    // Draw label
    if (!label_.empty()) {
        float labelX = position_.x + size_.x + 10.0f;
        float labelY = position_.y + size_.y / 2.0f - 10.0f;
        renderer->DrawText(label_, labelX, labelY, 20.0f, Color::White());
    }
}

bool UICheckbox::HandleMouseUp(float x, float y, int button) {
    bool result = UIWidget::HandleMouseUp(x, y, button);
    
    if (result) {
        SetChecked(!checked_);
    }
    
    return result;
}

void UICheckbox::SetChecked(bool checked) {
    checked_ = checked;
    
    if (onChange_) {
        onChange_(checked_);
    }
}

} // namespace UI
} // namespace Graphitech
