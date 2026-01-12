/**
 * @file UISlider.cpp
 * @brief Implementation of slider widget
 */

#include "UISlider.hpp"
#include "UIContext.hpp"
#include <algorithm>

namespace Graphitech {
namespace UI {

void UISlider::Render() {
    if (!visible_) return;
    
    auto* renderer = UIContext::GetRenderer();
    if (!renderer) return;
    
    // Draw track
    float trackHeight = size_.y * 0.3f;
    float trackY = position_.y + (size_.y - trackHeight) / 2.0f;
    renderer->DrawRect(position_.x, trackY, size_.x, trackHeight, trackColor_, true);
    
    // Draw thumb
    float thumbSize = size_.y;
    float thumbX = GetThumbPosition() - thumbSize / 2.0f;
    Color thumbCol = (hovered_ || dragging_) ? thumbHoverColor_ : thumbColor_;
    renderer->DrawRect(thumbX, position_.y, thumbSize, thumbSize, thumbCol, true);
    renderer->DrawRect(thumbX, position_.y, thumbSize, thumbSize, Color::White(), false);
}

bool UISlider::HandleMouseMove(float x, float y) {
    if (!visible_ || !enabled_) return false;
    
    UIWidget::HandleMouseMove(x, y);
    
    if (dragging_) {
        UpdateValueFromPosition(x);
        return true;
    }
    
    return hovered_;
}

bool UISlider::HandleMouseDown(float x, float y, int /*button*/) {
    if (!visible_ || !enabled_) return false;
    
    if (Contains(x, y)) {
        dragging_ = true;
        UpdateValueFromPosition(x);
        if (onStart_) onStart_(value_);
        return true;
    }
    
    return false;
}

bool UISlider::HandleMouseUp(float /*x*/, float /*y*/, int /*button*/) {
    if (!visible_ || !enabled_) return false;
    
    bool wasDragging = dragging_;
    dragging_ = false;
    
    if (wasDragging && onRelease_) onRelease_(value_);
    return wasDragging;
}

void UISlider::SetValue(float value) {
    float newVal = std::max(minValue_, std::min(value, maxValue_));

    // Only propagate change if it really differs to avoid triggering callbacks
    // during programmatic updates (which can cause unwanted undo commands).
    const float EPSILON = 0.001f;
    if (std::fabs(newVal - value_) > EPSILON) {
        value_ = newVal;
        if (onChange_) {
            onChange_(value_);
        }
    } else {
        value_ = newVal; // update silently
    }
}

void UISlider::SetRange(float minValue, float maxValue) {
    minValue_ = minValue;
    maxValue_ = maxValue;
    value_ = std::max(minValue_, std::min(value_, maxValue_));
}

float UISlider::GetThumbPosition() const {
    if (maxValue_ <= minValue_) return position_.x;
    
    float t = (value_ - minValue_) / (maxValue_ - minValue_);
    return position_.x + t * size_.x;
}

void UISlider::UpdateValueFromPosition(float mouseX) {
    float t = (mouseX - position_.x) / size_.x;
    t = std::max(0.0f, std::min(t, 1.0f));
    
    float newValue = minValue_ + t * (maxValue_ - minValue_);
    SetValue(newValue);
}

} // namespace UI
} // namespace Graphitech
