# Leçon 1 — Bonjour, lumière ! *(Hello, light!)*

**Âge :** 6–8 ans, avec un parent · **Montage :** ~10 minutes · **Jeu :** 10–15 minutes

## Objectif (*goal*)
Faire **clignoter une lumière** (*blink an LED*), puis changer sa vitesse. L'enfant découvre qu'un **programme** est une liste d'étapes que l'ordinateur suit dans l'ordre, encore et encore : c'est la **boucle** (*loop*).

## Matériel
- Arduino Uno + câble USB, platine d'essai (*breadboard*)
- 1 DEL (*LED*) rouge + 1 résistance de 220 Ω
- 2 fils de raccordement

## Montage (~10 minutes)
```
 Arduino D4 ───[ 220 Ω ]───(+)DEL(−)─── GND
                           patte     patte
                           longue    courte
```
1. Débrancher l'USB.
2. Planter la DEL dans la platine. Sa **patte longue** est le (+).
3. Relier **D4** à la résistance, et la résistance à la patte longue.
4. Relier la **patte courte** à **GND**.
5. Le parent vérifie, puis on branche l'USB.

## Téléverser (*upload*)
Ouvrir `01-hello-light.ino` dans l'Arduino IDE et cliquer sur **Téléverser**. Les étapes détaillées sont dans le [guide général](../README.md#téléverser-un-programme-upload).

## Comment ça marche
- **Pour le parent :** `setup()` s'exécute une fois au démarrage. `loop()` se répète à l'infini : allumer (`HIGH`), attendre `blinkTime` millisecondes, éteindre (`LOW`), attendre. 1000 ms = 1 seconde.
- **Pour l'enfant :** « L'Arduino lit ses instructions comme une recette : allume, attends, éteins, attends… et quand il arrive à la fin, il recommence depuis le début ! »

## Essaie ça ! (*try this*)
1. **Plus vite !** Changer `blinkTime = 1000` en `200`. Téléverser. Que se passe-t-il ?
2. **Plus lent…** Essayer `3000`. Compter à voix haute pendant que la lumière reste allumée.
3. **Le cœur qui bat :** allumer 100 ms, éteindre 100 ms, rallumer 100 ms, puis éteindre 700 ms. (Astuce : ajouter des lignes dans `loop()`.)
4. **Devine le nombre :** le parent choisit un `blinkTime` en secret ; l'enfant compte et devine combien de secondes.

## Sécurité
- La DEL a **toujours** sa résistance, sinon elle peut chauffer.
- On débranche l'USB avant de changer un fil.
- Petites pièces : pas dans la bouche.

*Inspiré de l'exemple « Blink » de l'Arduino (domaine public).*
