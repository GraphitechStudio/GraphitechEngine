#ifndef UIBUTTON_HPP
#define UIBUTTON_HPP

/**
 * @file UIButton.hpp
 * @brief Button widget
 */


#include "UIWidget.hpp"
#include "UIRenderer.hpp"
#include <functional>
#include <chrono>

namespace Graphitech {
namespace UI {

class UIButton : public UIWidget {
public:
    UIButton(float x, float y, float width, float height, const std::string& text = "")
        : UIWidget(x, y, width, height), text_(text), 
          backgroundColor_(Color::Gray(0.3f)),
          hoverColor_(Color::Gray(0.4f)),
          pressedColor_(Color::Gray(0.2f)),
          textColor_(Color::White()) {}

    void Render() override;
    bool HandleMouseUp(float x, float y, int button) override;

    void SetText(const std::string& text) { text_ = text; }
    std::string GetText() const { return text_; }
    
    void SetCallback(std::function<void()> callback) { onClick_ = callback; }
    void SetDoubleClickCallback(std::function<void()> callback) { onDoubleClick_ = callback; }
    
    void SetBackgroundColor(const Color& color) { backgroundColor_ = color; }
    void SetTextColor(const Color& color) { textColor_ = color; }
    void SetIconTexture(const std::string& path) { iconTexturePath_ = path; }
    void SetPreviewTexture(unsigned int textureId) { previewTextureId_ = textureId; hasPreviewTexture_ = true; }

    // Tooltip support: set full tooltip text (shown on hover if present)
    void SetTooltipText(const std::string& tooltip) { tooltipText_ = tooltip; }
    std::string GetTooltipText() const { return tooltipText_; }

private:
    std::string text_;
    std::function<void()> onClick_;
    Color backgroundColor_;
    Color hoverColor_;
    Color pressedColor_;
    Color textColor_;
    std::string iconTexturePath_;
    unsigned int previewTextureId_ = 0;
    bool hasPreviewTexture_ = false;
    std::string tooltipText_;

    // Double click support
    std::function<void()> onDoubleClick_;
    std::chrono::steady_clock::time_point lastClickTime_ = std::chrono::steady_clock::time_point();
};

} // namespace UI
} // namespace Graphitech

#endif // UIBUTTON_HPP
