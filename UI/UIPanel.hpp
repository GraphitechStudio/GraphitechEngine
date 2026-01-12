#ifndef UIPANEL_HPP
#define UIPANEL_HPP

/**
 * @file UIPanel.hpp
 * @brief Panel widget (container for other widgets)
 */


#include "UIWidget.hpp"
#include "UIRenderer.hpp"
#include <vector>
#include <memory>

namespace Graphitech {
namespace UI {

class UIPanel : public UIWidget {
public:
    UIPanel(float x, float y, float width, float height)
        : UIWidget(x, y, width, height),
          backgroundColor_(Color::Gray(0.2f)),
          borderColor_(Color::Gray(0.5f)),
          scrollOffsetY_(0.0f),
          maxScrollY_(0.0f) {}

    void Render() override;
    void Update(float deltaTime) override;
    bool HandleMouseMove(float x, float y) override;
    bool HandleMouseDown(float x, float y, int button) override;
    bool HandleMouseUp(float x, float y, int button) override;
    bool HandleMouseScroll(float xoffset, float yoffset) override;

    void AddChild(std::shared_ptr<UIWidget> widget);
    void RemoveChild(std::shared_ptr<UIWidget> widget);
    void ClearChildren();
    
    void SetBackgroundColor(const Color& color) { backgroundColor_ = color; }
    void SetBorderColor(const Color& color) { borderColor_ = color; }
    
    const std::vector<std::shared_ptr<UIWidget>>& GetChildren() const { return children_; }
    
    void SetScrollable(bool scrollable) { scrollable_ = scrollable; }
    float GetScrollOffset() const { return scrollOffsetY_; }

private:
    void UpdateScrollBounds();
    
    std::vector<std::shared_ptr<UIWidget>> children_;
    Color backgroundColor_;
    Color borderColor_;
    
    bool scrollable_ = false;
    float scrollOffsetY_;
    float maxScrollY_;

    // Captured child during mouse drag. When non-null, mouse move/up events should
    // be forwarded to this child even if the cursor leaves the panel bounds.
    UIWidget* capturedChild_ = nullptr;
};

} // namespace UI
} // namespace Graphitech

#endif // UIPANEL_HPP
