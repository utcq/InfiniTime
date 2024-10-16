#include "displayapp/screens/Cheat.h"
#pragma once
#define QUADRILATERO "poligono: 4 lati e 4 angoli"
#define PARALLELOGRAMMA "Quadrilatero con lati opposti paralleli\nProprieta':\n  - Lati opposti congruenti\n  - Angoli opposti congruenti\n  - Diagonali si tagliano a meta'\n  - Due lati paralleli e congruenti sono sufficienti\n\nTeoremi:\n  - Lati opposti congruenti\n  - Angoli opposti congruenti\n  - Diagonali si tagliano a meta' formano triangoli congruenti\n  - Un quadrilatero è parallelogramma se ha lati opposti o angoli congruenti"
#define PG_DIM "1. Lati opposti congruenti:\n  - Traccia AC (diagonale)\n  - Triangoli ABC e ADC condividono AC\n  - BAC = ACD (angoli alterni interni, rette AB e CD tagliate da AC)\n  - BCA = CAD (angoli alterni interni)\n  - Triangoli congruenti -> AB = CD, BC = AD\n\n2. Angoli opposti congruenti:\n  - Traccia AC (diagonale)\n  - BAC = ACD, BCA = CAD (come sopra)\n  - BAD = BCD (somme di angoli congruenti)\n  - ABC = ADC (triangoli congruenti)\n3. Diagonali si tagliano a meta':\n  - Triangoli AMB e CMD condividono diagonale BD\n  - AB = CD (lati opposti congruenti)\n  - BAM = CMD (angoli alterni interni)\n  - Triangoli congruenti -> AM = MC, BM = MD"
#define RETTANGOLO "Quadrilatero con tutti gli angoli retti\n\nProprieta':\n  - Angoli opposti congruenti (perche' retti)\n  - Lati opposti paralleli e congruenti\n  - Le diagonali si tagliano a meta'\n  - Le diagonali sono congruenti\n\nTeorema:\n  - In un rettangolo le diagonali sono congruenti: AC = BD\n  - Un parallelogramma con un angolo retto è un rettangolo\n  - Se le diagonali di un parallelogramma sono congruenti, è un rettangolo"
#define RT_DIM_DIAG "- Triangoli DAB e ADC hanno:\n  - AB = DC (lati opposti congruenti)\n  - AD in comune\n  - DAB = ADC (angoli retti)\n- I triangoli sono congruenti per il primo criterio\n- Quindi AC = BD (diagonali congruenti)"
#define RT_DIM_RETTO "- In un parallelogramma, gli angoli adiacenti a un lato sono supplementari\n- DAB + ADC = 180 (supplementari)\n- Poiche' DAB = ADC (congruenti), allora sono angoli retti\n- Il parallelogramma ha tutti gli angoli retti, quindi è un rettangolo"
#define TRAPEZIO "Quadrilatero con solo due lati paralleli (basi)\n\nProprieta':\n  - Base maggiore: lato parallelo più lungo\n  - Base minore: lato parallelo più corto\n  - Lati obliqui: non paralleli\n  - Altezza: segmento perpendicolare alle basi\n  - Angoli adiacenti ai lati obliqui sono supplementari (angoli coniugati interni)\n\nTeorema (Trapezio isoscele):\n  - In un trapezio isoscele, gli angoli adiacenti a ciascuna base sono congruenti\n  - Le diagonali di un trapezio isoscele sono congruenti\nTrapezio Isoscele:\n  - Ha i lati obliqui congruenti\nTrapezio Rettangolo:\n  - Ha uno dei lati obliqui perpendicolare alle basi"
#define TR_DIM_ANG "- Tracciamo le altezze BE e CF\n- I triangoli rettangoli ABE e FCD hanno:\n  - AB = CD (lati obliqui congruenti per ipotesi)\n  - BE = CF (lati opposti del rettangolo EBCF)\n- I triangoli sono congruenti per il criterio ipotenusa-cateto\n- Quindi alpha = delta, beta = gamma (angoli adiacenti alle basi congruenti)"
#define TR_DIM_DIAG "- I triangoli ABC e BCD hanno:\n  - BC in comune\n  - AB = CD (lati obliqui congruenti per ipotesi)\n  - ABC = BCD (angoli adiacenti alla stessa base congruenti)\n- I triangoli sono congruenti per il primo criterio\n- Quindi BD = AC (diagonali congruenti)"
#define TEO_INV_ISO "- Se in un trapezio gli angoli adiacenti a una base sono congruenti, allora il trapezio è isoscele"
#define TR_DIM_INV "- Tracciamo le altezze BE e CF\n- I triangoli rettangoli ABE e FCD hanno:\n  - alpha = delta (angoli adiacenti alla base congruenti per ipotesi)\n  - BE = CF (lati opposti del rettangolo EBCF)\n- I triangoli sono congruenti per un angolo acuto e un cateto congruenti\n- Quindi AB = CD (trapezio isoscele)"
static Topic *topics;
const unsigned pagesNum = 11;
Topic *setup_topics() {
	topics = new Topic[11] {
		{"Quadrilatero", QUADRILATERO},
		{"Parallelogramma", PARALLELOGRAMMA},
		{"PG. DIM", PG_DIM},
		{"Rettangolo", RETTANGOLO},
		{"RT. DIM (DIAG)", RT_DIM_DIAG},
		{"RT. DIM (RETTO)", RT_DIM_RETTO},
		{"Trapezio", TRAPEZIO},
		{"TR. DIM (ANG)", TR_DIM_ANG},
		{"TR. DIM (DIAG)", TR_DIM_DIAG},
		{"TEO. INV (ISO)", TEO_INV_ISO},
		{"TR. DIM (INV)", TR_DIM_INV},
	};
	return topics;
}