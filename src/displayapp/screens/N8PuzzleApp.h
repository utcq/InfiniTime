#pragma once

#include "displayapp/apps/Apps.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/widgets/Counter.h"
#include "displayapp/Controllers.h"
#include "Symbols.h"

#include <array>
#include <random>

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class N8Puzzle : public Screen {
      public:
        N8Puzzle(Controllers::MotionController& motionController,
             Controllers::MotorController& motorController,
             Controllers::Settings& settingsController);
        ~N8Puzzle() override;
        bool OnTouchEvent(Pinetime::Applications::TouchEvents event) override;
        void Refresh() override;

      private:
        std::mt19937 gen;

        Controllers::MotorController& motorController;
        Controllers::MotionController& motionController;
        Controllers::Settings& settingsController;

        typedef uint8_t Puzzle[3][3];

        Puzzle puzzle;
        void GeneratePuzzle();
        int countInversions(uint8_t tiles[9]);
        void Update();
        void DrawGrid();
        void UpdateGrid();
        void CheckWin();

        void SwipeLeft(uint8_t emptyIndex);
        void SwipeRight(uint8_t emptyIndex);
        void SwipeUp(uint8_t emptyIndex);
        void SwipeDown(uint8_t emptyIndex);

        bool running = true;

        lv_task_t* refreshTask;
        lv_obj_t *gridDisplay;
        lv_style_t cellStyle;
        };
    }
    template <>
    struct AppTraits<Apps::N8Puzzle> {
      static constexpr Apps app = Apps::N8Puzzle;
      static constexpr const char* icon = "N8";

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::N8Puzzle(controllers.motionController, controllers.motorController, controllers.settingsController);
      };
    };
  }
}