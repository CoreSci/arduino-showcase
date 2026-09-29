# Leçon 4 — Le tir à la corde *(Button tug-of-war)*

**Âge :** 6–8 ans, deux joueurs (enfant + parent, ou deux enfants) · **Montage :** ~10 minutes · **Jeu :** autant qu'on veut !

## Objectif (*goal*)
Un jeu à deux : chaque joueur appuie **le plus vite possible** sur son bouton. Chaque appui tire la « corde » vers lui. Le premier à tirer la corde jusqu'au bout **gagne**, avec une petite musique ! L'enfant découvre le **pointage** (*score*) : un nombre qui monte et qui descend, qu'on compare à un but.

## Matériel
- 4 DEL + 4 résistances de 220 Ω
- 2 boutons-poussoirs (*push-buttons*)
- 1 buzzer piézo passif

## Montage (~10 minutes)
```
  Joueur 1                                           Joueur 2
  BOUTON1 (D2─┤├─GND)                        BOUTON2 (D3─┤├─GND)

  D4 ─[220 Ω]─ DEL1   D5 ─[220 Ω]─ DEL2   D6 ─[220 Ω]─ DEL3   D7 ─[220 Ω]─ DEL4
                          (le milieu = DEL2 + DEL3)
  (patte courte de chaque DEL → GND)

  D8 ── (+)BUZZER(−) ── GND
```
1. Débrancher l'USB.
2. Placer les 4 DEL en ligne : c'est la **corde**. Le joueur 1 s'assoit du côté de la DEL 1, le joueur 2 du côté de la DEL 4.
3. Bouton 1 entre **D2** et **GND**, bouton 2 entre **D3** et **GND**. Pas besoin de résistance pour les boutons.
4. Buzzer (+) sur **D8**, (−) sur **GND**.
5. Le parent vérifie, puis on branche l'USB.

## Téléverser (*upload*)
Ouvrir `04-button-tug-of-war.ino` et **Téléverser** ([guide général](../README.md#téléverser-un-programme-upload)).

## Comment ça marche
- **Pour le parent :** la variable `rope` commence à 0. Chaque **nouvel** appui du joueur 1 fait `rope--`, et du joueur 2 `rope++`. Garder le bouton enfoncé ne compte qu'une fois : le programme compare l'état actuel à l'état précédent. Les DEL montrent où est la corde. À `-10` ou `+10`, `celebrate()` joue do-mi-sol-do et fait clignoter le côté du gagnant, puis une nouvelle partie commence.
- **Pour l'enfant :** « Chaque fois que tu appuies, tu tires un petit coup sur la corde. Si ton ami tire plus vite, la corde glisse vers lui ! »

## Essaie ça ! (*try this*)
1. **Une partie plus longue :** changer `WIN_AT = 10` en `20`.
2. **Handicap pour le parent :** le parent appuie avec un seul doigt… de la main gauche !
3. **Nouvelle musique :** changer les notes dans `melody` (par exemple 784, 659, 523, 392 pour une musique qui descend).
4. **Sans regarder :** jouer les yeux fermés, seulement avec les sons (clic grave = joueur 1, clic aigu = joueur 2).

## Sécurité
- On appuie avec les doigts, pas avec des objets pointus : les boutons et les fils se détachent facilement.
- Le buzzer ne se colle pas contre l'oreille.
- Seulement le câble USB. On débranche avant de changer des fils.
