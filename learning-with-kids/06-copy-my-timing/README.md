# Leçon 6 — Copie mon temps ! *(Copy my timing)*

**Âge :** 6–8 ans, avec un parent · **Montage :** ~10 minutes · **Jeu :** 15 minutes

## Objectif (*goal*)
Tenir le **bouton 1** aussi longtemps qu'on veut : la lumière reste allumée pendant qu'on le tient. Puis appuyer sur le **bouton 2** : la lumière **copie exactement** le même temps ! L'enfant découvre que l'Arduino a un **chronomètre** (*stopwatch*, `millis()`) et une **mémoire** (*memory*, une variable).

## Matériel
- 1 DEL + 1 résistance de 220 Ω
- 2 boutons-poussoirs

## Montage (~10 minutes)
```
 D4 ─[220 Ω]─(+)DEL(−)── GND

 BOUTON 1 (enregistrer / record) : D2 ─┤├─ GND
 BOUTON 2 (rejouer / play)       : D3 ─┤├─ GND
```
1. Débrancher l'USB.
2. DEL sur **D4** avec sa résistance.
3. Bouton 1 entre **D2** et **GND**, bouton 2 entre **D3** et **GND**.
4. Le parent vérifie, puis on branche l'USB et on ouvre le **Moniteur série** (9600 bauds) pour voir les secondes.

## Téléverser (*upload*)
Ouvrir `06-copy-my-timing.ino` et **Téléverser** ([guide général](../README.md#téléverser-un-programme-upload)).

## Comment ça marche
- **Pour le parent :** `millis()` donne le nombre de millisecondes depuis le démarrage. Quand le bouton 1 est enfoncé, on note l'heure de départ ; quand il est relâché, `recorded = millis() - start` est la durée. Le bouton 2 allume la DEL pendant `recorded` millisecondes (`delay(recorded)`). Le Moniteur série affiche la durée en secondes.
- **Pour l'enfant :** « L'Arduino regarde son chronomètre quand tu appuies et quand tu lâches. Il se souvient du temps, et il peut le refaire exactement pareil. »

## Essaie ça ! (*try this*)
1. **Le défi des 3 secondes :** tenir le bouton 1 exactement 3 secondes **sans compter à voix haute**. Le Moniteur série dit qui s'est approché le plus. Parent contre enfant !
2. **Le plus court possible :** qui fait le plus petit temps ? (Moins de 0,1 seconde, c'est très difficile !)
3. **La chanson de lumière :** faire une note longue et la faire rejouer. Est-ce vraiment identique ?
4. **Rejouer deux fois :** ajouter une deuxième copie dans le programme, avec une pause entre les deux.

## Sécurité
- Seulement le câble USB. On débranche avant de changer des fils.
- Petites pièces : pas dans la bouche.
