#include "window_option.h"

WindowOption::WindowOption(boxsize screen_size, std::shared_ptr<WindowName> next_window,
                           std::shared_ptr<Renderer> renderer, std::shared_ptr<OptionFile> options)
    : WidgetWindowSelection(next_window, renderer), _options(options), _message_displayed(false),
      _screen_size(screen_size) {

  _widget_menu = std::make_unique<WidgetMenu>(screen_size, renderer, "<ESC> Cancel     <ENTER> Save");

  // Geometry
  boxsize char_size = _renderer->font_char_size(FontName::F_Menu); // NOTE: Use font menu ?
  boxsize bs_title;
  point pt;

  // ------------------------------------------------------------------------
  // Title ------------------------------------------------------------------
  char_size.set_scale(3);
  bs_title.w = char_size.w * 4;
  bs_title.h = char_size.h;

  pt.x = screen_size.w / 2 - bs_title.w / 2;
  pt.y = bs_title.h * 0.05;

  _widget_title = std::make_unique<WidgetTextBox>(pt, bs_title);
  _widget_title->set_text("Options");

  // Catppuccin: Mocha
  _widget_title->set_color_text({250, 179, 135, 200}); // Peach
  // _widget_title->set_color_text({242, 205, 205, 200}); // Flamingo
  // _widget_title->set_color_text({180, 190, 254, 200}); // Lavender
  // _textbox_time->set_color_text({148, 226, 213, 200}); // Teal

  // ------------------------------------------------------------------------
  // Selection fields -------------------------------------------------------
  boxsize bs_field = renderer->font_char_size(FontName::F_Menu).scale(1);
  pt.x = screen_size.w / 2;
  pt.y += bs_title.h * 1.5;

  _widget_select_fields.emplace_back(std::make_unique<WidgetList>(
      pt, bs_field, "Resolution:",
      std::vector<SelectionItem>{{"480x480", "480-20"},
                                 {"640x640", "640-15"},
                                 {"800x800", "800-10"},
                                 {"1024x1024", "1024-5"}}, // TODO: Scales have to be validated
      true));
  _widget_select_fields.back()->set_choice_by_value(_options->get(OptionName::Resolution));

  pt.y += bs_field.h * 1;
  _widget_select_fields.emplace_back(std::make_unique<WidgetList>(
      pt, bs_field,
      "Keyboard layout:", std::vector<SelectionItem>{{"US", "US"}, {"Francais", "FR"}, {"Bepo", "Bepo"}}));
  _widget_select_fields.back()->set_choice_by_value(_options->get(OptionName::Layout));

  pt.y += bs_field.h * 1.5;
  _widget_select_fields.emplace_back(std::make_unique<WidgetList>(pt, bs_field, "Nb targets:", 1, 20, 1));
  _widget_select_fields.back()->set_choice_by_value(_options->get(OptionName::Targets));

  pt.y += bs_field.h * 1;
  _widget_select_fields.emplace_back(
      std::make_unique<WidgetList>(pt, bs_field, "Countdown:",
                                   std::vector<SelectionItem>{{"15s", "15"},
                                                              {"30s", "30"},
                                                              {"45s", "45"},
                                                              {"60s", "60"},
                                                              {"1m30", "90"},
                                                              {"2m", "120"},
                                                              {"2m30", "150"},
                                                              {"3m", "180"},
                                                              {"3m30", "210"},
                                                              {"4m", "240"},
                                                              {"4m30", "270"},
                                                              {"5m", "300"},
                                                              {"10m", "600"}}));
  _widget_select_fields.back()->set_choice_by_value(_options->get(OptionName::Countdown));

  pt.y += bs_field.h * 1;
  _widget_select_fields.emplace_back(std::make_unique<WidgetList>(pt, bs_field, "Speed:",
                                                                  std::vector<SelectionItem>{{"1", "20"},
                                                                                             {"2", "18"},
                                                                                             {"3", "16"},
                                                                                             {"4", "14"},
                                                                                             {"5", "12"},
                                                                                             {"6", "10"},
                                                                                             {"7", "8"},
                                                                                             {"8", "6"},
                                                                                             {"9", "4"},
                                                                                             {"10", "2"}}));
  _widget_select_fields.back()->set_choice_by_value(_options->get(OptionName::Speed));

  pt.y += bs_field.h * 1.5;
  _widget_select_fields.emplace_back(std::make_unique<WidgetBoolean>(pt, bs_field, "Letters"));
  _widget_select_fields.back()->set_bool(std::stoi(_options->get(OptionName::Letters)));

  pt.y += bs_field.h * 1;
  _widget_select_fields.emplace_back(std::make_unique<WidgetBoolean>(pt, bs_field, "Capitals"));
  _widget_select_fields.back()->set_bool(std::stoi(_options->get(OptionName::Capitals)));

  pt.y += bs_field.h * 1;
  _widget_select_fields.emplace_back(std::make_unique<WidgetBoolean>(pt, bs_field, "Numbers"));
  _widget_select_fields.back()->set_bool(std::stoi(_options->get(OptionName::Numbers)));

  pt.y += bs_field.h * 1;
  _widget_select_fields.emplace_back(std::make_unique<WidgetBoolean>(pt, bs_field, "Symbols"));
  _widget_select_fields.back()->set_bool(std::stoi(_options->get(OptionName::Symbols)));

  // ------------------------------------------------------------------------
  // Message ----------------------------------------------------------------
  pt.y += bs_field.h * 2;
  _widget_message = std::make_unique<WidgetTextBox>(pt, bs_field);
  _widget_message->set_color_text({30, 30, 46, 200}); // Base
  _widget_message->set_color({249, 226, 175, 200});   // Yellow
}

