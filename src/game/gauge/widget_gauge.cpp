#include "widget_gauge.h"
#include "utils.h"
#include <algorithm>
#include <iostream>

WidgetGauge::WidgetGauge(kebb::boxsize screen_size, std::shared_ptr<Renderer> renderer)
    : _renderer(renderer), _gauge_alpha(100) {

  _color = kebb::color(kebb::ColorName::C_Sky); // FIX: Possible in the initializer list ?

  // Geometry
  _char_size = _renderer->font_char_size(FontName::F_Menu);
  _char_size.set_scale(1.5);
  kebb::boxsize text_size = {static_cast<uint16_t>(_char_size.w * 2), _char_size.h}; // 2 chars for the txt
  kebb::boxsize size = {static_cast<uint16_t>(_char_size.w * 8), _char_size.h}; // 5.5 chars for the gauge

  uint16_t margin = text_size.h * 0.1;
  _pt_insertion = {static_cast<uint16_t>(screen_size.w - size.w - margin),
                   static_cast<uint16_t>(screen_size.h - size.h - margin)};
  kebb::point pt_text = {static_cast<uint16_t>(_pt_insertion.x + (5.5 * _char_size.w)), _pt_insertion.y};

  _textbox_level = std::make_unique<WidgetTextBox>(pt_text, text_size);
  _textbox_level->set_color_text(std::move(_color)); // FIX: Move ?
  _textbox_level->set_text("00");

  // Gauge --
  _padding_y_txt = _char_size.h * 0.2;
  _x_hundred = _char_size.w * 5;
  float y_zero = _pt_insertion.y + _char_size.h - _padding_y_txt;

  uint8_t alpha = 70;

  _triangle = {
      {
          SDL_FPoint{static_cast<float>(_pt_insertion.x), y_zero},
          {_color.r, _color.g, _color.b, _gauge_alpha},
          SDL_FPoint{0},
      },
      {
          SDL_FPoint{0, y_zero},
          {_color.r, _color.g, _color.b, _gauge_alpha},
          SDL_FPoint{0},
      },
      {
          SDL_FPoint{0, 0},
          {_color.r, _color.g, _color.b, _gauge_alpha},
          SDL_FPoint{0},
      },
  };
}

// ----------------------------------------------------------------------------------------------------
// ACCESSORS ------------------------------------------------------------------------------------------
void WidgetGauge::set_percentage(uint16_t val) {

  int x, y;

  if (val >= 0 && val <= 100) {
    x = val * _x_hundred / 100;
    y = (_char_size.h - _padding_y_txt * 2) * (100 - val) / 100; // Reverse the calculation

    _triangle[1].position.x = _pt_insertion.x + x;
    _triangle[2].position.x = _pt_insertion.x + x;
    _triangle[2].position.y = _pt_insertion.y + _padding_y_txt + y;
  }
}
void WidgetGauge::set_text(std::string val) {

  if (val.size() == 1)
    val = "0" + val;

  _textbox_level->set_text(std::move(val));
} // FIX: kepp move ?

// ----------------------------------------------------------------------------------------------------
// RENDER ---------------------------------------------------------------------------------------------
void WidgetGauge::render() const {
  _textbox_level->render(_renderer->renderer(), _renderer->font(FontName::F_Menu));

  SDL_SetRenderDrawColor(_renderer->renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderGeometry(_renderer->renderer(), nullptr, _triangle.data(), _triangle.size(), nullptr, 0);
}
