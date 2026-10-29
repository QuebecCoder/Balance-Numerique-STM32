# Balance numérique (STM32F072)

Balance à deux jauges de contrainte avec affichage OLED, tare et changement d'unité (g / oz).

Projet réalisé dans le cours Circuits électroniques III au Collège de Maisonneuve, avec Frédéric Le Nabec.

## Fonctionnement

- Les deux jauges de contrainte sont lues par un convertisseur ADS112C04 (24 bits, I²C), à 1000 échantillons/s avec un gain de 64.
- Au démarrage, l'offset de chaque jauge est mesuré à vide puis soustrait de chaque lecture.
- Les lectures sont moyennées pour réduire le bruit, puis converties en grammes ou en onces.
- Le résultat est affiché sur un écran OLED SSD1306.
- Deux boutons permettent de faire la tare et de changer d'unité.

## Matériel

| Élément | Détail |
|---|---|
| Microcontrôleur | STM32 NUCLEO-F072RB |
| Convertisseur | ADS112C04 (16 bits, I²C) |
| Capteurs | Deux jauges de contrainte |
| Écran | OLED SSD1306 (I²C) |
| Alimentation | 12 V |

## Broches

| Fonction | Broche |
|---|---|
| I²C2 SCL / SDA | PB10 / PB11 |
| UART2 TX / RX | PA0 / PA1 |
| Bouton tare | PC6 |
| Bouton unité | PC11 |
| RESET de l'ADC | PB4 |
| DRDY de l'ADC | PB5 |

## Compiler et téléverser

1. Ouvrir STM32CubeIDE.
2. **File → Import → Existing Projects into Workspace**, puis choisir ce dossier.
3. Compiler et téléverser sur la carte NUCLEO.

La configuration des périphériques est dans `Balance.ioc` (STM32CubeMX).

## Structure

```
Balance.ioc               configuration STM32CubeMX
Core/Src/main.c           initialisation des périphériques (généré)
Core/Src/usermain.c       boucle principale de la balance
Core/Src/fonctions_perso.c  affichage et fonctions utilitaires
Core/Src/ads112c04_*.c    pilote du convertisseur ADS112C04
Core/Src/ssd1306*.c       pilote de l'écran OLED
Drivers/                  bibliothèques HAL et CMSIS de ST
```

## Résultat

Balance fonctionnelle avec une précision d'environ ±0,3 g, limitée par le bruit résiduel plutôt que par la résolution du convertisseur.
