# Leçon 8 — Les clignotants du robot *(Robot turn signals)*

**Âge :** 6–8 ans, avec un parent · **Montage :** ~10 minutes · **Jeu :** 10–15 minutes

## Objectif (*goal*)
Les **clignotants** (*turn signals*) d'un robot ou d'une voiture. Le **bouton 1** fait clignoter les lumières de **gauche**, le **bouton 2** celles de **droite**, et **les deux boutons ensemble** allument les **feux de détresse** (*hazard lights*) : les quatre clignotent. L'enfant découvre la **cause et l'effet** (*cause and effect*) : chaque bouton produit un résultat différent.

## Matériel
- 4 DEL (idéalement 4 jaunes ou orange, comme de vrais clignotants) + 4 résistances de 220 Ω
- 2 boutons-poussoirs
- une boîte en carton pour faire le robot (optionnel)

## Montage (~10 minutes)
```
        GAUCHE (left)                         DROITE (right)
 D4 ─[220 Ω]─ DEL ─┐                   D6 ─[220 Ω]─ DEL ─┐
 D5 ─[220 Ω]─ DEL ─┴── GND             D7 ─[220 Ω]─ DEL ─┴── GND

 BOUTON 1 (gauche) : D2 ─┤├─ GND        BOUTON 2 (droite) : D3 ─┤├─ GND
```
1. Débrancher l'USB.
2. Placer 2 DEL à **gauche** (D4, D5) et 2 DEL à **droite** (D6, D7) de la platine, chacune avec sa résistance.
3. Bouton 1 entre **D2** et **GND** (à gauche), bouton 2 entre **D3** et **GND** (à droite).
4. Le parent vérifie, puis on branche l'USB.

## Téléverser (*upload*)
Ouvrir `08-robot-turn-signals.ino` et **Téléverser** ([guide général](../README.md#téléverser-un-programme-upload)).

## Comment ça marche
- **Pour le parent :** `loop()` lit les deux boutons. Dès qu'un bouton est enfoncé, le programme attend 80 ms pour laisser le temps d'appuyer sur les deux ensemble, puis appelle `blinkSides(left, right)`, qui fait clignoter le ou les côtés choisis 3 fois (`BLINKS`) à la vitesse `SPEED`.
- **Pour l'enfant :** « Quand une voiture tourne à gauche, elle clignote à gauche pour avertir les autres. Toi, tu es le conducteur du robot ! »

## Essaie ça ! (*try this*)
1. **La balade du robot :** le parent dit « à gauche ! », « à droite ! », « danger ! » et l'enfant appuie sur le bon bouton le plus vite possible.
2. **Clignotant plus long :** changer `BLINKS = 3` en `6`.
3. **Clignotant plus rapide :** changer `SPEED = 300` en `150`.
4. **Le robot en carton :** coller les DEL sur une boîte (les yeux, les bras…) avec de longs fils. Le parent vérifie le montage avant de rebrancher.

## Sécurité
- Si on utilise une boîte en carton, les fils ne doivent pas se toucher entre eux.
- Seulement le câble USB. On débranche avant de changer des fils.
- Petites pièces : pas dans la bouche.
