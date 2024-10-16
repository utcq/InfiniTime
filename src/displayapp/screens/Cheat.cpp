#include "displayapp/screens/Cheat.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Symbols.h"
#include "components/settings/Settings.h"
#include "components/motor/MotorController.h"
#include "components/motion/MotionController.h"
#include "displayapp/screens/Cheat_text.h"

static bool onMain = true;
static int active = 0;

using namespace Pinetime::Applications::Screens;

lv_obj_t* create_page(const char* text) {
  lv_obj_t* page = lv_obj_create(nullptr, nullptr);
  lv_obj_t* ta = lv_textarea_create(page, nullptr);
  lv_textarea_set_one_line(ta, false);
  lv_textarea_set_text(ta, text);
  lv_textarea_set_cursor_pos(ta, 0);
  lv_obj_set_style_local_bg_color(page, LV_PAGE_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_obj_set_style_local_bg_color(ta, LV_PAGE_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_obj_set_style_local_text_color(ta, LV_PAGE_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_size(ta, LV_HOR_RES, LV_VER_RES - 10);
  lv_obj_set_style_local_text_font(ta, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_bold_20);
  return page;
}

void topic_btn_handler(lv_obj_t* obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    for (unsigned i = 0; i < pagesNum; i++) {
      if (topics[i].btn == obj) {
        if (!topics[i].pageCreated) {
          topics[i].page = create_page(topics[i].content);
          topics[i].pageCreated = true;
          active = i;
        }
        lv_obj_clean(lv_scr_act());
        onMain = false;
        lv_scr_load_anim(topics[i].page, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
        break;
      }
    }
  }
}

void Cheat::BuildScreen() {
  btnList = lv_list_create(lv_scr_act(), nullptr);
  lv_obj_set_size(btnList, LV_HOR_RES, LV_VER_RES);
  lv_obj_align(btnList, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_local_bg_color(btnList, LV_LIST_PART_BG, LV_STATE_DEFAULT, LV_COLOR_BLACK);

  lv_obj_t* list_btn;
  for (unsigned i = 0; i < pagesNum; i++) {
    Topic* topic = &topics[i];
    list_btn = lv_list_add_btn(btnList, nullptr, topic->name);
    lv_obj_set_style_local_bg_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
    lv_obj_set_style_local_radius(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_text_color(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_margin_top(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_pad_top(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_pad_bottom(list_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 10);
    topic->btn = list_btn;
    lv_obj_set_event_cb(list_btn, topic_btn_handler);
  }
}

Cheat::Cheat() {
  this->BuildScreen();
}

Cheat::~Cheat() {
  for (unsigned i = 0; i < pagesNum; i++) {
    if (topics[i].pageCreated && topics[i].page) {
      lv_obj_clean(topics[i].page);
    }
  }
  lv_obj_clean(lv_scr_act());
  lv_obj_clean(btnList);
}

bool Cheat::OnButtonPushed() {
  if (!onMain) {
    lv_obj_clean(lv_scr_act());
    if (topics != nullptr) {
      topics[active].pageCreated = false;
    }
    onMain = true;
    BuildScreen();
    return true;
  }
  return false;
}

bool Cheat::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  if (event == TouchEvents::SwipeDown) { return true; }
  return false;
}