#include "messageListener.h"

MessageListener::MessageListener(ManageLoco* ml)
    : manageLoco(ml)
{

}


void MessageListener::run(){
    while(true){
       // QString s = ;
        analyseCommand(getCommand());
    }
}

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

