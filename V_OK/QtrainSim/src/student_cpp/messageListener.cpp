/**
 * @file messageListener.cpp
 * @authors Jeremie Chatillon et James Smith
 * @remark Permet de récupérer les commandes de l'utilisateur
 */

#include "messageListener.h"

/**
 * @brief MessageListener
 * @param ml - Manager de locomotive à modifier lors de commande
 * @remark Constructeur
 */
MessageListener::MessageListener(ManageLoco* ml)
    : manageLoco(ml)
{
    // do nothing
}

/**
 * @brief run
 * @remark thread l'écoute
 */
void MessageListener::run(){
    while(true){
        analyseCommand(getCommand());
    }
}

/**
 * @brief analyseCommand
 * @param s - commande rentré par l'utilisateur dans l'UI
 * @remark analyse quelle commande a été rentré par l'utilisateur
 */
void MessageListener::analyseCommand(const char * s){
    mutex.lock();
    if(!strcmp("prio_0", s)){
        manageLoco->setPrio(0);
        afficher_message(qPrintable(QString("Priority remove")));
    }
    else if(!strcmp("prio_1", s)){
        manageLoco->setPrio(1);
        afficher_message(qPrintable(QString("Priority set to loco 1")));

    }else if(!strcmp("prio_2", s)){
        manageLoco->setPrio(2);
        afficher_message(qPrintable(QString("Priority set to loco 2")));
    }else
        afficher_message(qPrintable(QString("Error: \"") + QString(s) + QString("\" is not a command")));

    mutex.unlock();
}

