#include "UIContextMenu.hpp"
#include "BitmapFont.hpp"
#include "UI/UIContext.hpp"
#include <algorithm>
#include <sstream>

namespace Graphitech {
namespace UI {

UIContextMenu::UIContextMenu()
    : UIWidget(0, 0, 0, 0), visible_(false) {
}

void UIContextMenu::Show(float x, float y) {
    SetPosition(x, y);
    visible_ = true;
}

void UIContextMenu::Hide() {
    visible_ = false;
}

bool UIContextMenu::HandleMouseMove(float x, float y) {
    if (!visible_) return false;
    return true;
}

bool UIContextMenu::HandleMouseDown(float x, float y, int button) {
    if (!visible_ || button != 0) return false;
    float mx = x;
    float my = y;
    float menuX = GetPosition().x;
    float menuY = GetPosition().y;
    size_t count = items_.size();
    float height = itemHeight_ * (float)count;

    if (!(mx >= menuX && mx <= menuX + width_ && my >= menuY && my <= menuY + height)) {
        Hide();
        return false;
    }

    size_t idx = (size_t)((my - menuY) / itemHeight_);
    if (idx < items_.size()) {
        // take a local copy of callback to avoid problems if it hides the menu
        auto cb = items_[idx].second;
        Hide();
        if (onSelectIndex) onSelectIndex((int)idx);
        if (cb) cb();
        return true;
    }

    Hide();
    return false;
}

void UIContextMenu::Render() {
    if (!visible_) return;
    float x = GetPosition().x;
    float y = GetPosition().y;
    size_t count = items_.size();
    float height = itemHeight_ * (float)count;

    // Background
    if (auto r = UIContext::GetRenderer()) r->DrawRoundedRect(x, y, width_, height, 6.0f, Color(0.12f, 0.12f, 0.18f, 1.0f), true);

    // Draw items using BitmapFont wrapping logic similar to buttons/context usage
    float fontSize = fontSize_;
    float scale = fontSize / (float)BitmapFont::FONT_CHAR_HEIGHT;
    float textLineHeight = (float)BitmapFont::GetTextHeight(scale);

    for (size_t i = 0; i < items_.size(); ++i) {
        float itemY = y + i * itemHeight_;
        if (i > 0) if (auto r = UIContext::GetRenderer()) r->DrawRect(x, itemY - 1, width_, 1, Color(0.08f, 0.08f, 0.1f, 1.0f), true);

        // Wrap text into up to 2 lines with ellipsis when needed
        float maxWidth = width_ - 16.0f;
        std::vector<std::string> lines;
        std::string text = items_[i].first;
        std::string remaining = text;
        for (int line = 0; line < 2 && !remaining.empty(); ++line) {
            // try to take as many words as possible
            size_t take = 0;
            std::string piece;
            std::istringstream iss(remaining);
            std::string word;
            std::string test;
            std::vector<std::string> words;
            while (iss >> word) words.push_back(word);

            if (words.empty()) break;

            for (size_t w = 0; w < words.size(); ++w) {
                test = piece.empty() ? words[w] : piece + " " + words[w];
                if ((float)BitmapFont::GetTextWidth(test, scale) <= maxWidth) {
                    piece = test;
                    take = w + 1;
                } else {
                    // If nothing fits and word is long, take characters
                    if (piece.empty()) {
                        std::string last = words[w];
                        while (!last.empty() && (float)BitmapFont::GetTextWidth(last + "...", scale) > maxWidth) last.pop_back();
                        if (last.empty()) last = "...";
                        piece = last + (w + 1 < words.size() ? "" : "");
                        take = w + 1;
                    }
                    break;
                }
            }

            if (take == 0) break;

            lines.push_back(piece);

            // shorten remaining
            size_t pos = 0;
            for (size_t k = 0; k < take; ++k) {
                // find next space
                size_t p = remaining.find(' ');
                if (p == std::string::npos) { remaining = ""; break; }
                remaining = remaining.substr(p + 1);
            }

            if (line == 1 && !remaining.empty()) {
                // ellipsize last line
                std::string last = lines.back();
                while (!last.empty() && (float)BitmapFont::GetTextWidth(last + "...", scale) > maxWidth) last.pop_back();
                lines.back() = last + "...";
            }
        }

        float totalTextHeight = (float)lines.size() * textLineHeight + (float)(lines.size() - 1) * 2.0f;
        float startY = itemY + (itemHeight_ - totalTextHeight) / 2.0f;
        for (size_t j = 0; j < lines.size(); ++j) {
            float textX = x + 8.0f;
            float textY = startY + j * (textLineHeight + 2.0f);
            if (auto r = UIContext::GetRenderer()) r->DrawText(lines[j], textX, textY, fontSize, Color::White());
        }
    }
}

} // namespace UI
} // namespace Graphitech