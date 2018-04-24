# **PCO Labo 4 -Train**

## **Init**

- Utilisation de la maquette A
- Langage C++

## **Conception**

Afin de pouvoir réaliser ce laboratoire, nous sommes partis sur avec 2 locomotives qui font chacune un parcours prédéfinit et une voie d'évitement fixe (cf: Parcours).La voie commune (section partagée), ne peut accueillir qu'un train à la fois:

- Si la loco 1 y est, la locomotive 2 doit passer par le chemin d'évitement.
- Si la loco 2 y est, la locomotive 1 doit s'arrêter jusqu’à ce que la voie soit à nouveau libre.

Les chemins des locomotives sont fixes.

Pour détecter quand une locomotive entre/sort de la voie commune, nous avons mis des "Listener" sur les 4 points d'entrées de celle-ci. Quand une locomotive passe sur un de ces points, une méthode est appelée pour gérer la section critique. La locomotive ne "connait" donc pas son chemin. C'est une classe qui s'occupe totalement du routage des locomotives.

### **Priorités:**

Pour la gestion des priorités, l'utilisateur peut enter 3 priorités :

- prio_0: enlève la notion de priorité.
- prio_1: donne la priorité à la loco 1, donc la loco 2 prends toujours la voie d'évitement
- prio_2: donne la priorité à la loco 2, donc la loco 1 s'arrête devant la voie commune jusqu'à un changement de priorité.

Comme l'entrée utilisateur est une méthode bloquante, nous avons créé une classe qui hérite de Qthread

## **Implémentations**

Nous avons premièrement créé une classe **ManageLoco**, cette classe a pour but d'initialiser, d'aiguiller les locomotives et traiterla section critique (en gros tout faire sauf l'entrée utilisateur). Elle gère seulement 2 locomotives (passées par références dans le constructeur).

Elle possède une classe interne **LocoListener **qui a pour but d'écouter en permane des points de contacts (les "Listeners") prédéfinis (cf: points de contacts). Cette classe est donc une classe "threadée". Il y a une instance par contacte, et chaque fois qu’un loco passes dessus, une méthode (**ManageLoco.traiterPointLoco()**) est appelée avec le numéros de la loco et du contacte correspondant. Vue que les contacts ne sont pas partagés par les locomotives, il n'y a pas besoin de contrôler si c'est bel et bien la bonne locomotive qui a actionné le contacte. La fonction **attendre_contact()** est utilisée à l'intérieur de la méthode **run()** pour éviter de faire de l'attente active et se réveiller seulement quand une loco passe dessus le contacte. Un **sleep(1)** est effectué après chacun des passages ne pas que les instruction soient appelés plusieurs fois pour 1seulpassage (1 passage = 1 appelle); Ce problème est survenu sur la maquette. Ces threads sont détruits brutalement a la destruction de la classe **ManageLoco**,il n'y a pas besoin d'attendre qu'ils aient fini leur travaille avant de les kill car il ne finiront théoriquement jamais d'écouter. Nous avons décidé de faire une classe interne car on croyait qu'elles avaient le même fonctionnement qu'en Java (elles gardent une référence sur la classe parent qui simplifiel'appel de fonction.) On a donc du garder une référence sur la classe **ManageLoco **depuis **LocoListener** pour pouvoir appeler **traiterPointLoco()**.

Après avoir décrit la classe **LocoListener**, nous allons expliquer la méthode **traiterPointLoco()** de la classe **ManageLoco**. Cette méthode prend en paramètre la loco à traiter et le point de contact (théoriquement celui sur lequel est la loco). Cette méthode traite tous les différentes gestions des points de contacts (cf: point de contact) à l'aide d'un switch:

- Les premiers cas sont quand une locomotive veut changer de sens après 2tours complets. A chaque fois que la locomotive passe ce point, un compteur est incémenté et quand il arrive à 3, la loco change de sens (compteur remis à 0). Le compteur va jusqu'à 3 car la loco repasse directement dessus après avoir changé de sens sans avoir fait un tour.

