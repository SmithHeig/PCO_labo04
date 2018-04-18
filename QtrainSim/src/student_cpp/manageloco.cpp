#include "manageloco.h"
#include "loco.h"
Locomotive* ManageLoco::locomotive1;
Locomotive* ManageLoco::locomotive2;

std::mutex ManageLoco::mutex;
bool ManageLoco::critSection;
bool ManageLoco::locoCritSection[2];
bool ManageLoco::locoWait;

int ManageLoco::tourL1;
int ManageLoco::tourL2;

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

    //setCritLoco1();
    setCritLoco1();



    critiquePointsList.append(new LocoListener(14, locomotive1->getID()));
    critiquePointsList.append(new LocoListener(10, locomotive2->getID()));
    critiquePointsList.append(new LocoListener(25, locomotive1->getID()));
    critiquePointsList.append(new LocoListener(28, locomotive2->getID()));

    critiquePointsList.append(new LocoListener(19, locomotive2->getID()));     // Inverser sens
    critiquePointsList.append(new LocoListener(23, locomotive1->getID()));     // Inverser sens

    for(auto i = critiquePointsList.begin(); i != critiquePointsList.end(); ++i)
        (*i)->start();

       // Loco::posLocos;

/*
    critiquePointsL1 << 10  << 34 << 30;
    for (int i = 0; i < critiquePointsL1.size(); i++) {
        attendre_contact(critiquePointsL1.at(i));
        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                    .arg(locomotive1->numero()).arg(critiquePointsL1.at(i))));
        locomotive1->afficherMessage(QString("I've reached contact no. %1.").arg(critiquePointsL1.at(i)));
    }*/


/*
    //critiquePointsL1 << 10 << 14 << 34 << 30;
    critiquePointsL2 << 10 << 34;
    for (int i = 0; i < critiquePointsL2.size(); i++) {
        attendre_contact(critiquePointsL2.at(i));
        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                    .arg(locomotive2->numero()).arg(critiquePointsL2.at(i))));
        locomotive2->afficherMessage(QString("I've reached contact no. %1.").arg(critiquePointsL2.at(i)));
    }
*/
    }

void ManageLoco::traiterSectionCritique(int pos, int idLoco){
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
        (++tourL2);
        if(tourL2 == 3){
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
    case 25:
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
