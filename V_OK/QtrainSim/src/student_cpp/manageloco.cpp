/**
* @file manageloco.cpp
* @authors Jeremie Chatillon et James Smith
*/

#include "manageloco.h"
#include "loco.h"

/*
Locomotive* ManageLoco::locomotive1;
Locomotive* ManageLoco::locomotive2;

QMutex ManageLoco::mutex;
bool ManageLoco::critSection;
bool ManageLoco::locoCritSection[2];
bool ManageLoco::locoWait;

int ManageLoco::tourL1;
int ManageLoco::tourL2;
*/

/**
 * @brief ManageLoco::ManageLoco
 * @param l1 - locomotive 1 à gérer
 * @param l2 - locomotive 2 à gérer
 * @remark constructeur avec paramètre
 */
ManageLoco::ManageLoco(Locomotive& l1, Locomotive& l2)
{
    // Conservation d'un référence sur les locomotives à gérer
    locomotive1 = &l1;
    locomotive2 = &l2;

    // Initalise les aiguillages
    init();

    // Initalise la loco 1
    locomotive1->fixerNumero(3);
    //locomotive1->fixerVitesse(5);         // Vitesse sur maquette
    locomotive1->fixerVitesse(10);
    locomotive1->fixerPosition(16, 23);
    locomotive1->allumerPhares();
    locomotive1->afficherMessage("Ready!");

    // Initalise la loco 2
    locomotive2->fixerNumero(13);
    //locomotive2->fixerVitesse(9);         // Vitesse sur maquette
    locomotive2->fixerVitesse(10);
    locomotive2->fixerPosition(13, 19);
    locomotive2->allumerPhares();
    locomotive2->inverserSens();
    locomotive2->afficherMessage("Ready!");


    // Initalise la section critique
    locoCritSection[0] = false;
    locoCritSection[1] = false;
    critSection = false;
    locoWait = false;



    // Commence à 1 pour le premier tour
    tourL1 = 1;
    tourL2 = 1;

    // Pas de prio au départ
    prio = 0;

    // Ajoute les listener sur les points d'entrées et sorties de la section critique
    critiquePointsList.append(new LocoListener(14, locomotive1->getID(), *this));
    critiquePointsList.append(new LocoListener(10, locomotive2->getID(), *this));
    critiquePointsList.append(new LocoListener(25, locomotive1->getID(), *this));
    critiquePointsList.append(new LocoListener(22, locomotive2->getID(), *this));

    // Ajoute les listenenr sur le points pour inverser le sens de la loco
    critiquePointsList.append(new LocoListener(19, locomotive2->getID(), *this));     // Inverser sens
    critiquePointsList.append(new LocoListener(23, locomotive1->getID(), *this));     // Inverser sens

    // Démarre les threads
    for(auto i = critiquePointsList.begin(); i != critiquePointsList.end(); ++i)
        (*i)->start();

    // Démarre les loco
    locomotive2->demarrer();
    locomotive1->demarrer();
}

ManageLoco::~ManageLoco(){
    /*for(auto i = critiquePointsList.begin(); i != critiquePointsList.end(); ++i)
        (*i)->terminate();
        */
}

/**
 * @brief ManageLoco::setPrio
 * @param p
 * @remark change la priorité des trains
 *         prio 0: priorité normal, le premier dedans reserve la section critique
 *         prio 1: la locomotive 1 à la priorité et la 2 passe toujours par l'évitement
 *         prio 2: la locomotive 2 à la priorité et la 1 s'arrête et attends un changement de priorité
 */
void ManageLoco::setPrio(int p){
    this->prio = p;
    mutex.lock();
    if(locoWait && !critSection){
        locoCritSection[0] = true;
        locoWait = false;
        setCritLoco1();
        locomotive1->demarrer();
    }
    mutex.unlock();
}

/**
 * @brief ManageLoco::entreSectionCritique
 * @param idLoco - id de la locomotive entrant dans la section critique
 * @remark gestion de la section critique selon locomotive, prio et l'autre locomotive
 */
