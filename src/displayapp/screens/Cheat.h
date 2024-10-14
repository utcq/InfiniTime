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

static const char* VOLUME = R"(
Il volume è una grandezza fisica che esprime lo spazio occupato da un corpo.
)";

static const char* TEMPERATURA = R"(
La temperatura è una grandezza fisica che esprime il calore di un corpo.
)";

static const char* PRESSIONE = R"(
La pressione è una grandezza fisica che esprime la forza esercitata su una superficie.
)";

static const char* DENSITA = R"(
La densita' è una grandezza fisica che esprime la quantità di materia presente in un volume.
)";

static const char* DECANTAZIONE = R"(
La decantazione
)";

static const char *FILTRAZIONE = R"(
La filtrazione
)";

static const char *EVAPORAZIONE = R"(
L'evaporazione
)";


namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Cheat : public Screen {
      public:
        Cheat();
        ~Cheat() override;
        void Refresh() override;

      private:
        lv_task_t* refreshTask;
        lv_obj_t *btnList;

        lv_obj_t *create_page(const char *text);
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
