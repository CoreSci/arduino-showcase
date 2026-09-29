# Leçon 2 — La lampe de poche Morse secrète *(Secret Morse flashlight)*

**Âge :** 6–8 ans, avec un parent · **Montage :** ~10 minutes · **Jeu :** 20 minutes

## Objectif (*goal*)
Envoyer un **message secret en code Morse** avec de la lumière et du son : d'abord **SOS**, puis **son propre prénom**. L'enfant découvre qu'un code, c'est une **séquence** (*sequence*) de signaux courts et longs dans le bon ordre.

## Matériel
- Montage de la leçon 1 (DEL sur D4 + résistance 220 Ω)
- 1 buzzer piézo passif (*passive piezo buzzer*)

## Montage (~10 minutes)
```
 Arduino D4 ───[ 220 Ω ]───(+)DEL(−)───┐
 Arduino D8 ──────────────(+)BUZZER(−)─┴── GND
```
1. Débrancher l'USB.
2. Garder la DEL de la leçon 1.
3. Planter le buzzer. Relier sa patte (+), souvent la plus longue ou marquée « + », à **D8**, et l'autre à **GND**.
4. Le parent vérifie, puis on branche l'USB et on ouvre le **Moniteur série** (*Serial Monitor*, 9600 bauds) pour voir les lettres.

## Téléverser (*upload*)
Ouvrir `02-morse-flashlight.ino` et **Téléverser** ([guide général](../README.md#téléverser-un-programme-upload)).

## Comment ça marche
- **Pour le parent :** le tableau `MORSE` contient le code de chaque lettre A–Z (« . » = court, « - » = long). `sendLetter()` cherche la lettre et fait un signal court (200 ms) ou long (600 ms) pour chaque symbole, avec la lumière **et** le son (`tone()`). Des pauses séparent les signaux, les lettres et les mots, comme dans le vrai code Morse.
- **Pour l'enfant :** « S, c'est trois petits bips : bip-bip-bip. O, c'est trois longs : biiip-biiip-biiip. SOS veut dire “à l'aide !” partout dans le monde. »

| Lettre | Morse | | Lettre | Morse |
|---|---|---|---|---|
| A | `.-` | | N | `-.` |
| E | `.` | | O | `---` |
| I | `..` | | S | `...` |
| L | `.-..` | | T | `-` |

## Essaie ça ! (*try this*)
1. **Mon prénom en Morse :** remplacer `"SOS"` par le prénom de l'enfant (lettres sans accents), par exemple `"LEO"`. Écouter et regarder.
2. **Devine le mot :** le parent met un mot secret de 2–3 lettres ; l'enfant écrit les points et les traits sur papier, puis devine le mot avec le tableau.
3. **Voix aiguë, voix grave :** changer `BEEP_PITCH = 700` en `300` (grave) ou `1500` (aigu).
4. **Mode espion :** rendre le code plus rapide en changeant `DOT = 200` en `100`.

## Sécurité
- Le buzzer ne se colle pas contre l'oreille.
- Seulement le câble USB. On débranche avant de changer des fils.
- Petites pièces : pas dans la bouche.
