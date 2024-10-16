#pragma once

#include "displayapp/apps/Apps.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/widgets/Counter.h"
#include "displayapp/Controllers.h"
#include "Symbols.h"

#include <array>
#include <random>

struct Topic {
  const char* name;
  const char* content;
  lv_obj_t* page;
  lv_obj_t* btn;
  bool pageCreated;
};

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Cheat : public Screen {
      public:
        Cheat();
        ~Cheat() override;
        bool OnButtonPushed() override;
        bool OnTouchEvent(Pinetime::Applications::TouchEvents event) override;

        void BuildScreen();

      private:
        lv_obj_t *btnList;
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
