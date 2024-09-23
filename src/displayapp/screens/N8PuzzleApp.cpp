#include "displayapp/screens/N8PuzzleApp.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Symbols.h"
#include "components/settings/Settings.h"
#include "components/motor/MotorController.h"
#include "components/motion/MotionController.h"

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

N8Puzzle::N8Puzzle(Controllers::MotionController& motionController,
           Controllers::MotorController& motorController,
           Controllers::Settings& settingsController)
  : motorController {motorController}, motionController {motionController}, settingsController {settingsController} {
    std::seed_seq sseq {static_cast<uint32_t>(xTaskGetTickCount()),
                      static_cast<uint32_t>(motionController.X()),
                      static_cast<uint32_t>(motionController.Y()),
                      static_cast<uint32_t>(motionController.Z())};
  gen.seed(sseq);
  this->GeneratePuzzle();
  refreshTask = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
  this->Update();
}

int N8Puzzle::countInversions(uint8_t tiles[9]) {
    int inversions = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = i + 1; j < 9; ++j) {
            if (tiles[i] != 0 && tiles[j] != 0 && tiles[i] > tiles[j]) {
                ++inversions;
            }
        }
    }
    return inversions;
}

void N8Puzzle::GeneratePuzzle() {
  uint8_t tiles[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
  do {
    std::shuffle(std::begin(tiles), std::end(tiles), gen);
  } while (countInversions(tiles) % 2 == 0);
  for (int i = 0; i < 9; i++) {
    puzzle[i / 3][i % 3] = tiles[i];
  }
}



N8Puzzle::~N8Puzzle() {
  lv_task_del(refreshTask);
  lv_obj_clean(lv_scr_act());
}

void N8Puzzle::DrawGrid() {
  // Draw a 3x3 grid
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (puzzle[i][j] == 0) {
        continue;
      }
      lv_obj_t* btn = lv_btn_create(lv_scr_act(), nullptr);
      lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_MAKE(86,96,81));
      lv_obj_set_size(btn, 72, 72);
      lv_obj_align(btn, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 5 + (i * 78), 5 + (j * 78));
      MakeLabel(&jetbrains_mono_bold_20,
                                  LV_COLOR_WHITE,
                                  LV_LABEL_LONG_EXPAND,
                                  0,
                                  LV_LABEL_ALIGN_CENTER,
                                  std::to_string(puzzle[i][j]).c_str(),
                                  btn,
                                  LV_ALIGN_CENTER,
                                  0,
                                  0);
    }
  }
}

void N8Puzzle::CheckWin() {
  uint8_t solved[9] = {1, 4, 7, 2, 5, 8, 3, 6, 0};
  for (int i = 0; i < 9; i++) {
    if (puzzle[i / 3][i % 3] != solved[i]) {
      return;
    }
  }
  this->running=false;
  MakeLabel(&jetbrains_mono_bold_20,
                              LV_COLOR_MAKE(81, 233, 81),
                              LV_LABEL_LONG_EXPAND,
                              0,
                              LV_LABEL_ALIGN_CENTER,
                              "You Win!",
                              lv_scr_act(),
                              LV_ALIGN_CENTER,
                              5,
                              0);
}

void N8Puzzle::Update() {
  lv_obj_clean(lv_scr_act());
  this->DrawGrid();
  this->CheckWin();
}

void N8Puzzle::SwipeLeft(uint8_t emptyIndex) {
  if (emptyIndex % 3 == 2) {
    return;
  }
  puzzle[emptyIndex / 3][emptyIndex % 3] = puzzle[emptyIndex / 3][(emptyIndex % 3) + 1];
  puzzle[emptyIndex / 3][(emptyIndex % 3) + 1] = 0;
  this->Update();
}

void N8Puzzle::SwipeRight(uint8_t emptyIndex) {
  if (emptyIndex % 3 == 0) {
    return;
  }
  puzzle[emptyIndex / 3][emptyIndex % 3] = puzzle[emptyIndex / 3][(emptyIndex % 3) - 1];
  puzzle[emptyIndex / 3][(emptyIndex % 3) - 1] = 0;
  this->Update();
}

void N8Puzzle::SwipeUp(uint8_t emptyIndex) {
  if (emptyIndex / 3 == 2) {
    return;
  }
  puzzle[emptyIndex / 3][emptyIndex % 3] = puzzle[(emptyIndex / 3) + 1][emptyIndex % 3];
  puzzle[(emptyIndex / 3) + 1][emptyIndex % 3] = 0;
  this->Update();
}

void N8Puzzle::SwipeDown(uint8_t emptyIndex) {
  if (emptyIndex / 3 == 0) {
    return;
  }
  puzzle[emptyIndex / 3][emptyIndex % 3] = puzzle[(emptyIndex / 3) - 1][emptyIndex % 3];
  puzzle[(emptyIndex / 3) - 1][emptyIndex % 3] = 0;
  this->Update();
}

bool N8Puzzle::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  if (!running) {
    this->Update();
    return false;
  }
  uint8_t emptyIndex=0;
  for (uint8_t i = 0; i < 3; i++) {
    for (uint8_t j = 0; j < 3; j++) {
      if (puzzle[i][j] == 0) {
        emptyIndex = i * 3 + j;
        break;
      }
    }
  }
  switch (event) {
    case TouchEvents::SwipeLeft: {
      this->SwipeUp(emptyIndex);
      return true;
    }
    case TouchEvents::SwipeRight: {
      this->SwipeDown(emptyIndex);
      return true;
    }
    case TouchEvents::SwipeUp: {
      this->SwipeLeft(emptyIndex);
      return true;
    }
    case TouchEvents::SwipeDown: {
      this->SwipeRight(emptyIndex);
      return true;
    }
    default: {}
  }
  return true;
}

void N8Puzzle::Refresh() {
  //this->Update();
}