# Leçon 5 — Le cadenas secret *(Secret-code lock)*

**Âge :** 6–8 ans, avec un parent · **Montage :** ~10 minutes · **Jeu :** 15–20 minutes

## Objectif (*goal*)
Appuyer sur les deux boutons **dans l'ordre secret** pour « ouvrir » le cadenas : la lumière rouge s'éteint et la **verte** s'allume. Une erreur, et tout recommence ! L'enfant découvre qu'un ordinateur suit une **séquence exacte** (*exact sequence*) : l'ordre compte.

## Matériel
- 3 DEL (rouge, jaune, verte) + 3 résistances de 220 Ω
- 2 boutons-poussoirs

## Montage (~10 minutes)
```
 D4 ─[220 Ω]─ DEL ROUGE  (fermé / locked)   ─┐
 D5 ─[220 Ω]─ DEL JAUNE  (bon appui)         ─┼── GND
 D6 ─[220 Ω]─ DEL VERTE  (ouvert / unlocked) ─┘

 BOUTON 1 : D2 ─┤├─ GND        BOUTON 2 : D3 ─┤├─ GND
```
1. Débrancher l'USB.
2. Placer les DEL **rouge** (D4), **jaune** (D5) et **verte** (D6), chacune avec sa résistance.
3. Bouton 1 entre **D2** et **GND**, bouton 2 entre **D3** et **GND**.
4. Le parent vérifie, puis on branche l'USB et on ouvre le **Moniteur série** (9600 bauds).

## Téléverser (*upload*)
Ouvrir `05-secret-code-lock.ino` et **Téléverser** ([guide général](../README.md#téléverser-un-programme-upload)).

## Comment ça marche
- **Pour le parent :** le code secret est la liste `SECRET = {1, 1, 2, 1, 2}`. La variable `step` compte les bons appuis. Un bon bouton fait `step++` et un clignotement jaune ; un mauvais bouton remet `step` à 0 et fait clignoter le rouge. Quand `step` atteint la longueur du code, le cadenas s'ouvre 3 secondes, puis se referme.
- **Pour l'enfant :** « Le cadenas se souvient d'une petite chanson de boutons. Il faut la jouer exactement dans l'ordre, sinon il dit “oups !” et on recommence. »

## Essaie ça ! (*try this*)
1. **Trouve le code :** le parent change le code en secret. L'enfant cherche en essayant : le jaune veut dire « bon, continue ! », le rouge « on recommence ».
2. **Mon propre code :** l'enfant invente son code, par exemple `{2, 2, 1}`. Le parent l'écrit dans `SECRET` et essaie de le deviner.
3. **Code plus long, plus difficile :** 7 appuis au lieu de 5.
4. **Le cadenas reste ouvert plus longtemps :** changer `delay(3000)` en `10000`.

## Sécurité
- Chaque DEL a sa résistance.
- Seulement le câble USB. On débranche avant de changer des fils.
- Petites pièces : pas dans la bouche.