WindowOption::~WindowOption() {}

void WindowOption::render() {
  // Clear screen
  SDL_SetRenderDrawColor(_renderer->renderer(), 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(_renderer->renderer());

  _widget_title->render(_renderer->renderer(), _renderer->font(FontName::F_Menu));

  for (auto &w : _widget_select_fields)
    w->render(_renderer->renderer(), _renderer->font(FontName::F_Menu));

  _widget_menu->render();

  if (_message_displayed)
    _widget_message->render(_renderer->renderer(), _renderer->font(FontName::F_Menu));

  // Update Screen
  SDL_RenderPresent(_renderer->renderer());
}

// ----------------------------------------------------------------------------------------------------
// MESSAGE --------------------------------------------------------------------------------------------
/*
 * Resize the _Widget_message and set the text.
 */
void WindowOption::display_message(std::string &&message) {

  boxsize char_size = _renderer->font_char_size(FontName::F_Menu);
  char_size.set_scale(0.8);
  _widget_message->set_w(char_size.w * message.length());
  _widget_message->set_text(std::move(message));

  _widget_message->set_x(_screen_size.w / 2 - _widget_message->size().w / 2);
  _message_displayed = true;
}

void WindowOption::check_new_resolution() {

  if (_options->get(OptionName::Resolution) != _widget_select_fields[0]->get_choice().value) {
    display_message("  Restart the application to set the new resolution.  ");
    _message_displayed = true;
  }
}

// ----------------------------------------------------------------------------------------------------
// CONTROLS -------------------------------------------------------------------------------------------
void WindowOption::control_escape() { *_next_window = WindowName::W_Welcome; }
void WindowOption::control_enter() {
  // Use has to select at least one target type
  if (_widget_select_fields[5]->get_bool() == true || _widget_select_fields[6]->get_bool() == true ||
      _widget_select_fields[7]->get_bool() == true || _widget_select_fields[8]->get_bool() == true) {

    // Up options, save and quit
    _options->set(OptionName::Resolution, _widget_select_fields[0]->get_choice().value);
    _options->set(OptionName::Layout, _widget_select_fields[1]->get_choice().value);
    _options->set(OptionName::Targets, _widget_select_fields[2]->get_choice().value);
    _options->set(OptionName::Countdown, _widget_select_fields[3]->get_choice().value);
    _options->set(OptionName::Speed, _widget_select_fields[4]->get_choice().value);
    _options->set(OptionName::Letters, std::to_string(_widget_select_fields[5]->get_bool()));
    _options->set(OptionName::Capitals, std::to_string(_widget_select_fields[6]->get_bool()));
    _options->set(OptionName::Numbers, std::to_string(_widget_select_fields[7]->get_bool()));
    _options->set(OptionName::Symbols, std::to_string(_widget_select_fields[8]->get_bool()));

    _options->save();

    *_next_window = WindowName::W_Welcome;

  } else
    display_message("  At least one target type is requiered.  ");
}

void WindowOption::control_left() {

  _message_displayed = false;
  for (auto &w : _widget_select_fields) {
    if (w->is_selected()) {
      w->action_left();

      check_new_resolution();
      return;
    }
  }
}
void WindowOption::control_right() {

  _message_displayed = false;
  for (auto &w : _widget_select_fields) {
    if (w->is_selected()) {
      w->action_right();

      check_new_resolution();
      return;
    }
  }
}
