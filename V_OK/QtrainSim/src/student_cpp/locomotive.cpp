#include "locomotive.h"
#include "ctrain_handler.h"

int Locomotive::curID = 1;

Locomotive::Locomotive() :
    _numero(-1),
    _vitesse(0),
    _enFonction(false),
    _sens(true),
    _id(curID++)
{

}

Locomotive::Locomotive(int numero, int vitesse) :
    _numero(numero),
    _vitesse(vitesse),
    _enFonction(false),
    _sens(true),
    _id(curID++)
{

}

int Locomotive::numero() const
{
    return _numero;
}

void Locomotive::fixerNumero(int numero)
{
    _numero = numero;
}

int Locomotive::vitesse() const
{
    return _vitesse;
}

void Locomotive::fixerVitesse(int vitesse)
{
    _vitesse = vitesse;

    if (_enFonction)
        mettre_vitesse_progressive(_numero, vitesse);
}

void Locomotive::fixerPosition(int contactAvant, int contactArriere)
{
    assigner_loco(contactAvant, contactArriere, _numero, _vitesse);
}

void Locomotive::afficherMessage(const QString &message)
{
    afficher_message_loco(_numero, qPrintable(message));
}

void Locomotive::allumerPhares()
{
    mettre_fonction_loco(_numero, ALLUME);
}

void Locomotive::eteindrePhares()
{
    mettre_fonction_loco(_numero, ETEINT);
}

void Locomotive::demarrer()
{
    mettre_vitesse_progressive(_numero, _vitesse);
    _enFonction = true;
}

void Locomotive::arreter()
{
    arreter_loco(_numero);
    _enFonction = false;
}

void Locomotive::inverserSens()
{
    inverser_sens_loco(_numero);
    _sens = (!_sens);
}

bool Locomotive::getSens() const
{
    return _sens;
}

int Locomotive::getID() const
{
    return _id;
}

void Locomotive::setID(int id)
{
    _id = id;
}
