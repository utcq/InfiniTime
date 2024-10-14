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
      class Cheat : public Screen {
      public:
        Cheat();
        ~Cheat() override;
        void Refresh() override;
        bool OnTouchEvent(Pinetime::Applications::TouchEvents event) override;

      private:
        lv_task_t* refreshTask;
        lv_obj_t *ta;
        bool setCursor = false;
        int ta_y = 0;
      };
    }

    template <>
    struct AppTraits<Apps::Cheat> {
      static constexpr Apps app = Apps::Cheat;
      static constexpr const char* icon = Screens::Symbols::phoneSlash;

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::Cheat();
      };
    };
  }
}
