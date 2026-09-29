# Leçon 7 — Le stationnement des petites voitures *(Toy-car parking lot)*

**Âge :** 6–8 ans, avec un parent et quelques petites voitures · **Montage :** ~10 minutes · **Jeu :** 15–20 minutes

## Objectif (*goal*)
Un **stationnement de 4 places** pour petites voitures. On appuie sur le **bouton 1** quand une voiture **entre**, sur le **bouton 2** quand elle **sort**. Chaque lumière allumée est une place prise. Quand c'est plein, les lumières clignotent : **COMPLET !** L'enfant découvre un **compteur** (*counter*) qui monte et qui descend, avec un minimum (0) et un maximum (4).

## Matériel
- 4 DEL + 4 résistances de 220 Ω
- 2 boutons-poussoirs
- 4 ou 5 petites voitures, et un carton pour dessiner le stationnement (optionnel mais très amusant !)

## Montage (~10 minutes)
```
 D4 ─[220 Ω]─ DEL place 1 ─┐
 D5 ─[220 Ω]─ DEL place 2 ─┤
 D6 ─[220 Ω]─ DEL place 3 ─┼── GND
 D7 ─[220 Ω]─ DEL place 4 ─┘

 BOUTON 1 (entrée / car in)  : D2 ─┤├─ GND
 BOUTON 2 (sortie / car out) : D3 ─┤├─ GND
```
1. Débrancher l'USB.
2. Placer les 4 DEL : ce sont les 4 places. On peut dessiner 4 cases sur un carton, à côté de chaque DEL.
3. Bouton 1 (**entrée**) entre **D2** et **GND**, bouton 2 (**sortie**) entre **D3** et **GND**.
4. Le parent vérifie, puis on branche l'USB et on ouvre le **Moniteur série** (9600 bauds).

## Téléverser (*upload*)
Ouvrir `07-toy-car-parking.ino` et **Téléverser** ([guide général](../README.md#téléverser-un-programme-upload)).

## Comment ça marche
- **Pour le parent :** la variable `cars` compte les voitures. « Entrée » fait `cars++` seulement s'il reste de la place (`cars < 4`) ; sinon `flashFull()` fait clignoter toutes les DEL. « Sortie » fait `cars--`, sans jamais descendre sous 0. `showCars()` allume autant de DEL qu'il y a de voitures.
- **Pour l'enfant :** « L'Arduino est le gardien du stationnement. Il compte les voitures qui entrent et qui sortent. Quand les 4 places sont prises, il dit “COMPLET !” »

## Essaie ça ! (*try this*)
1. **Jouer pour de vrai :** chaque fois qu'une petite voiture entre dans une case, appuyer sur « entrée ». Le nombre de lumières est-il toujours égal au nombre de voitures ?
2. **Le cinquième client :** essayer de faire entrer une 5e voiture. Que fait le gardien ?
3. **Le stationnement vide :** appuyer sur « sortie » quand il n'y a plus de voitures. Le compteur descend-il sous zéro ?
4. **Petit stationnement :** changer `SPOTS = 4` en `3`. Maintenant, c'est complet plus vite !

## Sécurité
- Les petites voitures restent loin des fils, pour ne pas débrancher le montage.
- Seulement le câble USB. On débranche avant de changer des fils.
- Petites pièces : pas dans la bouche.
