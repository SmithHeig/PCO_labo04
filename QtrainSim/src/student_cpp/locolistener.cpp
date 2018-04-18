#include "locoListener.h"
#include "manageloco.h"
#include "iostream"

LocoListener::LocoListener(int pos, int idLoco)
    :pos(pos), idLoco(idLoco)
{
    Loco::posLocos[0] = -1;
    Loco::posLocos[1] = -1;
}

void LocoListener::run(){
    while(true){
        attendre_contact(pos);
        if(Loco::posLocos[idLoco - 1] == pos){

            //std::cout << "Loco " << idLoco << " : fdtgzhugzftdrtfgzfutdrz le contact " << pos << "\n";
            ManageLoco::traiterSectionCritique(Loco::posLocos[idLoco - 1], idLoco);
        }
    };
}
