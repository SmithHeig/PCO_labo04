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

public:
    ManageLoco(Locomotive& l1, Locomotive& l2);

    static void traiterSectionCritique(int pos, int idLoco){
        mutex.lock();
        bool sens;
        if(idLoco == 1)
            sens = locomotive1->getSens();
        if(idLoco == 2)
            sens = locomotive2->getSens();

        std::cout << "Loco " << idLoco << " : Passe le contactes " << pos << ".  Attente: "<< locoWait<< "sens: " << sens <<" \n";
        mutex.unlock();
        switch(pos){
        case 19:
            mutex.lock();
            if(++tourL2 == 3){
                locomotive2->arreter();
                locomotive2->inverserSens();
                locomotive2->demarrer();
                tourL2 = 0;
            }
            //std::cout << "Loco " << tourL1 << " Sens: " << sens;
            mutex.unlock();
            break;
        case 23:
            mutex.lock();
            if(++tourL1 == 3){
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
                mutex.lock();
                if(critSection){
                    if(idLoco == 1){
                       locoWait = true;
                        mutex.unlock();
                        locomotive1->arreter();
                    }
                    if(idLoco == 2){
                        mutex.unlock();
                        setEvitement();
                    }
                        //locomotive2->arreter();



                } else{

                    locoCritSection[idLoco -1] = true;
                    critSection = true;
                    if(idLoco == 1)
                        setCritLoco1();
                    if(idLoco == 2)
                        setCritLoco2();
                    mutex.unlock();
                }
            } else {
                mutex.lock();
                if(locoCritSection[idLoco-1] == true){ // Sort de criticSection
                    if(locoWait == true){
                        //locoCritSection[idLoco-1] = false;

                        locoCritSection[0] = true;
                        locoWait = false;

                        setCritLoco1();
                        locomotive1->demarrer();

                    } else{
                         critSection = false;
    //                     locoCritSection[idLoco-1] = false;
                        // mutex.unlock();
                    }

                    locoCritSection[idLoco-1] = false;
                    //std::cout << "Attente: "<< locoWait << "\n";
                }
                mutex.unlock();
                //mutex.unlock();

            }
            break;
        case 32:
        case 28:
            if(!sens){
                mutex.lock();
                if(critSection){
                    if(idLoco == 1){
                       locoWait = true;
                        mutex.unlock();
                        locomotive1->arreter();
                    }
                    if(idLoco == 2){
                        mutex.unlock();
                        setEvitement();
                    }
                        //locomotive2->arreter();



                } else{

                    locoCritSection[idLoco -1] = true;
                    critSection = true;
                    if(idLoco == 1)
                        setCritLoco1();
                    if(idLoco == 2)
                        setCritLoco2();
                    mutex.unlock();
                }
            } else {
                mutex.lock();
                if(locoCritSection[idLoco-1] == true){ // Sort de criticSection
                    if(locoWait == true){
                        //locoCritSection[idLoco-1] = false;

                        locoCritSection[0] = true;
                        locoWait = false;

                        setCritLoco1();
                        locomotive1->demarrer();

                    } else{
                         critSection = false;
    //                     locoCritSection[idLoco-1] = false;
                        // mutex.unlock();
                    }

                    locoCritSection[idLoco-1] = false;
                    //std::cout << "Attente: "<< locoWait << "\n";
                }
                mutex.unlock();
                //mutex.unlock();

            }
            break;
        }
    }

};

#endif // MANAGELOCO_H
