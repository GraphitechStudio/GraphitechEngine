#ifndef UICONTEXTMENU_HPP
#define UICONTEXTMENU_HPP


#include "UIWidget.hpp"
#include "UIRenderer.hpp"
#include "UI/UIContext.hpp"
#include <sstream>
#include <vector>
#include <functional>
#include <string>

namespace Graphitech {
namespace UI {

class UIContextMenu : public UIWidget {
public:
    UIContextMenu();

    void Render() override;
    bool HandleMouseDown(float x, float y, int button) override;
    bool HandleMouseMove(float x, float y) override;

    void SetItems(const std::vector<std::pair<std::string, std::function<void()>>>& items) { items_ = items; }
    void Show(float x, float y);
    void Hide();
    bool IsVisible() const { return visible_; }

    // optional callback for external systems (ecs) to subscribe to selection by index
    std::function<void(int)> onSelectIndex;

private:
    std::vector<std::pair<std::string, std::function<void()>>> items_;
    bool visible_ = false;
    float width_ = 200.0f;
    float itemHeight_ = 44.0f;
    float fontSize_ = 14.0f;
};

} // namespace UI
} // namespace Graphitech

#endif // UICONTEXTMENU_HPP
