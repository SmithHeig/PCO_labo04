#ifndef MANAGELOCO_H
#define MANAGELOCO_H

#include "loco.h"
#include "locomotive.h"
#include "locoListener.h"
#include "ctrain_handler.h"
#include <QThread>
#include "iostream"

class ManageLoco
{
public:
    ManageLoco(Locomotive& l1, Locomotive& l2);
    static void traiterSectionCritique(int pos, int idLoco);
private:
    int nbLoco = 2;
    static int tourL1;
    static int tourL2;
    //Creation d'une locomotive
    static Locomotive* locomotive1;
    static Locomotive* locomotive2;

    static std::mutex mutex;
    static bool critSection;
    static bool locoCritSection[2];
    static bool locoWait;

    //Initialisation d'un parcours
    QList<int> critiquePoints;
    QList<LocoListener*> critiquePointsList;
private:
    static void setCritLoco1(){
        diriger_aiguillage(3,  DEVIE,       0);
        diriger_aiguillage(20,  DEVIE,       0);
    }
    static void setCritLoco2(){
        diriger_aiguillage(3,  TOUT_DROIT,       0);
        diriger_aiguillage(4,  DEVIE,       0);
        diriger_aiguillage(20,  TOUT_DROIT,       0);
        diriger_aiguillage(19,  DEVIE,       0);
    }

    static void setEvitement(){
        diriger_aiguillage(4,  TOUT_DROIT,       0);
        diriger_aiguillage(19,  TOUT_DROIT,       0);
    }

};

#endif // MANAGELOCO_H
