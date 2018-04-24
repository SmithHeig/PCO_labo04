#include "ctrain_handler.h"
#include "locomotive.h"
#include "manageloco.h"
#include "messageListener.h"
#include <QList>
#include <QMutex>


//Creation d'une locomotive
static Locomotive locomotive1;
static Locomotive locomotive2;

static QMutex* mutex;
bool isLock;
//Arret d'urgence
void emergency_stop()
{
    locomotive1.arreter();
    locomotive2.arreter();
    afficher_message("\nSTOP!");
    //mutex->unlock();
}


//Fonction principale
int cmain()
{
    mutex = new QMutex();

    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_A);

    //Initialisation d'un parcours
    //QList<int> parcours;
    // parcours << 16 << 15 << 14 << 7 << 6 << 5 << 34 << 33 << 32 << 25 << 24;
    // parcours << 13 << 12 << 11 << 10 << 4 << 6 << 5 << 34 << 33 << 28 << 22 << 21 << 20 << 19;
    //parcours << 13 << 12;

    //Initialisation des aiguillages
    
    
    diriger_aiguillage(8,  DEVIE,       0);
    diriger_aiguillage(2,  DEVIE,       0);
    diriger_aiguillage(20, DEVIE,       0);
    diriger_aiguillage(14, DEVIE,       0);
    diriger_aiguillage(11, TOUT_DROIT,  0);
    diriger_aiguillage(17, TOUT_DROIT,  0);
    diriger_aiguillage(23, TOUT_DROIT,  0);

    // INIT
    diriger_aiguillage(5,  TOUT_DROIT,       0);
    diriger_aiguillage(3,  DEVIE,       0);
    diriger_aiguillage(20,  DEVIE,       0);
    diriger_aiguillage(23,  TOUT_DROIT,       0);

    diriger_aiguillage(22, TOUT_DROIT,  0);
    diriger_aiguillage(1, TOUT_DROIT,  0);
    diriger_aiguillage(19, DEVIE,  0);
    diriger_aiguillage(16, TOUT_DROIT,  0);
    diriger_aiguillage(13, TOUT_DROIT,  0);
    diriger_aiguillage(10, TOUT_DROIT,  0);
    diriger_aiguillage(7, TOUT_DROIT,  0);



    ManageLoco ml(locomotive1, locomotive2);
    MessageListener messListener(&ml);
    messListener.start();
    isLock = true;
    //mutex->lock();
    //mutex.lock();

    while(true){

    }

/*
    Attente du passage sur les contacts
    for (int i = 0; i < parcours.size(); i++) {
        attendre_contact(parcours.at(i));
        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                    .arg(locomotive1.numero()).arg(parcours.at(i))));
        locomotive1.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
    }
*/

    //Arreter la locomotive
    locomotive1.afficherMessage("Yeah, piece of cake!");

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    afficher_message("Enter a command in the input field at the top of the window.");
    //QString commande = getCommand();

    //afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}


/*
//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_B);

    //Initialisation d'un parcours
    QList<int> parcours;
    parcours << 24 << 21 << 16 << 15 << 10 << 11 << 6 << 5;

    //Initialisation des aiguillages
    diriger_aiguillage(16, TOUT_DROIT,  0);
    diriger_aiguillage(15, DEVIE,       0);
    diriger_aiguillage(12, DEVIE,       0);
    diriger_aiguillage(11, DEVIE,       0);
    diriger_aiguillage(9,  TOUT_DROIT,  0);
    diriger_aiguillage(5,  TOUT_DROIT,  0);
    diriger_aiguillage(8,  DEVIE,       0);
    diriger_aiguillage(7,  TOUT_DROIT,  0);
    diriger_aiguillage(4,  TOUT_DROIT,  0);
    diriger_aiguillage(3,  TOUT_DROIT,  0);

    //Initialisation de la locomotive
    locomotive.fixerNumero(1);
    locomotive.fixerVitesse(12);
    locomotive.fixerPosition(24, 5);
    locomotive.allumerPhares();
    locomotive.demarrer();
    locomotive.afficherMessage("Ready!");

    //Attente du passage sur les contacts
    for (int i = 0; i < parcours.size(); i++) {
        attendre_contact(parcours.at(i));
        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                    .arg(locomotive.numero()).arg(parcours.at(i))));
        locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
    }

    //Arreter la locomotive
    locomotive.arreter();
    locomotive.afficherMessage("Yeah, piece of cake!");

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    afficher_message("Enter a command in the input field at the top of the window.");
    QString commande = getCommand();
    afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}
*/