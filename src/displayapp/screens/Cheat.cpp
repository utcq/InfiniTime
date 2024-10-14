#include "displayapp/screens/Cheat.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Symbols.h"
#include "components/settings/Settings.h"
#include "components/motor/MotorController.h"
#include "components/motion/MotionController.h"

using namespace Pinetime::Applications::Screens;

static const char* CHIMICA = R"(
Decantazione,
Filtrazione,
Centrifugazione,
Critallizzazione,
Estrazione con solvente,
Cromatografia,
Distillazione
)";

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

Cheat::Cheat() {
  this->ta = lv_textarea_create(lv_scr_act(), NULL);
  lv_obj_set_size(ta, LV_HOR_RES-10, LV_VER_RES - 10);
  lv_obj_set_pos(ta, 5, 5);
  lv_textarea_set_text(ta, CHIMICA);
  lv_textarea_set_cursor_pos(ta, 0);
  lv_textarea_set_scrollbar_mode(ta, LV_SCROLLBAR_MODE_ON);
  lv_textarea_set_edge_flash(ta, true);
  
  refreshTask = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}


Cheat::~Cheat() {
  lv_task_del(refreshTask);
  lv_obj_clean(lv_scr_act());
}

bool Cheat::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  /* lv_textarea_set_cursor_pos(ta, ta_y);
  if (event == Pinetime::Applications::TouchEvents::SwipeDown) {
    ta_y += 10;
  }
  if (event == Pinetime::Applications::TouchEvents::SwipeUp) {
    ta_y -= 10;
    ta_y = ta_y < 0 ? 0 : ta_y;
  }
  return true; */
  if (event == Pinetime::Applications::TouchEvents::SwipeDown) {
    ta_y=0; setCursor = true;
    return true;
  }
  return false;
}

void Cheat::Refresh() {
  if (setCursor)
    lv_textarea_set_cursor_pos(ta, ta_y);
    setCursor = false;
}
