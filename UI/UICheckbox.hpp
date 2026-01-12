#ifndef UICHECKBOX_HPP
#define UICHECKBOX_HPP

/**
 * @file UICheckbox.hpp
 * @brief Checkbox widget
 */


#include "UIWidget.hpp"
#include "UIRenderer.hpp"
#include <functional>

namespace Graphitech {
namespace UI {

class UICheckbox : public UIWidget {
public:
    UICheckbox(float x, float y, float size, const std::string& label = "")
        : UIWidget(x, y, size, size),
          checked_(false),
          label_(label),
          backgroundColor_(Color::Gray(0.3f)),
          checkColor_(Color::Green()) {}

    void Render() override;
    bool HandleMouseUp(float x, float y, int button) override;

    void SetChecked(bool checked);
    bool IsChecked() const { return checked_; }
    
    void SetLabel(const std::string& label) { label_ = label; }
    void SetCallback(std::function<void(bool)> callback) { onChange_ = callback; }

private:
    bool checked_;
    std::string label_;
    Color backgroundColor_;
    Color checkColor_;
    
    std::function<void(bool)> onChange_;
};

} // namespace UI
} // namespace Graphitech

#endif // UICHECKBOX_HPP
