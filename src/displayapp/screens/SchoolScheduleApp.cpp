#include "displayapp/screens/SchoolScheduleApp.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Symbols.h"
#include "components/settings/Settings.h"
#include "components/motor/MotorController.h"
#include "components/motion/MotionController.h"
#include "displayapp/DisplayApp.h"

using namespace Pinetime::Applications::Screens;

const char *days[numDays] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char* schoolSchedule[numDays][numLessons] = {
  {"Italiano", "Religion", "Storia", "Matematica", "Inglese", nullptr},
  {"Chimica", "Informatica", "Italiano", "Matematica", "Disegno", nullptr},
  {"Storia", "Biologia", "Inglese", "Fisica", "PE", nullptr},
  {"Matematica", "Chimica", "Italiano", "Storia", "Fisica", nullptr},
  {"Biologia", "St. Arte", "Inglese", "Matematica", "Informatica", nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
};


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

  void MakeThickLine(lv_obj_t* parent, lv_coord_t y, lv_color_t color, uint16_t thickness) {
    static lv_point_t line_points[] = {{0, 0}, {240, 0}};
    lv_obj_t* line = lv_line_create(parent, NULL);
    lv_line_set_points(line, line_points, 2);
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_line_color(&style, LV_STATE_DEFAULT, color);
    lv_style_set_line_width(&style, LV_STATE_DEFAULT, thickness);
    lv_obj_add_style(line, LV_LINE_PART_MAIN, &style);
    lv_obj_align(line, parent, LV_ALIGN_IN_TOP_MID, 0, y);
  }

  void dayClickEventHandler(lv_obj_t *obj, lv_event_t event) {
    if (event != LV_EVENT_CLICKED) { return; }
    auto* screen = static_cast<SchoolSchedule*>(obj->user_data);
    int day_index = 0;
    for (int i=0;i<numDays;i++) {
      if (screen->btnsnlabels[i]->first == obj) {
        day_index = i;
        break;
      }
    }
    lv_obj_clean(lv_scr_act());
    screen->dayScreen = std::make_unique<DayScreen>(day_index);
  }
}

DayScreen::DayScreen(int day_index) {
  MakeLabel(&jetbrains_mono_bold_20,
            LV_COLOR_GREEN,
            LV_LABEL_LONG_EXPAND,
            0,
            LV_LABEL_ALIGN_CENTER,
            days[day_index],
            lv_scr_act(),
            LV_ALIGN_IN_TOP_MID,
            0,
            10);
  for (int i=0;i<numLessons;i++) {
    if ((i==0) | (i>0 && schoolSchedule[day_index][i-1]!=nullptr)) {
      MakeThickLine(lv_scr_act(), (120+((i*30)-75)), LV_COLOR_GRAY, 2);
    }
    if (schoolSchedule[day_index][i]!=nullptr) {
      MakeLabel(&jetbrains_mono_bold_20,
                LV_COLOR_TEAL,
                LV_LABEL_LONG_EXPAND,
                0,
                LV_LABEL_ALIGN_LEFT,
                schoolSchedule[day_index][i],
                lv_scr_act(),
                LV_ALIGN_IN_LEFT_MID,
                5,
                (i*30)-60);
    } else if (i == (numLessons-1)) {
      continue;
    }
    if (i == (numLessons-1)) {
      MakeThickLine(lv_scr_act(), (120+((i*30)-45)), LV_COLOR_GRAY, 2);
    }
  }
}

DayScreen::~DayScreen() {
  lv_obj_clean(lv_scr_act());
}

void SchoolSchedule::CreateButtons() {
  for (int i=0;i<numDays;i++) {
    lv_obj_t *btn = lv_btn_create(lv_scr_act(), nullptr);
    btn->user_data = this;
    lv_obj_set_size(btn, 110, 50);
    lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
    unsigned x = ((i%2)*120)+5;
    unsigned y = ((i/2)*60)+35 ;
    lv_obj_align(btn, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, x, y);
    lv_obj_set_event_cb(btn, dayClickEventHandler);
    lv_obj_t *label = MakeLabel(&jetbrains_mono_bold_20,
                           LV_COLOR_WHITE,
                           LV_LABEL_LONG_EXPAND,
                           0,
                           LV_LABEL_ALIGN_CENTER,
                           days[i],
                           btn,
                           LV_ALIGN_CENTER,
                           0,
                           0);
    btnsnlabels[i] = new std::pair<lv_obj_t*, lv_obj_t*>(
      btn,
      label
    );
  }
}

SchoolSchedule::SchoolSchedule() {
  this->CreateButtons();
  refreshTask = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
}

SchoolSchedule::~SchoolSchedule() {
  lv_task_del(refreshTask);
  if (this->dayScreen.get() != nullptr) {
    delete this->dayScreen.release();
  }
  lv_obj_clean(lv_scr_act());
}

void SchoolSchedule::Refresh() {
  if (this->dayScreen.get() != nullptr) {
    //this->dayScreen->Refresh();
  }
}
