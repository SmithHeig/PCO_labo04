#ifndef LOCOLISTENER_H
#define LOCOLISTENER_H

#include <QThread>
#include "loco.h"
//#include "manageloco.h"

class LocoListener : public QThread{
private:
    const int pos;
    const int idLoco;
public:
    LocoListener(int rail, int idLoco);

protected:
    void run() Q_DECL_OVERRIDE;
};

#endif // LOCOLISTENER_H

