#include "ssd1306_conf.h"   // Configuration du driver OLED SSD1306
#include "ssd1306_fonts.h"  // Polices d’écriture pour l’affichage
#include "ssd1306.h"        // Fonctions principales du driver SSD1306
#include "ads112c04_hal.h"  // HAL personnalisé pour l’ADC ADS112C04
#include "ads112c04_api_defines.h"
#include "ads112c04_api.h"
#include "ads112c04_core_defines.h"
#include "main.h"           // Fichier principal généré par STM32CubeMX
#include <stdlib.h>         // Librairie standard
#include <stdio.h>          // Printf et co.
#include "fonctions_perso.h"// Fonctions perso utilisées dans ce code
#include <math.h>           // Fonctions mathématiques

extern UART_HandleTypeDef huart2;  // UART2 déclaré ailleurs
extern UART_HandleTypeDef huart4;  // UART4 déclaré ailleurs
extern SSD1306_t SSD1306;          // Structure de l’écran OLED

// Rappels des connexions hardware
// I2C2 : PB10 (SCL), PB11 (SDA)
// UART2 : PA0 (TX), PA1 (RX)
// Bouton UNIT : PC11
// Bouton TARE : PC6
// RESET : PB4
// Data_Ready (ADC) : PB5

// Variables globales liées aux boutons
bool boutonTareAppuye = 0;
bool selectionUnites = 0;    // 0 = grammes, 1 = onces
bool selectionDecimales = 0; // 0 = entier, 1 = un chiffre après la virgule


void newmain(void){
	start:
	int nombreDeMoyenne = 750;   // Nombre d’échantillons pour la moyenne

	int32_t data1 = 0;           // Valeur brute jauge 1
	int32_t data2 = 0;           // Valeur brute jauge 2
	int32_t offset1 = 0;         // Offset jauge 1
	int32_t offset2 = 0;         // Offset jauge 2
	int32_t array_jauge2[nombreDeMoyenne]; // Buffer stockage moyenne jauge 2
	int32_t array_jauge1[nombreDeMoyenne]; // Buffer stockage moyenne jauge 1
	char buffer[16];             // Buffer d'affichage
	int jauge = 0;               // Sélecteur de jauge (0 ou 1)
	double multiplicateur = 1;   // Conversion g / oz
	double ancienneConversion = 0; // Memorisation ancienne valeur pour l’affichage
	float gain=64;               // Gain PGA de l’ADC
	ads112c04_handler_t sensor_handler; // Gestionnaire ADC

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // Sortie RESET à 1
	ads112c04_init(&sensor_handler);                   // Init du convertisseur ADC
	ssdDemarrage();                                    // Démarrage de l’OLED
	ads112c04_setDataRate(&sensor_handler, SPS_1000);  // 1000 samples/sec
    ads112c04_setGain(&sensor_handler, gain);          // Gain PGA = 64
    ads112c04_setRefVoltage(&sensor_handler, REFP_REFN);// Référence externe
    ads112c04_setConversionMode(&sensor_handler, CONTINUOUS_CONVERSION); // Mode continu
    ads112c04_startConversion(&sensor_handler);        // Lancement conversion
    printf("Starting\r\n");

    // ----------- CALCUL OFFSET JAUGE 1 -------------
    for (int var = 0; var < 2500; ++var) {
        ads112c04_setInputs(&sensor_handler, AIN3, AIN2); // Sélection entrée ADC
        if(ads112c04_isDataReady(&sensor_handler) ){
        	data1 = ads112c04_readData(&sensor_handler);
        	offset1 = data1 + offset1; // Somme pour moyenne
        }
	}
    offset1 = offset1 / 2500; // Moyenne finale
	printf("Offset1final:%d\r\n", offset1);

    // ----------- CALCUL OFFSET JAUGE 2 -------------
    for (int var = 0; var < 2500; ++var) {
        ads112c04_setInputs(&sensor_handler, AIN0, AIN1);
        if(ads112c04_isDataReady(&sensor_handler) ){
        	data2 = ads112c04_readData(&sensor_handler);
        	offset2 = data2 + offset2;
        }
	}
    offset2 = offset2 / 2500;
	printf("Offset2final:%d\r\n", offset2);

	while(1){

		// ----------- GESTION BOUTONS -----------
		if(boutonTareAppuye){
			boutonTareAppuye=0;
			 goto start;
		}
		else if(selectionUnites){
			multiplicateur = 0.035274; // g → oz
			printf("hello");
		}
		else if(!selectionUnites){
			multiplicateur = 1.00; // Defaut = g
		}

    	// ----------- LECTURE JAUGE 1 -----------
    	if(jauge=1){ // Ne fonctionne pas avec un ==
            for (int i = 0; i < nombreDeMoyenne; i++) {
            	ads112c04_setInputs(&sensor_handler, AIN3, AIN2);
            	if(ads112c04_isDataReady(&sensor_handler) ){
            		data1 = ads112c04_readData(&sensor_handler);
            		data1 = data1 - offset1;      // Application de l’offset
            		array_jauge1[i] = data1;       // Stockage pour la moyenne
            	}
            	jauge = 0; // Bascule vers l’autre jauge
            }
    	}

    	// ----------- LECTURE JAUGE 2 -----------
    	if(jauge!=1){
            for (int i = 0; i < nombreDeMoyenne; i++) {
            	ads112c04_setInputs(&sensor_handler, AIN0, AIN1);
            	if(ads112c04_isDataReady(&sensor_handler) ){
            		data2 = ads112c04_readData(&sensor_handler);
            		data2 = data2 - offset2;
            		array_jauge2[i] = data2;
            	}
            }
        	jauge = 1;
    	}

    	// ----------- CALCUL MESURE -----------
    	printf("%d\r\n", moyenneDoubleXMesures(array_jauge1, array_jauge2, nombreDeMoyenne));

    	double conversion = (double)moyenneDoubleXMesures(array_jauge1, array_jauge2, nombreDeMoyenne);

    	// Zone morte autour de zéro
    	if(conversion < (gain*.1)){
    		if(conversion > (-gain*.1)){
    			conversion = 0;
    		}
    	}

    	// Conversion brute → volts → unités
    	conversion = conversion / (2.475 * gain);
    	conversion = conversion * multiplicateur;
    	//if(0>conversion<100){
    	//	gain=128;
    	//}
    	//else{
    	//	gain=64;
    	//}
    	// Choix formatage avec ou sans décimales
    	if(selectionDecimales){
        	sprintf(buffer,"%.1f", conversion);
        	nombreDeMoyenne = 750;
    	}
    	else{
        	sprintf(buffer,"%.f", conversion);
        	nombreDeMoyenne = 250;
    	}

    	// Affichage si variation importante
    	if(abs(conversion - ancienneConversion) > 2){
    		ssdWait();
    		ancienneConversion = conversion;
    	}
    	else{
        	ssdMAJMesure(buffer, selectionUnites);
    		ancienneConversion = conversion;
    	}
	}
}


// ----------- INTERRUPTIONS ----------------
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	switch(GPIO_Pin){
		case GPIO_PIN_6:      // Bouton TARE
			boutonTareAppuye = 1;
			break;

		case GPIO_PIN_11:     // Bouton UNIT / DECIMALES
			// selectionUnites = !selectionUnites;   // Choix entre le changement d'unites ou de format d'affichage (xx.x ou xx)
			selectionDecimales = !selectionDecimales; // Commente la ligne non utilise et garder l'autre
			break;

		default:
			break;
	}
}
