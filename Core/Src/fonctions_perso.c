#include "ssd1306_conf.h"   // Configuration du module OLED SSD1306
#include "ssd1306_fonts.h"  // Polices utilisées pour afficher du texte
#include "ssd1306.h"        // Fonctions principales du driver SSD1306
#include "ads112c04_hal.h"  // HAL spécifique pour l’ADC ADS112C04
#include "ads112c04_api_defines.h"
#include "ads112c04_api.h"
#include "ads112c04_core_defines.h"
#include "main.h"           // Fichier principal du projet STM32 (HAL)
#include <stdlib.h>         // Fonctions utilitaires standard
#include <stdio.h>          // printf, sprintf, etc.
#include "stdbool.h"        // Gestion du type bool
#include "fonctions_perso.h"// Fonctions personnalisées

ads112c04_handler_t sensor_handler;  // Instance globale du gestionnaire ADC

// -------------------------------------------------------------
// ÉCRAN : Affichage du message de démarrage
// -------------------------------------------------------------
void ssdDemarrage(void){
	ssd1306_Init();                       // Initialisation du display OLED
	ssd1306_Fill(Black);                  // Efface l'écran
	ssd1306_SetCursor(20, 30);            // Placement du curseur
	ssd1306_WriteString("Wait...", Font_11x18, White); // Texte "Wait..."
	ssd1306_UpdateScreen();               // Rafraîchit l'écran
}

// -------------------------------------------------------------
// ÉCRAN : Attente
// -------------------------------------------------------------
void ssdWait(void){
	ssd1306_Fill(Black);
	ssd1306_SetCursor(20, 30);
	ssd1306_WriteString("Wait...", Font_11x18, White);
	ssd1306_UpdateScreen();
}

// -------------------------------------------------------------
// ÉCRAN : Affichage de la mesure avec unité
// - 'selectionUnites' : 0 = g, 1 = oz
// -------------------------------------------------------------
void ssdMAJMesure(uint32_t mesure, bool selectionUnites){
	ssd1306_Fill(Black);                      // Vide l'écran avant écriture
	if(!selectionUnites){                     // Affichage en grammes
		ssd1306_SetCursor(33, 26);
		ssd1306_WriteString((char*)mesure, Font_11x18, White); // Converti en texte
		ssd1306_WriteString(" g", Font_11x18, White);
	}
	else{                                     // Affichage en onces
		ssd1306_SetCursor(17, 26);
		ssd1306_WriteString((char*)mesure, Font_11x18, White);
		ssd1306_WriteString(" oz", Font_11x18, White);
	}
	ssd1306_UpdateScreen();                   // Mise à jour écran
}

// -------------------------------------------------------------
// Moyenne simple entre deux mesures
// -------------------------------------------------------------
int32_t moyenne2Mesures(int32_t mesure1, int32_t mesure2){
	int32_t moyenne = (mesure1 + mesure2) / 2;
	return moyenne;
}

// -------------------------------------------------------------
// Moyenne d'un tableau de mesures
// -------------------------------------------------------------
int32_t moyenneXMesures(int32_t *mesures, int tailleTableau){
	int32_t somme = 0;
	int32_t moyenne = 0;

	for(int i = 0; i < tailleTableau; i++){
		somme += mesures[i];        // Additionne toutes les valeurs
	}

	moyenne = somme / tailleTableau; // Moyenne entière
	return moyenne;
}

// -------------------------------------------------------------
// Moyenne de deux tableaux (chaque tableau est moyenné séparément)
// puis on fait la moyenne des deux moyennes
// -------------------------------------------------------------
int32_t moyenneDoubleXMesures(int32_t *mesures1, int32_t *mesures2, int tailleTableaux){
	int32_t moyenne = 0;

	moyenne = moyenne2Mesures(
			moyenneXMesures(mesures1, tailleTableaux),
			moyenneXMesures(mesures2, tailleTableaux));

	return moyenne;
}
