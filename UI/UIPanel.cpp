/**
 * @file UIPanel.cpp
 * @brief Implementation of panel widget
 */

#include "UIPanel.hpp"
#include "UIContext.hpp"
#include <algorithm>

namespace Graphitech {
namespace UI {

void UIPanel::Render() {
    if (!visible_) return;
    
    auto* renderer = UIContext::GetRenderer();
    if (!renderer) return;
    
    // Draw background
    renderer->DrawRect(position_.x, position_.y, size_.x, size_.y, backgroundColor_, true);
    
    // Set up clipping for children
    renderer->PushScissor(position_.x, position_.y, size_.x, size_.y);
    
    // Render children with parent offset and scroll offset
    for (auto& child : children_) {
        if (child && child->IsVisible()) {
            auto childPos = child->GetPosition();
            // Apply panel position so children are relative to the panel, then apply scrolling
            child->SetPosition(childPos.x + position_.x, childPos.y + position_.y - scrollOffsetY_);
            child->Render();
            // Restore original position
            child->SetPosition(childPos.x, childPos.y);
        }
    }
    
    renderer->PopScissor();

    // Draw a simple scrollbar when content is larger than visible area
    if (scrollable_ && maxScrollY_ > 0.0f) {
        float barWidth = 8.0f;
        float barX = position_.x + size_.x - barWidth - 6.0f;
        float contentHeight = maxScrollY_ + size_.y;
        float thumbHeight = std::max(20.0f, size_.y * (size_.y / contentHeight));
        float thumbY = position_.y + (scrollOffsetY_ / maxScrollY_) * (size_.y - thumbHeight);

        // Track background
        renderer->DrawRect(barX - 2.0f, position_.y + 4.0f, barWidth + 4.0f, size_.y - 8.0f, Color(0.0f, 0.0f, 0.0f, 0.25f), true);
        // Thumb
        renderer->DrawRoundedRect(barX, thumbY, barWidth, thumbHeight, 4.0f, Color(0.8f, 0.8f, 0.8f, 0.8f), true);
    }
}

void UIPanel::Update(float deltaTime) {
    for (auto& child : children_) {
        if (child && child->IsVisible()) {
            child->Update(deltaTime);
        }
    }
    
    UpdateScrollBounds();
}

bool UIPanel::HandleMouseMove(float x, float y) {
    if (!visible_ || !enabled_) return false;

    bool handled = UIWidget::HandleMouseMove(x, y);

    // If a child has captured the mouse (drag in progress), forward events to it
    if (capturedChild_) {
        float adjustedX = x - position_.x;
        float adjustedY = y - position_.y + scrollOffsetY_;
        if (capturedChild_->HandleMouseMove(adjustedX, adjustedY)) handled = true;
        return handled;
    }

    if (Contains(x, y)) {
        // Adjust coordinates to be relative to panel position
        float adjustedX = x - position_.x;
        float adjustedY = y - position_.y + scrollOffsetY_;

        for (auto& child : children_) {
            if (child && child->IsVisible() && child->IsEnabled()) {
                if (child->HandleMouseMove(adjustedX, adjustedY)) {
                    handled = true;
                }
            }
        }
    }

    return handled;
}

bool UIPanel::HandleMouseDown(float x, float y, int button) {
    if (!visible_ || !enabled_) return false;

    if (Contains(x, y)) {
        // Adjust coordinates to be relative to panel position
        float adjustedX = x - position_.x;
        float adjustedY = y - position_.y + scrollOffsetY_;

        for (auto& child : children_) {
            if (child && child->IsVisible() && child->IsEnabled()) {
                if (child->HandleMouseDown(adjustedX, adjustedY, button)) {
                    // Capture this child for subsequent mouse moves/up only if it requests capture
                    if (child->WantsCaptureOnDown()) {
                        capturedChild_ = child.get();
                    }
                    return true;
                }
            }
        }

        return UIWidget::HandleMouseDown(x, y, button);
    }

    return false;
}

bool UIPanel::HandleMouseUp(float x, float y, int button) {
    if (!visible_ || !enabled_) return false;

    // If a child captured the mouse, forward up to it and release capture
    float adjustedX = x - position_.x;
    float adjustedY = y - position_.y + scrollOffsetY_;

    if (capturedChild_) {
        bool handled = capturedChild_->HandleMouseUp(adjustedX, adjustedY, button);
        capturedChild_ = nullptr;
        return handled;
    }

    for (auto& child : children_) {
        if (child && child->IsVisible() && child->IsEnabled()) {
            if (child->HandleMouseUp(adjustedX, adjustedY, button)) {
                return true;
            }
        }
    }

    return UIWidget::HandleMouseUp(x, y, button);
}

bool UIPanel::HandleMouseScroll(float /*xoffset*/, float yoffset) {
    if (!visible_ || !enabled_ || !scrollable_) return false;
    
    scrollOffsetY_ -= yoffset * 20.0f;
    scrollOffsetY_ = std::max(0.0f, std::min(scrollOffsetY_, maxScrollY_));
    
    return true;
}

void UIPanel::AddChild(std::shared_ptr<UIWidget> widget) {
    if (widget) {
        children_.push_back(widget);
        UpdateScrollBounds();
    }
}

void UIPanel::RemoveChild(std::shared_ptr<UIWidget> widget) {
    if (capturedChild_ == widget.get()) capturedChild_ = nullptr;
    children_.erase(
        std::remove(children_.begin(), children_.end(), widget),
        children_.end()
    );
    UpdateScrollBounds();
}

void UIPanel::ClearChildren() {
    children_.clear();
    scrollOffsetY_ = 0.0f;
    maxScrollY_ = 0.0f;
    capturedChild_ = nullptr;
}

void UIPanel::UpdateScrollBounds() {
    if (!scrollable_) return;
    
    float maxY = 0.0f;
    for (const auto& child : children_) {
        if (child && child->IsVisible()) {
            float childBottom = child->GetPosition().y + child->GetSize().y;
            maxY = std::max(maxY, childBottom);
        }
    }
    
    maxScrollY_ = std::max(0.0f, maxY - size_.y);
}

} // namespace UI
} // namespace Graphitech
