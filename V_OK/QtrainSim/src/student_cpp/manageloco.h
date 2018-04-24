/**
* @file manageloco.h
* @authors Jeremie Chatillon et James Smith
*/

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

    /**
     * @class Loclistener
     * @remark Permet d'écouter des points critique et de faire réagire le manager en conséquence
     */
    class LocoListener : public QThread{
    private:
        // Attributs
        ManageLoco& refThis;
        const int pos;
        const int idLoco;
    public:
        /**
         * @brief ManageLoco::LocoListener::LocoListener
         * @param pos - postion du capteur à écouter
         * @param idLoco - loco qui passe sur le capteur
         * @param it - manager à contacter lorsqu'il doit faire quelque chose
         * @remark constructeur
         */
        LocoListener(int rail, int idLoco, ManageLoco& it);

    protected:
        /**
         * @brief ManageLoco::LocoListener::run
         * @remark thread qui écoute les contacts et qui appelle le traitement selon la position
         */
        void run() Q_DECL_OVERRIDE;
    };

public:
    //ManageLoco();

    /**
     * @brief ManageLoco::ManageLoco
     * @param l1 - locomotive 1 à gérer
     * @param l2 - locomotive 2 à gérer
     * @remark constructeur avec paramètre
     */
    ManageLoco(Locomotive& l1, Locomotive& l2);

    /**
     * @brief ~ManagerLoco
     * @remark destructeur
     */
    ~ManageLoco();

    /**
     * @brief ManageLoco::setPrio
     * @param p
     * @remark change la priorité des trains
     *         prio 0: priorité normal, le premier dedans reserve la section critique
     *         prio 1: la locomotive 1 à la priorité et la 2 passe toujours par l'évitement
     *         prio 2: la locomotive 2 à la priorité et la 1 s'arrête et attends un changement de priorité
     */
    void setPrio(int p);

    /**
     * @brief ManageLoco::entreSectionCritique
     * @param idLoco - id de la locomotive entrant dans la section critique
     * @remark gestion de la section critique selon locomotive, prio et l'autre locomotive
     */
    void entreSectionCritique(int idLoco);

    /**
     * @brief ManageLoco::sortSectionCritique
     * @param idLoco - id de la locomotive sortant de la section critique
     * @remark gestion de la libération de la section critique
     */
    void sortSectionCritique(int idLoco);

    /**
     * @brief ManageLoco::traiterPointLoco
     * @param pos - numéro du capteur passé
     * @param idLoco - locomotive ayant passé le capteur
     * @remark check si le capteur à une importance et quoi faire
     */
    void traiterPointLoco(int pos, int idLoco);
private:
    // Attributs
    int prio;
    int tourL1;
    int tourL2;

    //Creation d'une locomotive
    Locomotive* locomotive1;
    Locomotive* locomotive2;

    QMutex mutex;
    bool critSection;
    bool locoCritSection[2];
    bool locoWait;

    QList<LocoListener*> critiquePointsList;
private:

    /**
     * @brief ManageLoco::init
     * @remark Initialisation des aiguillages par rapport au parcours initial
     */
    void init();

    /**
     * @brief ManageLoco::setCritLoco1
     * @remark set l'aiguillage lorsque la loco1 rentre dans la section critique
     */
    void setCritLoco1();

    /**
     * @brief ManageLoco::setCritLoco2
     * @remark set l'aiguillage lorsque la loco2 rentre dans la section critique
     */
    void setCritLoco2();

    /**
     * @brief ManageLoco::setEvitement
     * @remark set l'aiguillage lorsque la loco2 doit passer pas la voie d'évitement
     */
    void setEvitement();
};

#endif // MANAGELOCO_H
