#ifndef WINDOW_GAME_SURVIVAL_H
#define WINDOW_GAME_SURVIVAL_H

#include "game/gauge/widget_gauge.h"
#include "window_game.h"
#include <cstdint>
#include <iostream>
#include <sys/types.h>

struct Level {
  uint16_t waiting_time;
  uint16_t nb_target;
  uint16_t next_level;
};

class WindowGameSurvival : public WindowGame {
public:
  WindowGameSurvival(kebb::boxsize screen_size, std::shared_ptr<kebb::WindowName> next_window,
                     std::shared_ptr<Renderer> renderer, std::shared_ptr<Score> score,
                     std::shared_ptr<OptionFile> options);

  virtual ~WindowGameSurvival() override;

  virtual void logic() override;
  virtual void render() const override;

private:
  void add_target();
  uint16_t _nb_max_target;

  std::vector<Level> _levels;
  std::vector<Level>::iterator _current_level;

  void up_points();

  uint16_t _previous_miss;
  uint16_t _previous_fail;
  uint16_t _previous_success;

  uint16_t _previous_level;
  uint16_t _target_waiting_time;
  int16_t _points;

  std::unique_ptr<WidgetGauge> _widget_gauge;
};

#endif // !WINDOW_GAME_SURVIVAL_H