void ManageLoco::entreSectionCritique(int idLoco){
    // On veut que toutes ces opértions soient atomiques  ce sont des variables
    //  partagées dans la classe et on ne veut pas qu'elles cahngent de valeur
    //  pendant le traitement
    mutex.lock();
    // Traitement similaire pour toutes les priorités quand une loco est en
    //  section critique. Elle doit tout d'abord en sortir.
    if(critSection){    // Secrtion crtique occupée, la loco attends ou évite
        if(idLoco == 1){
           locoWait = true;
            locomotive1->arreter();
        }
        if(idLoco == 2){
            setEvitement();
        }
    } else {            // section critique libre => la loco vas y rentrer
        switch(prio){   // Diferents cas en fonction de la priorités
        case 0:     // Pas de prio, usage normal
            locoCritSection[idLoco -1] = true;
            critSection = true;
            if(idLoco == 1)
                setCritLoco1();
            if(idLoco == 2)
                setCritLoco2();
            break;
        case 1:     // prio pour loco 1
            if(idLoco == 1){    // La loco 1 entre en SC
                locoCritSection[0] = true;
                setCritLoco1();
                critSection = true;
            }
            if(idLoco == 2){    // la loco 2 evite tout les temps
                setEvitement();
            }
            break;

        case 2:     // prio pour loco 2
            if(idLoco == 1){     // La loco 1 s'arrête
                setCritLoco1();
                locoWait = true;
                locomotive1->arreter();
            }
            if(idLoco == 2){     // La loco 2 entre en SC
                setCritLoco2();
                critSection = true;
                locoCritSection[1] = true;
            }
            break;
        }
    }
    mutex.unlock();

}

/**
 * @brief ManageLoco::sortSectionCritique
 * @param idLoco - id de la locomotive sortant de la section critique
 * @remark gestion de la libération de la section critique
 */
void ManageLoco::sortSectionCritique(int idLoco){
    // On veut que toutes ces opértions soient atomiques  ce sont des variables
    //  partagées dans la classe et on ne veut pas qu'elles cahngent de valeur
    //  pendant le traitement
    mutex.lock();
    if(locoCritSection[idLoco-1] == true){ // Sort de criticSection
        // la loco 1 est arrêtée, redémarre si il n'y a pas la prio_2. Elle
        //   entre directement en sc (car elle attendait devant.
        if(locoWait == true && prio != 2){
            locoCritSection[0] = true;
            locoWait = false;
            setCritLoco1();
            locomotive1->demarrer();

        }else{
             critSection = false;
        }
        locoCritSection[idLoco-1] = false;
    }
    mutex.unlock();
}

/**
 * @brief ManageLoco::traiterPointLoco
 * @param pos - numéro du capteur passé
 * @param idLoco - locomotive ayant passé le capteur
 * @remark check si le capteur à une importance et quoi faire
 */
void ManageLoco::traiterPointLoco(int pos, int idLoco){

    // recupération local du sens (pour une utilisation ThreadSafe)
    bool sens;

    mutex.lock();       // bloque le temps
    // Récupère le sens de la loco et indique qu'elle a passé un contacte
    //  critique
    if(idLoco == 1){
        sens = locomotive1->getSens();
        afficher_message_loco(3, qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                            .arg(idLoco).arg(pos)));
    } else if(idLoco == 2){
        sens = locomotive2->getSens();
        afficher_message_loco(13, qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                            .arg(idLoco).arg(pos)));
    }
    mutex.unlock();

    // Traitement en fonction du point d'appelle
    switch(pos){
    case 19:            // Loco 2 à fait un tour complet
        mutex.lock();
        (++tourL2);
        if(tourL2 == 3){        // on contrôle avec 3 car il y a l'aller-retour
            locomotive2->arreter();
            locomotive2->inverserSens();
            locomotive2->demarrer();
            tourL2 = 0;
        }
        mutex.unlock();
        break;
    case 23:            // loco 1 fait un tour complet
        mutex.lock();
        (++tourL1);
        if(tourL1 == 3){        // on contrôle avec 3 car il y a l'aller-retour
            locomotive1->arreter();
            locomotive1->inverserSens();
            locomotive1->demarrer();
            tourL1 = 0;
        }
        mutex.unlock();
        break;

    case 14:
    case 10:
        if(sens){
            // pas besoin de mutex car méthode déjà protégée
            entreSectionCritique(idLoco);

        } else {
            // pas besoin de mutex car méthode déjà protégée
            sortSectionCritique(idLoco);
        }
        break;
    case 25:
    case 22:
        if(!sens){
            // pas besoin de mutex car méthode déjà protégée
            entreSectionCritique(idLoco);
        } else {
            // pas besoin de mutex car méthode déjà protégée
            sortSectionCritique(idLoco);
        }
        break;
    }
}

