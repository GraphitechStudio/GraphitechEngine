/**
 * @file UITextInput.cpp
 * @brief Implementation of text input widget
 */

#include "UITextInput.hpp"
#include "UIContext.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>

namespace Graphitech {
namespace UI {

void UITextInput::Render() {
    if (!visible_) return;
    
    auto* renderer = UIContext::GetRenderer();
    if (!renderer) return;
    
    // Draw background
    renderer->DrawRect(position_.x, position_.y, size_.x, size_.y, backgroundColor_, true);
    
    // Draw border (highlighted if focused)
    Color borderColor = focused_ ? Color::Blue() : Color::Gray(0.5f);
    renderer->DrawRect(position_.x, position_.y, size_.x, size_.y, borderColor, false);
    
    // Dynamic font size based on input height
    float fontSize = std::min(size_.y * 0.4f, 20.0f);
    fontSize = std::max(fontSize, 10.0f);
    
    // Draw text or placeholder
    float textX = position_.x + 5.0f;
    float textY = position_.y + size_.y / 2.0f - fontSize / 2.0f;
    
    if (text_.empty() && !placeholder_.empty() && !focused_) {
        renderer->DrawText(placeholder_, textX, textY, fontSize, placeholderColor_);
    } else if (!text_.empty()) {
        renderer->DrawText(text_, textX, textY, fontSize, textColor_);
        
        // Draw cursor if focused
        if (focused_) {
            float cursorX = textX + cursorPosition_ * (fontSize * 0.6f);
            renderer->DrawLine(cursorX, position_.y + 5.0f, cursorX, position_.y + size_.y - 5.0f, 2.0f, textColor_);
        }
    }
}

bool UITextInput::HandleMouseDown(float x, float y, int /*button*/) {
    if (!visible_ || !enabled_) return false;
    
    if (Contains(x, y)) {
        SetFocused(true);
        return true;
    } else {
        SetFocused(false);
    }
    
    return false;
}

bool UITextInput::HandleKeyPress(int key, int /*mods*/) {
    if (!visible_ || !enabled_ || !focused_) return false;
    
    if (key == GLFW_KEY_BACKSPACE && cursorPosition_ > 0) {
        text_.erase(cursorPosition_ - 1, 1);
        cursorPosition_--;
        
        if (onChange_) {
            onChange_(text_);
        }
        return true;
    }
    else if (key == GLFW_KEY_DELETE && cursorPosition_ < text_.length()) {
        text_.erase(cursorPosition_, 1);
        
        if (onChange_) {
            onChange_(text_);
        }
        return true;
    }
    else if (key == GLFW_KEY_LEFT && cursorPosition_ > 0) {
        cursorPosition_--;
        return true;
    }
    else if (key == GLFW_KEY_RIGHT && cursorPosition_ < text_.length()) {
        cursorPosition_++;
        return true;
    }
    else if (key == GLFW_KEY_HOME) {
        cursorPosition_ = 0;
        return true;
    }
    else if (key == GLFW_KEY_END) {
        cursorPosition_ = text_.length();
        return true;
    }
    
    return false;
}

bool UITextInput::HandleTextInput(unsigned int codepoint) {
    if (!visible_ || !enabled_ || !focused_) return false;
    
    // Only accept printable ASCII characters for now
    if (codepoint >= 32 && codepoint < 127) {
        text_.insert(cursorPosition_, 1, (char)codepoint);
        cursorPosition_++;
        
        if (onChange_) {
            onChange_(text_);
        }
        return true;
    }
    
    return false;
}

void UITextInput::SetText(const std::string& text) {
    text_ = text;
    cursorPosition_ = text_.length();
    
    if (onChange_) {
        onChange_(text_);
    }
}

void UITextInput::SetFocused(bool focused) {
    focused_ = focused;
    if (focused) {
        cursorPosition_ = text_.length();
    }
}

} // namespace UI
} // namespace Graphitech
