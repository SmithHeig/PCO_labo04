# PCO Labo 4 - Train

## Init

- Utilisation de la maquette A
- Langague C++

## Conception

- Deux trains
  - Train "évitement": Si arrive en second sur le trançon commun, passe par le tronçon d'évitement
  - Train "arrêt"; Si arrive en second sur le trançon commun, s'arrête

### Priorités

- Si le train "évitement" à la priorité: L'autre train s'arrête et fait plus rien jusqu'a changement de priorité
- Si le trian "arrêt" à la priorité: L'autre train prendra toujours la voie d'évitemment
- Priorité par défaut: le tronçon est reservé par le premier qui arrive et le second soit s'arrête si c'est le train "arrêt" ou prend l'évitemment si c'est le train "évitement"

## Parcours

1. Train "arrêt": *16 << 15 << 14 << 7 << **6 << 5 <<  34 << 33** << 32 << 25 << 24*
2. Train "évitement": *13 << 12 << 11 << 10 << 4 << **6 << 5 << 34 << 33** << 28 << 22 << 21 << 20 << 19*

#### Tronçon partagé

*6 << 5 << 34 << 33*

#### Voie d'évitement

*4 << 3 << 2 << 1 << 30 << 29 << 28*

#### Point de décisions

Les points de décisions sont les moment ou les trains vont vérifié si le tronçon est occupé et donc s'arrêter ou le reservé pour eux. Ils sont en avance pour avoir le temps de frainer pour s'arrêter

1. Train "arrêt": *15* et *24* 
2. Train "échappement": *11* et *21*

<!--A tester et modifier-->

#### Representation graphique

<!-- A faire -->