- Les seconds cas sont quand une locomotive veut entrer/sortie en section critique. Le sens de la loco définit si elle veut entrer ou si elle en sort. Quand la loco *entre en section critique*, la méthode **entreSectionCritique **est appelée ; Il faut premièrement contrôler si la section est libre. Si ce n'est pas le cas, elle se stoppe (loco1) ou prends le chemin d'évitement (loco2) dans tous les cas. Si elle est libre, trois possibilité s'offrent en fonction de la priorité choisie(prio_1-3):

  - prio_0: Les 2 locos peuvent enter en section critique sans problème. Leur chemin de section critique est mis en place et elle continue leur chemin.
  - prio_1: Si c'est la loco 1 qui entre, son chemin critique est mis en place et elle continue. Cependant, si c'est la loco2, le chemin d'évitement est mis en place.
  - prio_2 Si c'est la loco 1 qui entre, elle s'arrête. Et si c'est la loco2 son chemin critique est mis en place et elle continue.

  Quand une loco *sort de section critique*, deux possibilités s'offre à nous: 

  - La loco 1 sort de section critique : Elle informe juste que la section critique est disponible et ne fait rien d'autre car les voies seront toujours ajustées correctement pour la loco 2 (peu importe la priorité).
  - La loco 2 sort de section critique : Si la loco 1 est bloquée et que la priorité n'est pas la seconde (prio_2, locomotive 1 s'arrête), la loco 1 est redémarrée. La section n'est pas rendue disponible car la loco 1 y rentre.

Pour la gestion des entrées utilisateurs, nous avons implémenté la classe **messageListener** qui fonctionne comme la classe **LocoListener**. Vu que la fonction **getCommand()** est bloquante, ne fallait pas bloquer l'exécution du programme. Donc nous avons créé un thread qui attends en permanence une commande utilisateur. Une fois une commande reçue, la méthode **analyseCommand** est appelée pour taiter la commande et changer la priorité des locos en appelant la méthode **setPrio() **de la classe **ManageLoco**.

#### Mutex et accès concurent

Dans notre classe **ManageLoco**, nous avons décidé de vérouiller avec un mutex toutes les actions sur une ressource partagée. Nous avons fait cela pour grantire l'intégrité des variables. C'est aussi pour garantire qu'une instruction se fasse en entier sans qu'une autre commence et change les variables en cours de modification soient changées.

## **Parcours**

Les deux locomotives ont un parcours prédéfinit. Tous les parcours sont enregistrés "en dur" dans la classe **ManageLoco**.

1. Locomotive 1"stop": *16 << 15 << 14 << 7 << \***6<< 5 << 34 << 33** << 32 << 25 << 24.*
2. Locomotive 2"évitement": *13 << 12 << 11 << 10 << 4<< \***6 << 5 << 34 << 33** << 28 << 22<< 21 << 20 << 19*

Voie commune : **6 << 5 << 34 <<33**

Voie d'évitement pour la locomotive 2 : **29<< 30 << 31 << 1 << 2 << 3 **

Il y a 4 méthodes qui Initialise les voies:

- **init()**: Initialise les parcours initiaux des2 locomotives.
- **setCritLoco1()**: Initialise la voie commune pour la loco 1.
- **setCritLoco2()**: Initialise la voie commune pour la loco 2.
- **setEvitement()**: Initialise la voie d'évitement pour la loco 2.

Mis à part la méthode **init()** qui est appelée dans le constructeur de **ManageLoco**, toutes ces autres méthodes sont appelées quand la loco correspondante passe le contacte pour entrer en section critique.

## **Points de contact**

Il y a deux sortes de points de contact:

- Les points de contacts qui servent d'entrées/sorties de section partagée:
  -  *25 et 14 pour laloco1.*
  -  *10 et 22 pour laloco2.*
- Les points de contacts qui servent à compter quand une loco fait un tour complet et à inverser le sens 
  - *23 pour la loco1*.
  - *19 pour la loco2.*

## Maquette

Le programme fonctionne sur la maquette, il faut juste faire attention à ne pas mettre une vitesse trop haute. Sinon les aiguillages n'ont pas le temps de se mettre à jour ou la locomotive ne s'arrête pas assez vite.

 ## Fichier de configuration (config.h)

Le fichier config.h, contient les différentes constantes utilisé pour la configuration des trains et de la maquette.

On y retrouve:

- Les points de décision de la section critique
- Les points de fin de tour
- Les priorités
- Le numéro des locomotives
- La vitesse des locomotives