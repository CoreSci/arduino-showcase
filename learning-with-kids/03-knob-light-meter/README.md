# Leçon 3 — Le bouton magique *(Magic knob light meter)*

**Âge :** 6–8 ans, avec un parent · **Montage :** ~10 minutes · **Jeu :** 10–15 minutes

## Objectif (*goal*)
Tourner un **bouton** (*knob*, un potentiomètre) et voir **de plus en plus de lumières s'allumer**. L'enfant découvre qu'un **capteur** (*sensor*) transforme le monde réel en **nombre**, et que le programme décide quoi faire avec ce nombre.

## Matériel
- 4 DEL + 4 résistances de 220 Ω
- 1 potentiomètre (*potentiometer*, 10 kΩ)
- fils de raccordement

## Montage (~10 minutes)
```
 D4 ─[220 Ω]─ DEL1 ─┐
 D5 ─[220 Ω]─ DEL2 ─┤
 D6 ─[220 Ω]─ DEL3 ─┼── GND
 D7 ─[220 Ω]─ DEL4 ─┘

 Potentiomètre :   5V ── (patte gauche)
                   A0 ── (patte du milieu)
                  GND ── (patte droite)
```
1. Débrancher l'USB.
2. Placer les 4 DEL en ligne, comme une **jauge** : rouge, jaune, verte, bleue. Chaque DEL a sa résistance.
3. Planter le potentiomètre. Les pattes des côtés vont à **5V** et **GND**, celle du milieu à **A0**.
4. Le parent vérifie, puis on branche l'USB et on ouvre le **Moniteur série** (9600 bauds).

## Téléverser (*upload*)
Ouvrir `03-knob-light-meter.ino` et **Téléverser** ([guide général](../README.md#téléverser-un-programme-upload)).

## Comment ça marche
- **Pour le parent :** `analogRead(A0)` donne un nombre de **0** (tourné à fond d'un côté) à **1023** (à fond de l'autre). `map()` transforme ce nombre en 0, 1, 2, 3 ou 4 lumières. Le Moniteur série affiche les deux nombres dix fois par seconde.
- **Pour l'enfant :** « Le bouton, c'est comme le volume de la radio. L'Arduino regarde jusqu'où tu l'as tourné et allume autant de lumières. »

## Essaie ça ! (*try this*)
1. **Le nombre secret :** tourner lentement et regarder le nombre changer dans le Moniteur série. Peux-tu t'arrêter exactement sur 500 ?
2. **Le jeu du parent :** le parent dit « deux lumières ! » et l'enfant doit tourner jusqu'à en avoir exactement deux.
3. **À l'envers :** échanger les fils 5V et GND du potentiomètre. Que se passe-t-il quand on tourne ?
4. **Les lumières qui clignotent :** ajouter une ligne pour que la 4e DEL clignote quand le bouton est au maximum (`knob > 1000`).

## Sécurité
- Chaque DEL a sa résistance.
- Ne jamais relier directement 5V à GND : le potentiomètre se branche avec ses trois pattes.
- Seulement le câble USB. On débranche avant de changer des fils.
