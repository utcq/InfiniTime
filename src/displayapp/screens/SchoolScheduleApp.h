#pragma once

#include "displayapp/apps/Apps.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/widgets/Counter.h"
#include "displayapp/Controllers.h"
#include "Symbols.h"

#include <array>
#include <random>
#include <memory>

constexpr static uint8_t numDays = 6;
constexpr static uint8_t numLessons = 6;

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class DayScreen : public Screen {
        public:
          DayScreen(int day);
          ~DayScreen() override;
      };


      class SchoolSchedule : public Screen {
      public:
        SchoolSchedule();
        ~SchoolSchedule() override;
        void Refresh() override;

      public:
        std::pair<lv_obj_t*, lv_obj_t*> *btnsnlabels[numDays];
        lv_task_t* refreshTask;

        std::unique_ptr<Screen> dayScreen;
      private:
        void CreateButtons();
      };
    }

    template <>
    struct AppTraits<Apps::SchoolSchedule> {
      static constexpr Apps app = Apps::SchoolSchedule;
      static constexpr const char* icon = Screens::Symbols::list;

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::SchoolSchedule();
      };
    };
  }
}