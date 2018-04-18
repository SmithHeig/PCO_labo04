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
    locomotive1->fixerVitesse(20);
    locomotive1->fixerPosition(16, 23);
    locomotive1->allumerPhares();
    locomotive1->demarrer();
    locomotive1->afficherMessage("Ready!");


    locomotive2->fixerNumero(2);
    locomotive2->fixerVitesse(20);
    locomotive2->fixerPosition(13, 19);
    locomotive2->allumerPhares();
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



    critiquePointsList.append(new LocoListener(14, 1));
    critiquePointsList.append(new LocoListener(10, 2));
    critiquePointsList.append(new LocoListener(32, 1));
    critiquePointsList.append(new LocoListener(28, 2));

    critiquePointsList.append(new LocoListener(19, 2));     // Inverser sens
    critiquePointsList.append(new LocoListener(23, 1));     // Inverser sens

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

