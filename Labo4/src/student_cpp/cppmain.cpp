/**
* @file cppmain.cpp
* @authors Jeremie Chatillon et James Smith
*/

#include "ctrain_handler.h"
#include "locomotive.h"
#include "manageloco.h"
#include "messageListener.h"
#include <QList>
#include <QMutex>
#include <QSemaphore>


//Creation d'une locomotive
static Locomotive locomotive1;
static Locomotive locomotive2;

// Création des mutex et variable servant à sécurisé les sections critiques
static bool isStop = false;
static QSemaphore* _mutex;
bool isLock;

/**
 * @brief emergency_stop
 * @remark Permet d'arrêter les trains. Appeller par la crois à coté du play
 */
void emergency_stop()
{
    if(!isStop){
       locomotive1.arreter();
       locomotive2.arreter();
       afficher_message("\nSTOP! C'est la fin.");
       _mutex->release(); // Permet de finir le programme
        isStop = !isStop;
    }
}


/**
 * @brief cmain
 * @return entier representant la réussite ou l'echec du programme
 * @remark Programme principal
 */
int cmain()
{
    // Initialisation du mutex
    _mutex = new QSemaphore(0);

    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_A);

    ManageLoco* ml = new ManageLoco(locomotive1, locomotive2);
    MessageListener* messListener = new MessageListener(ml);
    messListener->start();

    //attendre_contact(27);

    // Bloque tant que emergency_stop n'est pas appelé
    _mutex->acquire();

    delete ml;
    //delete messListener;

    //Fin de la simulation
    mettre_maquette_hors_service();

    return EXIT_SUCCESS;
}
