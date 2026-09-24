# Mood Detector

Plateforme de vote sur ESP32 pour recueillir un mood par equipe et consulter
les resultats depuis une interface web locale.

## Demarrage rapide

```bash
$HOME/.platformio/penv/bin/pio run
$HOME/.platformio/penv/bin/pio test -e native
```

Le firmware est genere dans `.pio/build/esp32dev/firmware.bin`.

## Utilisation

1. Alimenter l'ESP32 en USB.
2. Se connecter au Wi-Fi `Mood1` avec le mot de passe configure.
3. Ouvrir `http://192.168.4.1`.
4. Selectionner une equipe sur le clavier, puis appuyer sur `A`, `B` ou `C`.

Le manuel complet est disponible dans [docs/manuel-utilisateur.md](docs/manuel-utilisateur.md).

## Documentation

- [Manuel utilisateur](docs/manuel-utilisateur.md)
- [Schema de cablage](docs/schema-cablage.md)
- [Documentation technique](docs/documentation-technique.md)
- [Documentation materiel](Hardware/Readme.md)
- [Tests](test/README)

## Materiel

- ESP32 Dev Module 30 broches
- RTC DS3231
- clavier matriciel 4x4
- trois LEDs ou boutons lumineux de retour
- alimentation USB 5 V

Le tableau complet des broches et les controles avant mise sous tension sont
dans [docs/schema-cablage.md](docs/schema-cablage.md).

## Securite

Les identifiants du depot sont des valeurs de prototype. Modifier
`include/AppConfig.h` avant toute utilisation partagee. Le serveur OTA sur le
port `8080` est protege par authentification ; le serveur de consultation sur
le port `80` reste local mais ne demande pas encore d'authentification.
# mood-detector