#ifndef UISLIDER_HPP
#define UISLIDER_HPP

/**
 * @file UISlider.hpp
 * @brief Slider widget
 */


#include "UIWidget.hpp"
#include "UIRenderer.hpp"
#include <functional>

namespace Graphitech {
namespace UI {

class UISlider : public UIWidget {
public:
    UISlider(float x, float y, float width, float height, float minValue = 0.0f, float maxValue = 1.0f)
        : UIWidget(x, y, width, height), 
          minValue_(minValue), maxValue_(maxValue), value_(minValue),
          dragging_(false),
          trackColor_(Color::Gray(0.3f)),
          thumbColor_(Color::Gray(0.6f)),
          thumbHoverColor_(Color::Gray(0.7f)) {}

    void Render() override;
    bool HandleMouseMove(float x, float y) override;
    bool HandleMouseDown(float x, float y, int button) override;
    bool HandleMouseUp(float x, float y, int button) override;

    void SetValue(float value);
    float GetValue() const { return value_; }
    
    void SetRange(float minValue, float maxValue);
    void SetCallback(std::function<void(float)> callback) { onChange_ = callback; }

    // Set callback invoked when dragging starts
    void SetOnStart(std::function<void(float)> cb) { onStart_ = cb; }
    // Set callback invoked when dragging finishes (mouse release)
    void SetOnRelease(std::function<void(float)> cb) { onRelease_ = cb; }

    // Slider captures mouse on down so it receives subsequent move/up events even
    // if the cursor leaves the panel bounds.
    bool WantsCaptureOnDown() const override { return true; }

private:
    float minValue_;
    float maxValue_;
    float value_;
    bool dragging_;
    
    Color trackColor_;
    Color thumbColor_;
    Color thumbHoverColor_;
    
    std::function<void(float)> onChange_;
    std::function<void(float)> onStart_;
    std::function<void(float)> onRelease_;
    
    float GetThumbPosition() const;
    void UpdateValueFromPosition(float mouseX);
};

} // namespace UI
} // namespace Graphitech

#endif // UISLIDER_HPP
