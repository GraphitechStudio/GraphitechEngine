#ifndef UIWIDGET_HPP
#define UIWIDGET_HPP

/**
 * @file UIWidget.hpp
 * @brief Base class for all UI widgets
 */


#include <glm/glm.hpp>
#include <string>
#include <functional>
#include <memory>

namespace Graphitech {
namespace UI {

/**
 * @brief Base class for all UI widgets
 */
class UIWidget {
public:
    UIWidget(float x, float y, float width, float height)
        : position_(x, y), size_(width, height), visible_(true), enabled_(true), 
          hovered_(false), pressed_(false), id_(nextId_++) {}

    virtual ~UIWidget() = default;

    // Core widget methods
    virtual void Render() = 0;
    virtual void Update(float /*deltaTime*/) {}
    virtual bool HandleMouseMove(float x, float y);
    virtual bool HandleMouseDown(float x, float y, int button);
    virtual bool HandleMouseUp(float x, float y, int button);
    virtual bool HandleMouseScroll(float /*xoffset*/, float /*yoffset*/) { return false; }
    virtual bool HandleKeyPress(int /*key*/, int /*mods*/) { return false; }
    virtual bool HandleTextInput(unsigned int /*codepoint*/) { return false; }

    /**
     * @brief Whether this widget should capture mouse events (move/up) when it
     * receives a mouse-down. By default widgets do not capture; sliders do.
     */
    virtual bool WantsCaptureOnDown() const { return false; }

    // Position and size
    glm::vec2 GetPosition() const { return position_; }
    glm::vec2 GetSize() const { return size_; }
    void SetPosition(float x, float y) { position_ = glm::vec2(x, y); }
    void SetSize(float width, float height) { size_ = glm::vec2(width, height); }

    // Visibility and enabled state
    virtual bool IsVisible() const { return visible_; }
    void SetVisible(bool visible) { visible_ = visible; }
    bool IsEnabled() const { return enabled_; }
    void SetEnabled(bool enabled) { enabled_ = enabled; }

    // State
    bool IsHovered() const { return hovered_; }
    bool IsPressed() const { return pressed_; }
    
    // ID
    uint32_t GetId() const { return id_; }

    // Check if point is inside widget
    bool Contains(float x, float y) const;

protected:
    glm::vec2 position_;
    glm::vec2 size_;
    bool visible_;
    bool enabled_;
    bool hovered_;
    bool pressed_;
    uint32_t id_;

private:
    static uint32_t nextId_;
};

} // namespace UI
} // namespace Graphitech

#endif // UIWIDGET_HPP
