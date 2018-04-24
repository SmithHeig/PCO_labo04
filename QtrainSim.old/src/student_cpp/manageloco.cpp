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
ManageLoco::ManageLoco(Locomotive& l1, Locomotive& l2)
{


    locomotive1 = &l1;
    locomotive2 = &l2;

    locomotive1->fixerNumero(1);
    locomotive1->fixerVitesse(12);
    locomotive1->fixerPosition(16, 23);
    locomotive1->allumerPhares();
    locomotive1->demarrer();
    locomotive1->afficherMessage("Ready!");


    locomotive2->fixerNumero(2);
    locomotive2->fixerVitesse(12);
    locomotive2->fixerPosition(13, 19);
    locomotive2->allumerPhares();
    //locomotive2->inverserSens();
    locomotive2->demarrer();
    locomotive2->afficherMessage("Ready!");

    locoCritSection[0] = false;
    locoCritSection[1] = false;
    critSection = false;
    locoWait = false;

    tourL1 = 1;         // commence à 1 pour le premier tour
    tourL2 = 1;

    prio = 0;

    //setCritLoco1();
    setCritLoco1();



    critiquePointsList.append(new LocoListener(14, locomotive1->getID(), *this));
    critiquePointsList.append(new LocoListener(10, locomotive2->getID(), *this));
    critiquePointsList.append(new LocoListener(25, locomotive1->getID(), *this));
    critiquePointsList.append(new LocoListener(28, locomotive2->getID(), *this));

    critiquePointsList.append(new LocoListener(19, locomotive2->getID(), *this));     // Inverser sens
    critiquePointsList.append(new LocoListener(23, locomotive1->getID(), *this));     // Inverser sens

    for(auto i = critiquePointsList.begin(); i != critiquePointsList.end(); ++i)
        (*i)->start();
    }


void ManageLoco::setPrio(int p){
    this->prio = p;
    if(locoWait && !critSection){
        locoCritSection[0] = true;
        locoWait = false;

        setCritLoco1();
        locomotive1->demarrer();
    }
}

void ManageLoco::traiterSectionCritique(int pos, int idLoco){
    mutex.lock();
    bool sens;
    if(idLoco == 1)
        sens = locomotive1->getSens();
    if(idLoco == 2)
        sens = locomotive2->getSens();
    //afficher_message_loco(idLoco, "Passe contacte ");

    afficher_message_loco(idLoco, "Passe contacte " );
   // std::cout << "Loco " << idLoco << " : Passe le contactes " << pos << ".  Attente: "<< locoWait<< "sens: " << sens <<" \n";
    mutex.unlock();
    switch(pos){
    case 19:
        mutex.lock();
        (++tourL2);
        if(tourL2 == 3){
            locomotive2->arreter();
            locomotive2->inverserSens();
            locomotive2->demarrer();
            tourL2 = 0;
        }
        mutex.unlock();
        break;
    case 23:
        mutex.lock();
        if((++tourL1) == 3){
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

            if(critSection){
                mutex.lock();
                if(idLoco == 1){
                   locoWait = true;
                    locomotive1->arreter();
                }
                if(idLoco == 2){
                    setEvitement();
                }
                mutex.unlock();
            } else{
                switch(prio){
                case 0:
                    mutex.lock();
                     if(!critSection){
                        locoCritSection[idLoco -1] = true;
                        critSection = true;
                        if(idLoco == 1)
                            setCritLoco1();
                        if(idLoco == 2)
                            setCritLoco2();

                    }
                     mutex.unlock();
                    break;
                case 1:
                    mutex.lock();
                    if(idLoco == 1){
                        locoCritSection[0] = true;
                        setCritLoco1();
                        critSection = true;
                    }
                    if(idLoco == 2){
                        setEvitement();
                    }
                    mutex.unlock();
                    break;

                case 2:
                    mutex.lock();

                    if(idLoco == 1){

                        setCritLoco1();
                        locoWait = true;
                        locomotive1->arreter();

                    }
                    if(idLoco == 2){
                        setCritLoco2();
                        critSection = true;
                        locoCritSection[1] = true;
                    }
                    mutex.unlock();
                    break;
                }
            }
        } else {
            mutex.lock();
            if(locoCritSection[idLoco-1] == true){ // Sort de criticSection
                if(locoWait == true && prio != 2){
                    locoCritSection[0] = true;
                    locoWait = false;
                    setCritLoco1();
                    locomotive1->demarrer();
                } else{
                     critSection = false;
                }
                locoCritSection[idLoco-1] = false;
            }
            mutex.unlock();
            //mutex.unlock();

        }
        break;
    case 25:
    case 28:
        if(!sens){
            switch(prio){
            case 0:
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
                } else{
                    locoCritSection[idLoco -1] = true;
                    critSection = true;
                    if(idLoco == 1)
                        setCritLoco1();
                    if(idLoco == 2)
                        setCritLoco2();
                    mutex.unlock();
                }
                break;
            case 1:
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
                } else{
                    if(idLoco == 1){
                        locoCritSection[0] = true;
                        setCritLoco1();
                        critSection = true;
                    }
                    if(idLoco == 2){
                        setEvitement();
                    }
                    mutex.unlock();
                }

                break;

            case 2:
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
                } else{
                    if(idLoco == 1){

                        setCritLoco1();
                        locoWait = true;

                        locomotive1->arreter();

                    }
                    if(idLoco == 2){
                        setCritLoco2();
                        critSection = true;
                        locoCritSection[1] = true;
                    }
                    mutex.unlock();
                }
                break;
            }
        } else {
            mutex.lock();
            if(locoCritSection[idLoco-1] == true){ // Sort de criticSection
                if(locoWait == true && prio != 2){
                    locoCritSection[0] = true;
                    locoWait = false;

                    setCritLoco1();
                    locomotive1->demarrer();

                } else{
                     critSection = false;
                }

                locoCritSection[idLoco-1] = false;
            }
            mutex.unlock();
        }
        break;
    }

}


ManageLoco::LocoListener::LocoListener(int pos, int idLoco, ManageLoco& it)
    :pos(pos), idLoco(idLoco), refThis(it)
{
    Loco::posLocos[0] = -1;
    Loco::posLocos[1] = -1;
}

void ManageLoco::LocoListener::run(){
    while(true){
        attendre_contact(pos);
        if(Loco::posLocos[idLoco - 1] == pos){
            refThis.traiterSectionCritique(Loco::posLocos[idLoco - 1], idLoco);
        }
    };
}
