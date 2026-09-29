# Apprendre avec les enfants — Arduino (Learning with kids)

Huit petites leçons d'électronique à faire **avec un enfant de 6 à 8 ans**. Chaque leçon donne un résultat qu'on voit, qu'on entend ou qu'on touche : une lumière qui clignote, un bouton qu'on tourne, un jeu à deux joueurs. Le parent monte le circuit et lit le guide ; l'enfant appuie, tourne, observe et essaie les défis.

*Eight short, hands-on electronics lessons for a child aged 6–8 with a parent. Guides are in French; code and code comments are in English, like the rest of this portfolio. Text shown to the child (Serial Monitor messages) is in French.*

## Les leçons

| # | Leçon | L'enfant apprend… | Matériel en plus du montage de base |
|---|---|---|---|
| 1 | [Bonjour, lumière !](01-hello-light/) *(Hello, light!)* | allumer / éteindre, attendre | — |
| 2 | [La lampe de poche Morse secrète](02-morse-flashlight/) *(Secret Morse flashlight)* | les séquences, les codes | buzzer |
| 3 | [Le bouton magique](03-knob-light-meter/) *(Magic knob light meter)* | un capteur donne un nombre | potentiomètre |
| 4 | [Le tir à la corde](04-button-tug-of-war/) *(Button tug-of-war)* | compter, comparer | buzzer |
| 5 | [Le cadenas secret](05-secret-code-lock/) *(Secret-code lock)* | suivre une séquence exacte | — |
| 6 | [Copie mon temps !](06-copy-my-timing/) *(Copy my timing)* | mesurer le temps | — |
| 7 | [Le stationnement des petites voitures](07-toy-car-parking/) *(Toy-car parking lot)* | compter en montant et en descendant | — |
| 8 | [Les clignotants du robot](08-robot-turn-signals/) *(Robot turn signals)* | cause et effet | — |

Faites-les dans l'ordre : chaque leçon ajoute une seule idée nouvelle.

## La trousse (kit)

- 1 **Arduino Uno** et son câble USB
- 1 **platine d'essai** (*breadboard*) et des fils de raccordement (*jumper wires*)
- 4 **DEL** (*LEDs*) : idéalement rouge, jaune, verte, bleue
- 4 **résistances de 220 Ω** (une par DEL)
- 2 **boutons-poussoirs** (*push-buttons*)
- 1 **potentiomètre** (*potentiometer*, 10 kΩ)
- 1 **buzzer piézo passif** (*passive piezo buzzer*)

Tout est alimenté par le **câble USB** (5 V) : pas de piles, pas de prise murale.

## Le montage de base (même câblage pour toutes les leçons)

On garde les mêmes broches (*pins*) partout, pour ne pas tout recâbler à chaque fois.

| Pièce | Broche Arduino | Branchement |
|---|---|---|
| DEL 1 (rouge) | D4 | D4 → résistance 220 Ω → patte longue (+) de la DEL ; patte courte (−) → GND |
| DEL 2 (jaune) | D5 | pareil |
| DEL 3 (verte) | D6 | pareil |
| DEL 4 (bleue) | D7 | pareil |
| Bouton 1 | D2 | une patte → D2, l'autre patte → GND |
| Bouton 2 | D3 | une patte → D3, l'autre patte → GND |
| Potentiomètre | A0 | pattes des côtés → 5V et GND ; patte du milieu → A0 |
| Buzzer | D8 | patte (+) → D8 ; patte (−) → GND |

```
            ARDUINO UNO
          +-------------+
   BTN1 --| D2          |
   BTN2 --| D3          |
 [220]-LED1| D4          |
 [220]-LED2| D5       A0 |-- milieu du potentiomètre
 [220]-LED3| D6       5V |-- côté du potentiomètre
 [220]-LED4| D7      GND |-- ligne GND de la platine (DEL −, boutons, buzzer −, potentiomètre)
  BUZZER(+)| D8          |
          +-------------+
```

Les boutons n'ont pas besoin de résistance : le programme active la résistance interne de l'Arduino (`INPUT_PULLUP`). Bouton appuyé = broche à `LOW`.

## Téléverser un programme (*upload*)

1. Installer l'**Arduino IDE** (arduino.cc/en/software).
2. Ouvrir le fichier `.ino` de la leçon.
3. Menu **Outils → Carte → Arduino Uno**, puis **Outils → Port** : choisir le port de l'Arduino.
4. Cliquer sur la flèche **Téléverser** (*Upload*).
5. Pour les leçons qui écrivent des messages : **Outils → Moniteur série** (*Serial Monitor*), à 9600 bauds.

## Sécurité (à lire une fois)

- **Seulement le câble USB.** Jamais de prise murale, jamais de pile de 9 V branchée au circuit.
- **Chaque DEL a sa résistance.** Sans elle, la DEL peut chauffer et brûler.
- **Débrancher l'USB avant de changer des fils.** On rebranche seulement quand le parent a vérifié.
- **Petites pièces :** les DEL, résistances et fils ne vont pas dans la bouche. Rangez-les loin des plus petits (moins de 3 ans).
- **Le buzzer :** son volume est faible, mais on ne le colle pas contre l'oreille.
- Si quelque chose **chauffe ou sent le brûlé** : débrancher l'USB tout de suite et vérifier le montage avec un adulte.

*Leçon 1 s'inspire de l'exemple « Blink » de l'Arduino (domaine public). Les autres leçons sont originales, inspirées de mes exercices de cours de 2014.*
