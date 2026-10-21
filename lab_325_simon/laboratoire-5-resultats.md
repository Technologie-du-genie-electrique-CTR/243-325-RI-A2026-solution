# Laboratoire 5 - Résultats

Auteur : Simon Boucher

## Procédure1

### Mesures

* Rapport cyclique DEL (%):~50%
* Rapport cyclique A3 (%):~0,15
* Rapport cyclique A4 (%):~0
* Durée du bloc d'instruction A3 (ms):~1,4844
* Durée du bloc d'instruction A4 (ms):~0,0153
* Durée du délai (ms):~1000,55
* Durée de l'affichage (ms):~1,4659 (ce résultat dépend du nombre de caractères affichés)


## Procédure2

### Mesures

* Durée du délai (ms):1086,10
* Durée de l'appui du bouton (ms):87,55 (ça dépend de la durée d'appui de chacun)


## Procédure3

### Mesures

* Durée du délai (ms):~0,0031
* Durée de l'appui du bouton (ms):130,97 (ça dépend de la durée d'appui de chacun)


## Procédure4

### Mesures

* Rapport cyclique A3 (%):~99,83
* Durée du bloc d'instruction A3 (ms):~1,8072
* Durée du délai (ms):0,0031
* Durée de l'ISR HAL_TIM_PeriodElapsedCallback (ms):0,0077


## Procédure5

### Mesures

Aucune mesure à prendre puisque cette procédure est très similaire à la 6ième qui présente un intérêt supérieur.


## Procédure6

### Mesures

* Rapport cyclique A3 (%):~99,83
* Durée du bloc d'instruction A3 (ms):~1,8022
* Durée du délai (ms):~0,003
* Durée de l'ISR HAL_TIM_PeriodElapsedCallback (ms):~0,0044


## Procédure7

### Mesures

* Rapport cyclique A3 (%):~0,13
* Durée du bloc d'instruction A3 (ms):~1,349
* Durée du délai (ms):~999,63
* Durée de l'ISR HAL_TIM_PeriodElapsedCallback (ms):~0,0044


## Synthèse

**Observez et expliquez la différence de durée de la routine d'interruption de la procédure 6 par rapport à celle de la procédure 4.**

L'ISR P4 exécute toutes les lignes d'instructions correspondantes au traitement à effectuer lorsque l'évènement d'expiration du timer survient.

L'ISR P6 n'exécute que la levée d'un indicateur évènementiel lorsque l'évènement d'expiration du timer survient. Le fil d'exécution principal détecte que l'indicateur est levé et exécute toutes les lignes d'instructions correspondantes au traitement à effectuer lors de cet évènement.

L'ISR est un fil d'exécution temporaire qui interrompt le fil d'exécution principal.


**Si le traitement à effectuer, lorsqu'une interruption du timer survient, était beaucoup plus complexe, que ce passerait-il par rapport au ISR des procédure 4 et 6?**

L'ISR P4 pourrait durer encore plus longtemps si le traitement à effectuer était plus complexe contraitement à l'ISR P6 qui demeure minimaliste en déportant le traitement dans le fil d'exécution principal.

Dans le cas d'un micrologiciel complexe devant gérer plusieurs évènements, un traitement trop complexe exécuté dans un ISR pourrait retarder le traitement d'interruption, manquer des interruptions et alors fournir des résultats erronés en compromettant l'aspect temps réel du système.


**Observez et expliquez la différence entre les signaux périodiques de durée du bloc d'instruction de la boucle TANT QUE capturé par le canal A3 de la procédure 7 par rappot à la procédure 6.**

Le signal A3 de P6 présente un rapport cyclique de plus de 99%, ce qui signifie que l'UCT (CPU) exécute sans arrêt des instructions. Le faible délai observé de fin de boucle n'est que l'exécution d'une instruction TANT QUE. Les instructions exécutées sont la vérification des indicateurs d'évènements.

Le signal A3 de P7 présente un rapport cyclique de moins de 1%, ce qui signifie que l'UCT est en dormance plus de 99% du temps. La vérification des indicateurs d'évènements n'est exécutée que lorsqu'un évènement survient.


**Quel est l'avantage de la procédure 7 sur la procédure 6?**

La procédure 7 permet à l'UCT d'être inactif 99% du temps, permettant une économie d'énergie de près de 99% de l'énergie consommée par P6. Une telle économie d'énergie permettrait à un dispositif électronique alimenté par pile ou batterie d'offrir une autonomie (durée de fonctionnement) largement supérieure ou d'être alimenté par une plus petite pile ou batterie et panneau solaire.

La procédure 7 réduit de beaucoup l'usure du microcontrôleur et lui permettrait une durée de vie utile supérieure à celle qu'offrirait P6.