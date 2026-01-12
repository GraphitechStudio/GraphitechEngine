#ifndef UITEXTINPUT_HPP
#define UITEXTINPUT_HPP

/**
 * @file UITextInput.hpp
 * @brief Text input widget
 */


#include "UIWidget.hpp"
#include "UIRenderer.hpp"
#include <functional>

namespace Graphitech {
namespace UI {

class UITextInput : public UIWidget {
public:
    UITextInput(float x, float y, float width, float height, const std::string& placeholder = "")
        : UIWidget(x, y, width, height),
          text_(""),
          placeholder_(placeholder),
          focused_(false),
          cursorPosition_(0),
          backgroundColor_(Color::Gray(0.15f)),
          textColor_(Color::White()),
          placeholderColor_(Color::Gray(0.5f)) {}

    void Render() override;
    bool HandleMouseDown(float x, float y, int button) override;
    bool HandleKeyPress(int key, int mods) override;
    bool HandleTextInput(unsigned int codepoint) override;

    void SetText(const std::string& text);
    std::string GetText() const { return text_; }
    
    void SetPlaceholder(const std::string& placeholder) { placeholder_ = placeholder; }
    void SetCallback(std::function<void(const std::string&)> callback) { onChange_ = callback; }
    
    bool IsFocused() const { return focused_; }
    void SetFocused(bool focused);

private:
    std::string text_;
    std::string placeholder_;
    bool focused_;
    size_t cursorPosition_;
    
    Color backgroundColor_;
    Color textColor_;
    Color placeholderColor_;
    
    std::function<void(const std::string&)> onChange_;
};

} // namespace UI
} // namespace Graphitech

#endif // UITEXTINPUT_HPP
