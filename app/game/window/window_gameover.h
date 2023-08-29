#ifndef WINDOW_GAMEOVER_H
#define WINDOW_GAMEOVER_H

#include "file_option.h"
#include "file_record.h"
#include "widget_textbox.h"
#include "window/widget_bottom_menu.h"
#include "window/widget_window.h"

class WindowGameOver : public WidgetWindow {
public:
  WindowGameOver(kebb::boxsize screen_size, std::shared_ptr<kebb::WindowName> next_window,
                 std::shared_ptr<Renderer> renderer, std::shared_ptr<RecordFile> records,
                 std::shared_ptr<OptionFile> options);
  virtual ~WindowGameOver() override;

  virtual void control_escape() override;
  virtual void control_enter() override;

  virtual void logic() override;
  virtual void render() const override;

private:
  std::shared_ptr<RecordFile> _records;
  std::shared_ptr<OptionFile> _options;

  std::unique_ptr<WidgetTextBox> _textbox_title;
  std::unique_ptr<WidgetTextBox> _textbox_mode;
  std::unique_ptr<WidgetTextBox> _textbox_difficulty;

  std::unique_ptr<WidgetTextBox> _textbox_time;
  std::unique_ptr<WidgetTextBox> _textbox_success;
  std::unique_ptr<WidgetTextBox> _textbox_fail;
  std::unique_ptr<WidgetTextBox> _textbox_miss;

  std::unique_ptr<WidgetBottomMenu> _widget_menu;

  SDL_Rect _separation_0;
  SDL_Rect _separation_1;
  SDL_Color _separation_color;
};

#endif // !WINDOW_GAMEOVER_H
