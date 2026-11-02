# Projet - Résultats

Auteur : Simon Boucher

## Étape 1


### Justification de configuration

Inscrivez la configuration appliquée et la raison de ce choix.

#### ADC

* Fin de conversion: EOC flag at the end of all conversions => Lorsque les 10 échantillons sont numérisés et déplacés, l’ADC déclenche une interruption.
* Nombre de conversion: 10 => L’ADC acquisitionne une rafale (la plus rapide possible) de 10 échantillons consécutifs.
* Rank: pour chaque _rank_ 
  * Canal: 1 => c'est le seul qui est connecté au potentiomètre.
  * Durée d'échantillonnage: 3 cycles => L’ADC acquisitionne une rafale (la plus rapide possible).

##### DMA

* Incrément de l'adresse du périphérique: désactivé => le registre de conversion ADC est unique et ne peut recevoir qu'un seul résultat.
* Incrément de l'adresse mémoire: activé => les résultats ADC sont déplacés dans un tableau de résultats afin d'être tous conservés et traités.
* Largeur de la donnée: demi mot (16 bits) => la résolution de conversion est de 12 bits, alors un espace mémoire de 16 bits est requis. 

### Analyseur logique

* Tension aux bornes du potentiomètre:
  * La tension la plus élevée (V): 4.902
  * La plage de tensions qui produit un résultat de 4095 (V): [3.371,3.924] => PS: cette mesure n'a pas été demandée, mais aurait dû l'être.
* Période d'expiration du timer (ms): 10


## Étape 2

### Justification de configuration

Inscrivez la configuration appliquée et la raison de ce choix (demeurez concis).

#### I2C

* Broche SCL: PB8 => connecté à la broche D15 du connecteur Arduino qui est connecté à la broche SCL du connecteur LCD de Freenove. 
* Broche SDA: PB9 => connecté à la broche D14 du connecteur Arduino qui est connecté à la broche SDA du connecteur LCD de Freenove.
* Configuration des broches: Pull-up => requis électronique du bus I2C.

### Compréhension

* Cause de la latence du rafraîchissement de l'affichage à la console: Les délais bloquants de l'exemple du pilote LCD.

* Durée de la latence (secondes): 10

### Analyseur logique

* Fréquence de l'horloge I2C (kHz): 100