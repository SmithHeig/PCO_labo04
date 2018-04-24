#ifndef MESSAGELISTENER_H
#define MESSAGELISTENER_H

#include "manageloco.h"
#include <QMutex>

class MessageListener : public QThread
{
private:

    QMutex mutex;
    ManageLoco* manageLoco;
public:
    MessageListener(ManageLoco* ml);
    ~MessageListener(){};
    void analyseCommand(const char* s);
protected:
    void run() Q_DECL_OVERRIDE;
};

#endif // MESSAGELISTENER_H
