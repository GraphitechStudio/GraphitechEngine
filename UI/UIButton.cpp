/**
 * @file UIButton.cpp
 * @brief Implementation of button widget
 */

#include "UIButton.hpp"
#include "UIContext.hpp"
#include "BitmapFont.hpp"
#include <algorithm>
#include <sstream>
#include <vector>
#include <chrono>

namespace Graphitech {
namespace UI {

void UIButton::Render() {
    if (!visible_) return;
    
    auto* renderer = UIContext::GetRenderer();
    if (!renderer) return;
    
    Color bgColor = backgroundColor_;
    if (pressed_) {
        bgColor = pressedColor_;
    } else if (hovered_) {
        bgColor = hoverColor_;
    }
    
    // Draw button background
    renderer->DrawRect(position_.x, position_.y, size_.x, size_.y, bgColor, true);
    
    // Draw preview texture if available (for texture files and folders)
    if (hasPreviewTexture_ && previewTextureId_ != 0) {
        // Draw texture at top area, above text
        float texSize = std::min(size_.x * 0.8f, size_.y * 0.55f);
        float texX = position_.x + (size_.x - texSize) / 2.0f;
        float texY = position_.y + 8.0f; // small top padding
        renderer->DrawTexture(previewTextureId_, texX, texY, texSize, texSize);
    }
    // Draw icon texture if available (for folders, etc.)
    else if (!iconTexturePath_.empty()) {
        // TODO: Load and cache texture, then draw it
        // For now, we'll implement a simple texture cache in MapGraphitech
    }
    
    // Draw text (centered at bottom) with wrapping and ellipsis
    if (!text_.empty()) {
        // Fixed font size for button labels
        float fontSize = 20.0f; // 20px fixed size for button labels
        float scale = fontSize / (float)BitmapFont::FONT_CHAR_HEIGHT;
        float lineHeight = BitmapFont::GetTextHeight(scale);

        // Compute max width available for text (90% of button width)
        float maxTextWidth = size_.x * 0.9f; // 90% of button width

        // Compute vertical space available for text (below any preview texture)
        float texBottom = 8.0f; // default top padding for text area
        if (hasPreviewTexture_ && previewTextureId_ != 0) {
            float texSize = std::min(size_.x * 0.8f, size_.y * 0.55f);
            texBottom = 8.0f + texSize; // texY + texSize
        }
        float availableHeight = size_.y - texBottom - 8.0f; // bottom padding

        int maxLines = std::max(1, int(availableHeight / (lineHeight + 2.0f)));
        maxLines = std::min(maxLines, 2); // limit to at most 2 lines for readability

        // Robust wrapping that allows breaking long words across lines (up to maxLines)
        auto wrapText = [&](const std::string &text, float maxWidth, int maxLinesAllowed) {
            std::vector<std::string> out;
            float s = scale;
            size_t idx = 0;
            size_t len = text.size();

            while (idx < len && (int)out.size() < maxLinesAllowed) {
                // skip leading spaces
                while (idx < len && text[idx] == ' ') idx++;
                if (idx >= len) break;

                // try to append as many words as possible to current line
                size_t start = idx;
                std::string cur;
                bool finishedLine = false;

                while (!finishedLine && idx < len) {
                    // find next word
                    size_t wordStart = idx;
                    while (idx < len && text[idx] != ' ') idx++;
                    std::string word = text.substr(wordStart, idx - wordStart);

                    std::string test = cur.empty() ? word : cur + " " + word;
                    if (BitmapFont::GetTextWidth(test, s) <= maxWidth) {
                        cur = test;
                        // continue to next word
                    } else {
                        if (cur.empty()) {
                            // single word doesn't fit on an empty line -> break it across chars
                            size_t take = 0;
                            while (wordStart + take < len) {
                                std::string piece = text.substr(wordStart, take + 1);
                                if (BitmapFont::GetTextWidth(piece, s) <= maxWidth) {
                                    take++;
                                } else break;
                            }
                            if (take == 0) {
                                // Can't place even a single char — fallback to ellipsis
                                out.push_back("...");
                                return out;
                            }
                            std::string part = text.substr(wordStart, take);
                            out.push_back(part);
                            // move idx to after the piece
                            idx = wordStart + take;
                            // if we reached limit, ellipsize last
                            if ((int)out.size() == maxLinesAllowed && idx < len) {
                                std::string &last = out.back();
                                while (!last.empty() && BitmapFont::GetTextWidth(last + "...", s) > maxWidth) last.pop_back();
                                last += "...";
                                return out;
                            }
                            finishedLine = true;
                        } else {
                            // push current and retry the same word on next line
                            out.push_back(cur);
                            // move idx back to start of the word to process it in next iteration
                            idx = wordStart;
                            // if we've hit limit, ellipsize last
                            if ((int)out.size() == maxLinesAllowed) {
                                std::string &last = out.back();
                                while (!last.empty() && BitmapFont::GetTextWidth(last + "...", s) > maxWidth) last.pop_back();
                                last += "...";
                                return out;
                            }
                            finishedLine = true;
                        }
                    }

                    // skip spaces to next word
                    while (idx < len && text[idx] == ' ') idx++;
                }

                if (!finishedLine) {
                    if (!cur.empty()) {
                        out.push_back(cur);
                        // if we hit limit and there is still text remaining, add ellipsis
                        if ((int)out.size() == maxLinesAllowed && idx < len) {
                            std::string &last = out.back();
                            while (!last.empty() && BitmapFont::GetTextWidth(last + "...", s) > maxWidth) last.pop_back();
                            last += "...";
                            return out;
                        }
                    }
                }
            }

            return out;
        };

        std::vector<std::string> lines = wrapText(text_, maxTextWidth, maxLines);

        // Compute starting Y so the block sits near the bottom with padding
        float totalHeight = (float)lines.size() * lineHeight + (float)(lines.size() - 1) * 2.0f;
        float textStartY = position_.y + size_.y - totalHeight - 8.0f;

        for (size_t i = 0; i < lines.size(); ++i) {
            float lineW = BitmapFont::GetTextWidth(lines[i], scale);
            float textX = position_.x + (size_.x - lineW) / 2.0f;
            float textY = textStartY + i * (lineHeight + 2.0f);
            renderer->DrawText(lines[i], textX, textY, fontSize, textColor_);
        }
    }

    // Tooltip handling: if widget has a tooltip text and is hovered, show it near the mouse
    if (!tooltipText_.empty()) {
        if (hovered_) {
            auto mp = UIContext::GetMousePosition();
            UIContext::ShowTooltip(tooltipText_, mp.x, mp.y);
        } else {
            UIContext::HideTooltip();
        }
    }
}

bool UIButton::HandleMouseUp(float x, float y, int button) {
    bool result = UIWidget::HandleMouseUp(x, y, button);
    
    if (result) {
        using clock = std::chrono::steady_clock;
        static const std::chrono::milliseconds DOUBLE_CLICK_THRESHOLD(300);
        auto now = clock::now();

        if (lastClickTime_.time_since_epoch().count() != 0 && (now - lastClickTime_) <= DOUBLE_CLICK_THRESHOLD) {
            // Double click
            lastClickTime_ = clock::time_point();
            if (onDoubleClick_) onDoubleClick_();
        } else {
            // Single click - record time and call single click callback
            lastClickTime_ = now;
            if (onClick_) onClick_();
        }
    }
    
    return result;
}

} // namespace UI
} // namespace Graphitech
