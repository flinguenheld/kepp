#ifndef WINDOW_WELCOME_TIMER_H
#define WINDOW_WELCOME_TIMER_H

#include "option/option_file.h"
#include "utils.h"
#include "widget/button/widget_boolean.h"
#include "widget/button/widget_list.h"
#include "widget/button/widget_selection.h"
#include "widget/widget_menu.h"
#include "widget/widget_window.h"
#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>

class WindowWelcomeTimer : public WidgetWindowSelection {
public:
  WindowWelcomeTimer(kebb::boxsize screen_size, std::shared_ptr<kebb::WindowName> next_window,
                     std::shared_ptr<Renderer> renderer, std::shared_ptr<OptionFile> options);
  virtual ~WindowWelcomeTimer() override;

  virtual void control_escape() override;
  virtual void control_enter() override;

  virtual void render() const override;

private:
  std::unique_ptr<WidgetTextBox> _widget_title;
  std::unique_ptr<WidgetMenu> _widget_menu;
  std::shared_ptr<OptionFile> _options;
};
#endif // !WINDOW_WELCOME_TIMER_H
