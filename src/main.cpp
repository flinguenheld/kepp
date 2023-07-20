#include "controller.h"
#include "loop.h"
#include "option/option_file.h"
#include "renderer.h"
#include "utils.h"
#include "widget/widget_base.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

int main() {
  auto options = std::make_shared<OptionFile>();
  options->read();

  // Resolution & scale --
  auto resolution_option = options->get(OptionName::Resolution);
  auto hyphen_position = resolution_option.find('-');
  uint16_t res = std::stoi(resolution_option.substr(0, hyphen_position));

  const kebb::boxsize screen_size = {res, res};
  const uint16_t scale_factor = std::stoi(resolution_option.substr(++hyphen_position));

  // --
  const uint16_t target_font_size = uint16_t(screen_size.w * scale_factor * 0.072);
  const uint16_t score_font_size = uint16_t(screen_size.w * scale_factor * 0.04);
  const uint16_t menu_font_size = uint16_t(screen_size.w * scale_factor * 0.05);

  auto renderer = std::make_shared<Renderer>(screen_size, scale_factor, target_font_size, score_font_size,
                                             menu_font_size);
  auto score = std::make_shared<Score>();

  Controller controller(options);
  Loop game(screen_size.scale(scale_factor), score, renderer, options);
  game.run(controller);

  std::cout << "Kebb has terminated successfully!\n";
  return 0;
}
