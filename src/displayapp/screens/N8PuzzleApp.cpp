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
  this->DrawGrid();
  this->Update();
  refreshTask = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
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
  } while (countInversions(tiles) % 2 != 0);
  for (int i = 0; i < 9; i++) {
    puzzle[i / 3][i % 3] = tiles[i];
  }
}

N8Puzzle::~N8Puzzle() {
  lv_task_del(refreshTask);
  lv_obj_clean(lv_scr_act());
}

void N8Puzzle::DrawGrid() {

  gridDisplay = lv_table_create(lv_scr_act(), nullptr);

  lv_style_init(&cellStyle);

  lv_style_set_border_color(&cellStyle, LV_STATE_DEFAULT, lv_color_hex(0x708090)); // border
  lv_style_set_border_width(&cellStyle, LV_STATE_DEFAULT, 4);
  lv_style_set_bg_opa(&cellStyle, LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_style_set_bg_color(&cellStyle, LV_STATE_DEFAULT, lv_color_hex(0x2F4F4F)); // bg
  lv_style_set_pad_top(&cellStyle, LV_STATE_DEFAULT, 55);
  lv_style_set_text_color(&cellStyle, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF)); // text

  lv_obj_add_style(gridDisplay, LV_TABLE_PART_CELL1, &cellStyle);

  lv_table_set_col_cnt(gridDisplay, 3);
  lv_table_set_row_cnt(gridDisplay, 3);
  for (int col = 0; col < 3; col++) {
    static constexpr int colWidth = LV_HOR_RES_MAX / 3;
    lv_table_set_col_width(gridDisplay, col, colWidth);
    for (int row = 0; row < 3; row++) {
      lv_table_set_cell_type(gridDisplay, row, col, 1);
      lv_table_set_cell_align(gridDisplay, row, col, LV_LABEL_ALIGN_CENTER);
    }
  }
}

void N8Puzzle::UpdateGrid() {
  for (int i = 0; i < 9; i++) {
    if (puzzle[i / 3][i % 3] == 0) {
      lv_table_set_cell_value(gridDisplay, i / 3, i % 3, "");
      continue;
    }
    lv_table_set_cell_value(gridDisplay, i / 3, i % 3, std::to_string(puzzle[i / 3][i % 3]).c_str());
  }
}
void N8Puzzle::CheckWin() {
  uint8_t solved[9] = {1, 2, 3, 4, 5, 6, 7, 8};
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
  this->UpdateGrid();
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
    case TouchEvents::SwipeUp: {
      this->SwipeUp(emptyIndex);
      return true;
    }
    case TouchEvents::SwipeDown: {
      this->SwipeDown(emptyIndex);
      return true;
    }
    case TouchEvents::SwipeLeft: {
      this->SwipeLeft(emptyIndex);
      return true;
    }
    case TouchEvents::SwipeRight: {
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