#include "displayapp/screens/Cheat.h"
#pragma once
#define QUADRILATERO "poligono: 4 lati e 4 angoli"
#define PARALLELOGRAMMA "Quadrilatero con lati opposti paralleli\ndiagonale AC, triangoli con AC in comune,\nBAC=ACD, rette tagliate da AC\nBCA=CAD, rette tagliate da AC\nBAD=BCD somme angoli interni\nABC=ADC per triangoli congruenti\nAB=CD lati opposti, ABM=CDM angoli alterni interni\nBAM=CMD angoli corrispondenti angoli alterni interni "
const unsigned pagesNum = 2;
static Topic topics[] = {
  {"Quadrilatero", QUADRILATERO},
  {"Parallelogramma", PARALLELOGRAMMA},
};
