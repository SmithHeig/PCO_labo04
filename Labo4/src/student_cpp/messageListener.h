/**
 * @file messageListener.h
 * @authors Jeremie Chatillon et James Smith
 * @remark Permet de récupérer les commandes de l'utilisateur
 */

#ifndef MESSAGELISTENER_H
#define MESSAGELISTENER_H

#include "manageloco.h"
#include <QMutex>

class MessageListener : public QThread
{
private:
    // Attributs
    QMutex mutex;
    ManageLoco* manageLoco;
public:
    /**
     * @brief MessageListener
     * @param ml - Manager de locomotive à modifier lors de commande
     * @remark Constructeur
     */
    MessageListener(ManageLoco* ml);

    /**
     * @brief ~MessageListener
     * @remark Destructeur
     */
    ~MessageListener(){};

    /**
     * @brief analyseCommand
     * @param s - commande rentré par l'utilisateur dans l'UI
     * @remark analyse quelle commande a été rentré par l'utilisateur
     */
    void analyseCommand(const char* s);
protected:

    /**
     * @brief run
     * @remark thread l'écoute
     */
    void run() Q_DECL_OVERRIDE;
};

#endif // MESSAGELISTENER_H
