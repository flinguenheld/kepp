#ifndef WINDOW_OPTION_H
#define WINDOW_OPTION_H

#include "option/option_file.h"
#include "utils.h"
#include "widget/button/widget_boolean.h"
#include "widget/button/widget_list.h"
#include "widget/button/widget_selection.h"
#include "widget/widget_menu.h"
#include "widget/widget_window.h"
#include <algorithm>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

class WindowOption : public WidgetWindowSelection {
public:
  WindowOption(boxsize screen_size, std::shared_ptr<WindowName> next_window,
               std::shared_ptr<Renderer> renderer, std::shared_ptr<OptionFile> options);
  virtual ~WindowOption();

  virtual void control_escape();
  virtual void control_enter();
  virtual void control_left();
  virtual void control_right();

  virtual void render();

private:
  std::unique_ptr<WidgetTextBox> _widget_title;
  std::unique_ptr<WidgetTextBox> _widget_message;
  std::unique_ptr<WidgetMenu> _widget_menu;
  std::shared_ptr<OptionFile> _options;

  // Help message
  void display_message(std::string &&message);
  void check_new_resolution();
  bool _message_displayed;

  const boxsize _screen_size;
};
#endif // !WINDOW_OPTION_H
