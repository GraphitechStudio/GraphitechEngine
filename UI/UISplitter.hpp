#ifndef UISPLITTER_HPP
#define UISPLITTER_HPP

/**
 * @file UISplitter.hpp
 * @brief Splitter widget for resizable panels
 */


#include "UIWidget.hpp"
#include "UIRenderer.hpp"
#include <functional>

namespace Graphitech {
namespace UI {

enum class SplitterOrientation {
    Horizontal,
    Vertical
};

class UISplitter : public UIWidget {
public:
    UISplitter(float x, float y, float size, SplitterOrientation orientation)
        : UIWidget(x, y, 
                   orientation == SplitterOrientation::Vertical ? size : 0, 
                   orientation == SplitterOrientation::Horizontal ? size : 0),
          orientation_(orientation),
          dragging_(false),
          splitterSize_(size),
          backgroundColor_(Color::Gray(0.4f)),
          hoverColor_(Color::Gray(0.6f)) {}

    void Render() override;
    bool HandleMouseMove(float x, float y) override;
    bool HandleMouseDown(float x, float y, int button) override;
    bool HandleMouseUp(float x, float y, int button) override;

    void SetCallback(std::function<void(float)> callback) { onDrag_ = callback; }
    void SetOrientation(SplitterOrientation orientation) { orientation_ = orientation; }
    void SetLength(float length);
    
    float GetSplitterSize() const { return splitterSize_; }

private:
    SplitterOrientation orientation_;
    bool dragging_;
    float splitterSize_;
    Color backgroundColor_;
    Color hoverColor_;
    
    std::function<void(float)> onDrag_;
};

} // namespace UI
} // namespace Graphitech

#endif // UISPLITTER_HPP
