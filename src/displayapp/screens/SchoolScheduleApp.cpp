#include "displayapp/screens/SchoolScheduleApp.h"
#include <lvgl/lvgl.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Screen.h"
#include "components/settings/Settings.h"
#include "displayapp/screens/ScreenList.h"

using namespace Pinetime::Applications::Screens;

namespace {
  lv_obj_t* MakeLabel(lv_font_t* font,
                      lv_color_t color,
                      lv_label_long_mode_t longMode,
                      uint8_t width,
                      lv_label_align_t labelAlignment,
                      const char* text,
                      lv_obj_t* reference,
                      lv_align_t alignment,
                      int8_t x,
                      int8_t y) {
    lv_obj_t* label = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, font);
    lv_obj_set_style_local_text_color(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, color);
    lv_label_set_long_mode(label, longMode);
    if (width != 0) {
      lv_obj_set_width(label, width);
    }
    lv_label_set_align(label, labelAlignment);
    lv_label_set_text(label, text);
    lv_obj_align(label, reference, alignment, x, y);
    return label;
  }
}

DaySchedule::DaySchedule(int day): day(day) {
}

DaySchedule::~DaySchedule() {
  lv_obj_clean(lv_scr_act());
}

auto SchoolSchedule::CreateScreenList() const {
  std::array<std::function<std::unique_ptr<Screen>()>, nScreens> screens;
  for (size_t i = 0; i < screens.size(); i++) {
    screens[i] = [this, i]() -> std::unique_ptr<Screen> {
      return CreateScreen(i);
    };
  }
  return screens;
}

SchoolSchedule::SchoolSchedule(DisplayApp *app)
  : app {app},
  screens {app, 0, CreateScreenList(), Screens::ScreenListModes::UpDown} {
}

SchoolSchedule::~SchoolSchedule() {
  lv_obj_clean(lv_scr_act());
}

bool SchoolSchedule::OnTouchEvent(TouchEvents event) {
  return screens.OnTouchEvent(event);
}

std::unique_ptr<Screen> SchoolSchedule::CreateScreen(unsigned int screenNum) const {
  std::array<Screens::Screen, daysPerScreen> screens;
  for (int i = 0; i < daysPerScreen; i++) {
    int day_index = screenNum * daysPerScreen + i;
    screens[i] = DaySchedule(day_index);
  }
}