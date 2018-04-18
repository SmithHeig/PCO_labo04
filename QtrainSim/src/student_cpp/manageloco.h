#ifndef MANAGELOCO_H
#define MANAGELOCO_H


#include "locomotive.h"
#include "ctrain_handler.h"
#include <QThread>
#include <QMutex>
#include "iostream"

class ManageLoco
{
private:
    // Class interne
    class LocoListener : public QThread{
    private:
        ManageLoco& refThis;
        const int pos;
        const int idLoco;
    public:
        LocoListener(int rail, int idLoco, ManageLoco& it);

    protected:
        void run() Q_DECL_OVERRIDE;
    };

public:
    ManageLoco(Locomotive& l1, Locomotive& l2);
     void traiterSectionCritique(int pos, int idLoco);
private:
    int nbLoco = 2;
     int tourL1;
     int tourL2;
    //Creation d'une locomotive
     Locomotive* locomotive1;
     Locomotive* locomotive2;

     QMutex mutex;
     bool critSection;
     bool locoCritSection[2];
     bool locoWait;

    //Initialisation d'un parcours
    QList<int> critiquePoints;
    QList<LocoListener*> critiquePointsList;
private:
     void init(){
        diriger_aiguillage(1,  TOUT_DROIT,       0);
        diriger_aiguillage(2,  DEVIE,            0);
        diriger_aiguillage(3,  DEVIE,            0);
        diriger_aiguillage(1,  TOUT_DROIT,       0);
    }

     void setCritLoco1(){
        diriger_aiguillage(3,  DEVIE,       0);
        diriger_aiguillage(20,  DEVIE,       0);
    }
     void setCritLoco2(){
        diriger_aiguillage(3,  TOUT_DROIT,       0);
        diriger_aiguillage(4,  DEVIE,       0);
        diriger_aiguillage(20,  TOUT_DROIT,       0);
        diriger_aiguillage(19,  DEVIE,       0);
    }

     void setEvitement(){
        diriger_aiguillage(4,  TOUT_DROIT,       0);
        diriger_aiguillage(19,  TOUT_DROIT,       0);
    }
};

#endif // MANAGELOCO_H
