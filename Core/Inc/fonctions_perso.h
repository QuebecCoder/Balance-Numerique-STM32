#ifndef FONCTIONS_PERSO_H_
#define FONCTIONS_PERSO_H_

// FONCTIONS ÉCRAN OLED
void ssdDemarrage(void);
void ssdMAJMesure(uint32_t mesure, bool selectionUnites);

// FONCTIONS CALCULS DE MOYENNE
int32_t moyenne2Mesures(int32_t mesure1, int32_t mesure2);
int32_t moyenneXMesures(int32_t *mesures, int tailleTableau);
int32_t moyenneDoubleXMesures(int32_t *mesures1, int32_t *mesures2, int tailleTableaux);

// FONCTIONS OPTIMISATION
int8_t meilleurGain(int32_t data1, int32_t data2);
void adjustementGainOverflow(int32_t data1,int32_t data2, float gain,int32_t offset1,int32_t offset2);
void adjustementGainSmall(int32_t data1,int32_t data2, float gain,int32_t offset1,int32_t offset2);
#endif
