/**
* @file locomotive.cpp
* @authors Jeremie Chatillon et James Smith
*/

#include "locomotive.h"
#include "ctrain_handler.h"


/**
 * @brief Locomotive::Locomotive
 * @remark Constructeur sans paramètre
 */
Locomotive::Locomotive() :
    _numero(-1),
    _vitesse(0),
    _enFonction(false),
    _sens(true)
{
 // do nothing
}

/**
 * @brief Locomotive::Locomotive
 * @param numero - numéro de la locomotive
 * @param vitesse - vitesse de la locomotive
 * @remark Constructeur avec paramètres
 */
Locomotive::Locomotive(int numero, int vitesse) :
    _numero(numero),
    _vitesse(vitesse),
    _enFonction(false),
    _sens(true)
{

}

/**
 * @brief Locomotive::numero
 * @return le numero de la locomotive
 * @remark getter
 */
int Locomotive::numero() const
{
    return _numero;
}

/**
 * @brief Locomotive::fixerNumero
 * @param numero - nouveau numéro de la locomotive
 * @remark setter
 */
void Locomotive::fixerNumero(int numero)
{
    _numero = numero;
}

/**
 * @brief Locomotive::vitesse
 * @return la vitesse actuelle de la locomotive
 * @remark getter
 */
int Locomotive::vitesse() const
{
    return _vitesse;
}

/**
 * @brief Locomotive::fixerVitesse
 * @param vitesse - nouvelle vitesse de la locomotive
 * @remark setter. Si "en fonction", va changer progressivement
 */
void Locomotive::fixerVitesse(int vitesse)
{
    _vitesse = vitesse;

    if (_enFonction)
        mettre_vitesse_progressive(_numero, vitesse);
}

/**
 * @brief Locomotive::fixerPosition
 * @param contactAvant - numéro du contact positionner derrière la locomotive
 * @param contactArriere - numéro du contact positionner devant la locomotive
 * @remark setter - fixe la position de départ de la locomotive
 */
void Locomotive::fixerPosition(int contactAvant, int contactArriere)
{
    assigner_loco(contactAvant, contactArriere, _numero, _vitesse);
}

/**
 * @brief Locomotive::afficherMessage
 * @param message - message à afficher
 * @remark Affiche un message dans l'UI
 */
void Locomotive::afficherMessage(const QString &message)
{
    afficher_message_loco(_numero, qPrintable(message));
}

/**
 * @brief Locomotive::allumerPhares
 * @remark Allume les phares de la locomotive
 */
void Locomotive::allumerPhares()
{
    mettre_fonction_loco(_numero, ALLUME);
}

/**
 * @brief Locomotive::eteindrePhares
 * @remark Eteind les phars de la locomotive
 */
void Locomotive::eteindrePhares()
{
    mettre_fonction_loco(_numero, ETEINT);
}

/**
 * @brief Locomotive::demarrer
 * @remark Met en route la locomotive progressivement
 */
void Locomotive::demarrer()
{
    mettre_vitesse_progressive(_numero, _vitesse);
    _enFonction = true;
}

/**
 * @brief Locomotive::arreter
 * @remark arrête la locomotive
 */
void Locomotive::arreter()
{
    arreter_loco(_numero);
    _enFonction = false;
}

/**
 * @brief Locomotive::inverserSens
 * @remark fait faire demi tour à la locomotive (repart dans l'autre sens
 */
void Locomotive::inverserSens()
{
    inverser_sens_loco(_numero);
    _sens = (!_sens);
}

/**
 * @brief Locomotive::getSens
 * @return donne le sens de la locomotive
 * @remark getter
 */
bool Locomotive::getSens() const
{
    return _sens;
}

/**
 * @brief Locomotive::getID
 * @return l'id de la locomotive
 * @remark getter
 */
int Locomotive::getID() const
{
    return _id;
}

/**
 * @brief Locomotive::setID
 * @param id - set l'id de la locomotive
 * @remark setter
 */
void Locomotive::setID(int id)
{
    _id = id;
}
