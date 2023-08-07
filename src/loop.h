#ifndef LOOP_H
#define LOOP_H

#include "SDL.h"
#include "SDL_blendmode.h"
#include "SDL_timer.h"
#include "SDL_ttf.h"
#include "about/window_about.h"
#include "controller.h"
#include "file/option_file.h"
#include "file/record_file.h"
#include "game/window/survival_mod/window_survival_mod.h"
#include "game/window/survival_mod/window_welcome_survival.h"
#include "game/window/timer_mod/window_timer_mod.h"
#include "game/window/timer_mod/window_welcome_timer.h"
#include "game/window/window_gameover.h"
#include "option/window_option.h"
#include "renderer.h"
#include "welcome/window_welcome.h"

class Loop {
public:
  Loop(kebb::boxsize screen_size, std::shared_ptr<Renderer> renderer, std::shared_ptr<OptionFile> options);

  void run(Controller &controller);

private:
  std::shared_ptr<WidgetWindow> _current_window; // Shared with the controller

  std::shared_ptr<RecordFile> _records; // Create here to avoid repetitive read/write actions
  std::shared_ptr<OptionFile> _options;

  std::shared_ptr<Renderer> _renderer;
  const kebb::boxsize _screen_size;
};

#endif