/**
 * @brief ManageLoco::LocoListener::LocoListener
 * @param pos - postion du capteur à écouter
 * @param idLoco - loco qui passe sur le capteur
 * @param it - manager à contacter lorsqu'il doit faire quelque chose
 * @remark constructeur
 */
ManageLoco::LocoListener::LocoListener(int pos, int idLoco, ManageLoco& it)
    :pos(pos), idLoco(idLoco), refThis(it)
{
 // do nothing
}

/**
 * @brief ManageLoco::LocoListener::run
 * @remark thread qui écoute les contacts et qui appelle le traitement selon la position
 */
void ManageLoco::LocoListener::run(){
    while(true){
        attendre_contact(pos);
        // pas besoin de mutex car méthode déjà protégée
        refThis.traiterPointLoco(pos, idLoco);

        sleep(1);       // Sleep nécessaire pour la maquette car la loco reste un moment sur le detecteur.

    };
}

/**
 * @brief ManageLoco::init
 * @remark Initialisation des aiguillages par rapport au parcours initial
 */
void ManageLoco::init(){
    diriger_aiguillage(1, TOUT_DROIT,  0);
    diriger_aiguillage(2,  DEVIE,       0);
    diriger_aiguillage(5,  TOUT_DROIT,       0);
    diriger_aiguillage(7, TOUT_DROIT,  0);
    diriger_aiguillage(8,  DEVIE,       0);
    diriger_aiguillage(9,  DEVIE,       0);
    diriger_aiguillage(10,  TOUT_DROIT,       0);
    diriger_aiguillage(11, TOUT_DROIT,  0);
    diriger_aiguillage(13, TOUT_DROIT,  0);
    diriger_aiguillage(14,  DEVIE,       0);
    diriger_aiguillage(15,  DEVIE,       0);
    diriger_aiguillage(16, TOUT_DROIT,  0);
    diriger_aiguillage(17, TOUT_DROIT,  0);
    diriger_aiguillage(21, DEVIE,  0);
    diriger_aiguillage(22, TOUT_DROIT,  0);
    diriger_aiguillage(23, TOUT_DROIT,  0);
}

/**
 * @brief ManageLoco::setCritLoco1
 * @remark set l'aiguillage lorsque la loco1 rentre dans la section critique
 */
void ManageLoco::setCritLoco1(){
   diriger_aiguillage(3,  DEVIE,       0);
   diriger_aiguillage(20,  DEVIE,       0);
}

/**
 * @brief ManageLoco::setCritLoco2
 * @remark set l'aiguillage lorsque la loco2 rentre dans la section critique
 */
void ManageLoco::setCritLoco2(){
   diriger_aiguillage(3,  TOUT_DROIT,       0);
   diriger_aiguillage(4,  DEVIE,       0);
   diriger_aiguillage(20,  TOUT_DROIT,       0);
   diriger_aiguillage(19,  DEVIE,       0);
}

/**
 * @brief ManageLoco::setEvitement
 * @remark set l'aiguillage lorsque la loco2 doit passer pas la voie d'évitement
 */
void ManageLoco::setEvitement(){
   diriger_aiguillage(4,  TOUT_DROIT,       0);
   diriger_aiguillage(19,  TOUT_DROIT,       0);
}
