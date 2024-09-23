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
      class SchoolSchedule : public Screen {
      public:
        SchoolSchedule(Controllers::MotionController& motionController,
             Controllers::MotorController& motorController,
             Controllers::Settings& settingsController);
        ~SchoolSchedule() override;
        void Refresh() override;

      public:
        constexpr static uint8_t numBtns = 6;
        const char *days[numBtns] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        std::pair<lv_obj_t*, lv_obj_t*> *btnsnlabels[numBtns];
        lv_task_t* refreshTask;

        constexpr static uint8_t numLessons = 6;
        const char* schoolSchedule[numBtns][numLessons] = {
          {"Italiano", "Religion", "Storia", "Matematica", "Inglese", nullptr},
          {"Chimica", "Informatica", "Italiano", "Matematica", "Disegno", nullptr},
          {"Storia", "Biologia", "Inglese", "Fisica", "PE", nullptr},
          {"Matematica", "Chimica", "Italiano", "Storia", "Fisica", nullptr},
          {"Biologia", "St. Arte", "Inglese", "Matematica", "Informatica", nullptr},
          {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
        };
        lv_obj_t* dayScreen = nullptr;
      private:

        Controllers::MotorController& motorController;
        Controllers::MotionController& motionController;
        Controllers::Settings& settingsController;

        void CreateButtons();
      };
    }

    template <>
    struct AppTraits<Apps::SchoolSchedule> {
      static constexpr Apps app = Apps::SchoolSchedule;
      static constexpr const char* icon = Screens::Symbols::list;

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::SchoolSchedule(controllers.motionController, controllers.motorController, controllers.settingsController);
      };
    };
  }
}