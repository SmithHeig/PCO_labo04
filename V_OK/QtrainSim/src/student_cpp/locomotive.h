/**
 * @file locomotive.h
 * @authors Jeremie Chatillon et James Smith
 */

#ifndef LOCOMOTIVE_H
#define LOCOMOTIVE_H

#include <QString>

class Locomotive
{
private:
    static int curID;
public:

    /**
     * @brief Locomotive::Locomotive
     * @remark Constructeur sans paramètre
     */
    Locomotive();

    /**
     * @brief Locomotive::Locomotive
     * @param numero - numéro de la locomotive
     * @param vitesse - vitesse de la locomotive
     * @remark Constructeur avec paramètres
     */
    Locomotive(int numero, int vitesse);

    /**
     * @brief Locomotive::numero
     * @return le numero de la locomotive
     * @remark getter
     */
    int numero() const;

    /**
     * @brief Locomotive::fixerNumero
     * @param numero - nouveau numéro de la locomotive
     * @remark setter
     */
    void fixerNumero(int numero);

    /**
     * @brief Locomotive::vitesse
     * @return la vitesse actuelle de la locomotive
     * @remark getter
     */
    int vitesse() const;

    /**
     * @brief Locomotive::fixerVitesse
     * @param vitesse - nouvelle vitesse de la locomotive
     * @remark setter. Si "en fonction", va changer progressivement
     */
    void fixerVitesse(int vitesse);

    /**
     * @brief Locomotive::fixerPosition
     * @param contactAvant - numéro du contact positionner derrière la locomotive
     * @param contactArriere - numéro du contact positionner devant la locomotive
     * @remark setter - fixe la position de départ de la locomotive
     */
    void fixerPosition(int contactAvant, int contactArriere);

    /**
     * @brief Locomotive::afficherMessage
     * @param message - message à afficher
     * @remark Affiche un message dans l'UI
     */
    void afficherMessage(const QString &message);

    /**
     * @brief Locomotive::allumerPhares
     * @remark Allume les phares de la locomotive
     */
    void allumerPhares();

    /**
     * @brief Locomotive::eteindrePhares
     * @remark eteind les phars de la locomotive
     */
    void eteindrePhares();

    /**
     * @brief Locomotive::demarrer
     * @remark Met en route la locomotive progressivement
     */
    void demarrer();

    /**
     * @brief Locomotive::arreter
     * @remark arrête la locomotive
     */
    void arreter();

    /**
     * @brief Locomotive::inverserSens
     * @remark fait faire demi tour à la locomotive (repart dans l'autre sens
     */
    void inverserSens();

    /**
     * @brief Locomotive::getSens
     * @return donne le sens de la locomotive
     * @remark getter
     */
    bool getSens() const;

    /**
     * @brief Locomotive::getID
     * @return l'id de la locomotive
     * @remark getter
     */
    int getID() const;

    /**
     * @brief Locomotive::setID
     * @param id - set l'id de la locomotive
     * @remark setter
     */
    void setID(int id);

private:
    // Attributs
    int _numero;
    int _id;
    int _vitesse;
    bool _enFonction;
    bool _sens;             // true = gauche à droite, false = droite à gauche
};

#endif // LOCOMOTIVE_H
