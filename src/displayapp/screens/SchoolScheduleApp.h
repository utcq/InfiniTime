#pragma once

#include "displayapp/apps/Apps.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/widgets/Counter.h"
#include "displayapp/Controllers.h"
#include "displayapp/screens/ScreenList.h"
#include "Symbols.h"

#include <array>
#include <random>

static constexpr int nDays = 5;
static constexpr int nLessons = 6;

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class DaySchedule : public Screen {
      public:
        DaySchedule(int day);
        ~DaySchedule() override;
        bool OnTouchEvent(TouchEvents event) override;
      
      private:
        int day;
      };
      class SchoolSchedule : public Screen {
      public:
        struct Item {
          const char *name;
          const char *lessons[nLessons];
        };

        SchoolSchedule(DisplayApp *app);
        ~SchoolSchedule() override;
        bool OnTouchEvent(TouchEvents event) override;
      
      private:
        DisplayApp *app;
        auto CreateScreenList() const;
        std::unique_ptr<Screen> CreateScreen(
          unsigned screenNum
        ) const;

        static constexpr int daysPerScreen = 4;
        std::array<Item, nDays> daysItems;
        static constexpr int nScreens = (
          (nDays-1)/daysPerScreen+1
        );

        ScreenList<nScreens> screens;
      };
    }

    template <>
    struct AppTraits<Apps::SchoolSchedule> {
      static constexpr Apps app = Apps::SchoolSchedule;
      static constexpr const char* icon = Screens::Symbols::list;

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::SchoolSchedule(controllers.displayApp);
      };
    };
  }
}