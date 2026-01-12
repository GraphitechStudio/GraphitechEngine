/**
 * @file UISplitter.cpp
 * @brief Implementation of splitter widget
 */

#include "UISplitter.hpp"
#include "UIContext.hpp"

namespace Graphitech {
namespace UI {

void UISplitter::Render() {
    if (!visible_) return;
    
    auto* renderer = UIContext::GetRenderer();
    if (!renderer) return;
    
    Color color = (hovered_ || dragging_) ? hoverColor_ : backgroundColor_;
    
    if (orientation_ == SplitterOrientation::Vertical) {
        // Vertical splitter (horizontal line)
        renderer->DrawRect(position_.x, position_.y, size_.x, splitterSize_, color, true);
    } else {
        // Horizontal splitter (vertical line)
        renderer->DrawRect(position_.x, position_.y, splitterSize_, size_.y, color, true);
    }
}

bool UISplitter::HandleMouseMove(float x, float y) {
    if (!visible_ || !enabled_) return false;
    
    bool wasHovered = hovered_;
    hovered_ = Contains(x, y);
    
    if (dragging_ && onDrag_) {
        if (orientation_ == SplitterOrientation::Vertical) {
            onDrag_(y);
        } else {
            onDrag_(x);
        }
        return true;
    }
    
    return hovered_ || wasHovered;
}

bool UISplitter::HandleMouseDown(float x, float y, int /*button*/) {
    if (!visible_ || !enabled_) return false;
    
    if (Contains(x, y)) {
        dragging_ = true;
        pressed_ = true;
        return true;
    }
    
    return false;
}

bool UISplitter::HandleMouseUp(float /*x*/, float /*y*/, int /*button*/) {
    if (!visible_ || !enabled_) return false;
    
    bool wasDragging = dragging_;
    dragging_ = false;
    pressed_ = false;
    
    return wasDragging;
}

void UISplitter::SetLength(float length) {
    if (orientation_ == SplitterOrientation::Vertical) {
        size_.x = length;
        size_.y = splitterSize_;
    } else {
        size_.x = splitterSize_;
        size_.y = length;
    }
}

} // namespace UI
} // namespace Graphitech
