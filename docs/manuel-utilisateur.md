# Manuel utilisateur

## 1. Demarrage

1. Brancher l'ESP32 sur une alimentation USB 5 V.
2. Attendre la sequence de demarrage des LEDs.
3. Depuis un telephone ou un ordinateur, ouvrir la liste des reseaux Wi-Fi.
4. Se connecter au point d'acces :

   - SSID : `Mood1`
   - Mot de passe : `123456789`

5. Ouvrir `http://192.168.4.1` dans un navigateur et saisir les identifiants :
   - utilisateur : `admin` ;
   - mot de passe : `123456789`.

L'adresse IP est affichee sur le port serie au demarrage. Le nom `mood-detector.local` n'est pas configure par le firmware actuel et ne doit donc pas etre utilise comme adresse de reference.

## 2. Enregistrer un vote

Sur la borne :

1. Selectionner une equipe avec une touche numerique.
2. Appuyer sur un bouton de mood :
   - `A` : mood 1, affiche en vert dans la version `Mood` actuelle ;
   - `B` : mood 2, affiche en jaune ;
   - `C` : mood 3, affiche en rouge.
3. La LED confirme la prise en compte du vote.

Dans la version `Mood`, les touches numeriques `1` a `9` selectionnent les equipes correspondantes. Le clavier matriciel ne possede pas de touche `0` dediee : l'equipe 0 n'a pas de touche physique explicite dans la configuration actuelle.

Un mood appuye sans equipe selectionnee est refuse et declenche la sequence d'erreur.

## 3. Consulter les resultats

Depuis `http://192.168.4.1` :

- la date et l'heure du RTC sont affichees ;
- la duree depuis le demarrage est affichee ;
- le tableau des votes affiche les trois moods par equipe ;
- les totaux sont affiches en bas du tableau ;
- le lien de telechargement exporte un CSV.

Le serveur principal utilise le port HTTP `80`.

## 4. Sauvegarder et restaurer

Les actions de sauvegarde et de restauration sont aussi disponibles sur le serveur OTA :

- `http://192.168.4.1:8080/save`
- `http://192.168.4.1:8080/restore`
- `http://192.168.4.1:8080/reset`
- `http://192.168.4.1:8080/download`

Identifiants actuels :

- utilisateur : `admin`
- mot de passe : `123456789`

`reset` efface la sauvegarde et remet aussi les votes en memoire a zero. Les votes sauvegardes sont automatiquement restaures au redemarrage.

Le bouton "Reinitialiser" de la page principale (`http://192.168.4.1/reset`) mene a une page de confirmation avant d'effacer les votes, pour eviter un reset accidentel.

## 5. Regler l'heure

Sur la page principale, utiliser le formulaire avec le format strict :

```text
YYYYMMDDHHMMSS
```

Exemple : `20260924143000`.

Une date invalide est refusee avec une reponse HTTP `400`.

## 6. Mise a jour OTA

1. Se connecter au Wi-Fi `Mood1`.
2. Ouvrir `http://192.168.4.1:8080/update`.
3. S'authentifier avec les identifiants OTA.
4. Televerser le firmware genere par PlatformIO.

Le fichier attendu est generalement :

```text
.pio/build/esp32dev/firmware.bin
```

Ne pas couper l'alimentation pendant la mise a jour.

## 7. Depannage

### Le reseau Wi-Fi n'apparait pas

- verifier l'alimentation USB ;
- redemarrer l'ESP32 ;
- verifier le port serie a `115200` bauds.

### La page ne repond pas

- verifier que l'appareil est connecte a `Mood1` ;
- utiliser `http://192.168.4.1`, sans HTTPS ;
- verifier que le port `80` n'est pas remplace par `8080`.

### Les votes ne sont pas conserves

- utiliser `/save` avant de couper l'alimentation ;
- verifier que `Preferences` s'ouvre correctement dans le port serie ;
- utiliser `/restore` pour recharger explicitement la sauvegarde.

## 8. Securite

Les identifiants actuels sont des valeurs de prototype. Avant une utilisation reelle :

- modifier les valeurs dans `include/AppConfig.h` ;
- ne pas partager le mot de passe ;
- ne pas exposer le point d'acces a un reseau non controle ;
- le serveur principal et le serveur OTA utilisent tous les deux Basic Auth ;
   modifier les identifiants avant une utilisation reelle.
