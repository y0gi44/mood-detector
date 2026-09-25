# Documentation technique

## Architecture

```text
src/main.cpp
    |
    +-- Mood
         +-- Votes / VoteItem       Comptage et persistance des votes
         +-- Keyboard               Lecture du clavier matriciel
         +-- Affichage               Sequences LED non bloquantes
         +-- RTC DS3231              Date et heure
         +-- WiFiServer:80            Interface web et CSV
         +-- WebServer:8080           Sauvegarde, restauration et OTA
         +-- Preferences             Persistance flash ESP32
```

`Mood` est l'application active. La variante historique `Creneau` reste dans le depot mais est exclue du build courant par `lib_ignore = Creneau`.

## Build

```bash
$HOME/.platformio/penv/bin/pio run
```

Le binaire est genere dans :

```text
.pio/build/esp32dev/firmware.bin
```

## Tests

Les tests de logique de vote sont independants du materiel :

```bash
$HOME/.platformio/penv/bin/pio test -e native
```

Ils verifient :

- les moods 1, 2 et 3 ;
- les bornes de candidats ;
- la serialisation et la restauration ;
- le rejet de donnees invalides.

## Configuration

Les valeurs de prototype sont centralisees dans `include/AppConfig.h` :

- SSID du point d'acces ;
- mot de passe Wi-Fi ;
- utilisateur OTA ;
- mot de passe OTA.

Modifier ce fichier avant toute installation partagee.

## Contrats de donnees

Un element de vote est serialise sous la forme :

```text
mood1;mood2;mood3;
```

La collection est serialisee avec `|` entre les candidats :

```text
mood1;mood2;mood3;|mood1;mood2;mood3;|
```

Les compteurs sont des `uint16_t` et sont satures a `65535`. Les donnees invalides sont ignorees et l'element concerne est remis a zero.

## Cycle principal

La boucle Arduino execute successivement :

1. lecture d'une requete HTTP brute ;
2. lecture du clavier ;
3. traitement d'un vote ;
4. progression d'une animation LED ;
5. traitement du serveur OTA.

Le serveur HTTP brut dispose d'un timeout d'inactivite de deux secondes et limite la taille des en-tetes a 1024 octets.

## Ports reseau

| Port | Service | Authentification |
|---:|---|---|
| 80 | page web, date, resultats, CSV, reset (avec confirmation) | Basic Auth |
| 8080 | sauvegarde, restauration, reset, OTA | Basic Auth |

## Points de vigilance

- Les deux serveurs HTTP demandent les identifiants de `include/AppConfig.h`.
- Les identifiants sont stockes dans le firmware et doivent etre changes avant production.
- L'ESP32 doit etre alimente en 3.3 V sur ses GPIO.
- Une validation sur materiel reel reste necessaire pour les timings du clavier, les LEDs et le RTC.
