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
     * @brief The LocoListener class
     */
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
    //ManageLoco();
    ~ManageLoco();
    ManageLoco(Locomotive& l1, Locomotive& l2);
    void traiterPointLoco(int pos, int idLoco);
    void sortSectionCritique(int idLoco);
    void entreSectionCritique(int idLoco);
    void setPrio(int p);
private:
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
     * @brief init
     * Initalise les parcours initiaux des 2 locomotives.
     */
    void init();
    /**
     * @brief setCritLoco1
     * Initalise la voie commune pour la loco 1.
     */
    void setCritLoco1();
    /**
     * @brief setCritLoco2
     * Initalise la voie commune pour la loco 2.
     */
    void setCritLoco2();
    /**
     * @brief setEvitement
     * Initalise la voie d'évitement pour la loco 2.
     */
    void setEvitement();
};

#endif // MANAGELOCO_H
