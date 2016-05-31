/**
 * \file Graphe.h
 * \brief Gestion d'un graphe.
 * \author Mondher Bouden
 * \version 0.1
 * \date Juillet 2015
 *
 *  Travail pratique numéro 3
 *
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
//Vous pouvez ajouter d'autres librairies de la STL

#ifndef _GRAPHE__H
#define _GRAPHE__H

namespace TP3
{

/**
 * \struct Ponderations
 * \brief Une struture utilisée pour stocker les pondérations utilisées sur les arcs des sommets
 */
struct Ponderations
{
	float duree;		// La durée du vol en heures (ex. 0.8 heures entre Québec et Montréal)
	float cout;			// Le cout du trajet en dollars (ex. 220.46 $ pour un vol entre Québec et Montréal)
	int ns;				// Un niveau de sécurité représentant un indice qui peut être négatif
};


/**
 * \struct Coordonnees
 * \brief Une structure utilisée pour stocker les coordonnées d'un sommet
 */
struct Coordonnees
{
	float lt;			// La latitude en décimale (ex. pour la ville de Québec : 46.80)
	float lg;			// La longitude en décimale (ex. pour la ville de Québec : -71.24)
};


/**
 * \class Graphe
 * \brief Classe représentant un graphe
 */
class Graphe
{
public:

	/**
	 * \brief      Constructeur par défaut
	 * \post       Un graphe vide est créé
	 */
	Graphe();

	/**
	 * \brief      Destructeur
	 * \post       Le graphe a été détruit
	 * Doit appeler detruireGraphe()
	 */
	~Graphe();

	/**
	 * \brief Constructeur de copie.
	 * \pre Il y a assez de mémoire.
	 * \param[in] source (Graphe) objet Graphe a utiliser pour faire la copie profonde
	 * \exception bad_alloc s'il n'y a pas assez de mémoire.
	 */
	Graphe(const Graphe &source);

	/**
	* \brief Surcharge de l'opérateur d'affectation.
	* \pre Il doit y avoir assez de mémoire.
	* \param[in] src (Graphe) objet Graphe a utiliser pour faire la copie profonde
	* \post Le graphe a un contenu identique à src.
	* \return Un objet Graphe (par ref)
	* \exception bad_alloc s'il n'y a pas assez de mémoire.
	*/
	Graphe& operator=(const Graphe& src);

	/**
	 * \brief Surcharge de l'opérateur de sortie.
	 * \post Les nombres de sommets et d'arcs du graphe seront affichés
	 * \post chaque sommet, son nom sera affiché
	 * \post pour chaque sommet, le nom de tous ses voisins seront affichés
	 * Ne pas toucher à cette fonction !
	 */
	friend std::ostream& operator<<(std::ostream& out, const Graphe& g)
	{
		out << "Le graphe contient " << g.nbSommets << " sommet(s) et "
				<< g.nbArcs << " arc(s)" << std::endl << std::endl;
		Sommet* vertex = g.listeSommets;
		while (vertex != 0)
		{
			out << "Sommet: " << vertex->nom << "  Voisins: ";
			// Afficher les arcs.
			Arc* arc = vertex->listeDest;
			if(arc != 0)
			{
				while (arc->suivDest != 0)
				{
					out << arc->dest->nom << ", ";
					arc = arc->suivDest;
				}
				out << arc->dest->nom;
			}
			else
			{
				out << "Rien";
			}
			out << std::endl << std::endl;
			vertex = vertex->suivant;
		}
		return out;
	}

	/**
	* \brief Ajout d'un sommet au graphe
	* \param[in] nom (string) Nom de la ville
	* \param[in] lt (float) Latitude
	* \param[in] lg (float) Longitude
	* \exception  bad_alloc Pas assez de mémoire pour alloué le nouvel arc
	* \exception logic_error si le sommet existe deja dans le graphe
	*/
	void ajouterSommet(const std::string& nom, float lt, float lg);

	/**
	* \brief Ajout d'un arc au graphe
	* \pre Les deux sommets doivent exister
	* \param[in] nom1 (string) Nom de la ville d'ou l'arc part (depart)
	* \param[in] nom2 (string) Nom de la ville d'ou l'arc arrive (arrivee)
	* \param[in] duree (float) Duree du trajet en heures
	* \param[in] cout (float) Cout du trajet en $
	* \param[in] ns (int) niveau de securite du trajet (plus grand le chiffre, plus grand le risque)
	* \post Un arc a été ajouté entre le sommet qui a le nom1 et le sommet qui a le nom2
	* \exception  bad_alloc Pas assez de mémoire pour alloué le nouvel arc
	* \exception  logic_error Un des deux sommets n'existe pas
	* \exception  logic_error Il y a déjà un arc orienté entre ces deux sommets
	*/
	void ajouterArc(const std::string& nom1, const std::string& nom2, float duree, float cout, int ns);

	/**
	* \brief Enlève un sommet du graphe ainsi que tous les arcs qui vont et partent de ce sommet
	* \pre Le sommet doit exister
	* \param[in] nom (string) Nom de la ville a enlever du graphe
	* \post Le sommet identifié par 'nom' a été enlevé du graphe
	* \exception logic_error Le sommet spécifié en argument n'existe pas
	*/
	void enleverSommet(const std::string& nom);

	/**
	* \brief Enlève un arc du graphe
	* \pre Les deux sommets identifiés par leur nom doivent appartenir au graphe.
	* \pre Un arc reliant les deux sommets doit exister.
	* \param[in] nom1 (string) Nom de la ville d'ou l'arc a enlever part (depart)
	* \param[in] nom2 (string) Nom de la ville d'ou l'arc a enlever arrive (arrivee)
	* \post Le graphe contient un arc en moins, le graphe inchangé sinon
	* \exception logic_error Un des deux sommets, ou les deux, n'existent pas
	* \exception logic_error Il n'y a pas d'arc entre les 2 sommets
	*/
	void enleverArc(const std::string& nom1, const std::string& nom2);

	/**
	* \brief Retourne la liste des noms des sommets dans un vecteur de chaînes de caractères
	* \return vector : Vector contenant tous les sommets du graphe
	* \post Le graphe reste inchangé.
	*/
	std::vector<std::string> listerNomsSommets() const;

	/**
	* \brief Retourne la liste des noms des sommets adjacents au sommet passé en argument dans un vecteur de chaînes de caractères
	* \pre Le sommet doit appartenir au graphe
	* \param[in] nom (string) Nom de la ville a verifier
	* \return vector : Vector contenant tous les sommets adjacents au sommet nom
	* \post Le graphe reste inchangé.
	* \exception logic_error si le sommet n'existe pas
	*/
	std::vector<std::string> listerSommetsAdjacents(const std::string& nom) const;

	/**
	* \brief Retourne le nombre de sommets dans la liste des sommets
	* \return int : Nombre de sommets dans le graphe
	* \post Le graphe reste inchangé.
	*/
	int nombreSommets() const;

	/**
	* \brief Retourne le nombre d'arcs
	* \return int : Nombre d'arcs dans le graphe
	* \post le graphe reste inchangé
	*/
	int nombreArcs() const;

	/**
	* \brief Indique si la liste des sommets est vide
	* \return Boolean : True si graphe est vide, false sinon
	* \post Le graphe reste inchangé
	*/
	bool estVide() const;

	/**
	* \brief Retourne l'existence d'un sommet
	* \param[in] nom (string) Nom de la ville a verifier
	* \post Le graphe reste inchangé.
	* \return Boolean : True si sommet existe, false sinon
	*/
	bool sommetExiste(const std::string& nom) const;

	/**
	* \brief Retourne l'existence d'un arc
	* \pre Les sommets doivent appartenir au graphe
	* \param[in] sommetUn (string) Nom de la ville d'ou l'arc a verifier part (depart)
	* \param[in] sommetDeux (string) Nom de la ville d'ou l'arc a verifier arrive (arrivee)
	* \return Boolean : True si arc existe, false sinon
	* \post Le graphe reste inchangé.
	* \exception logic_error L'un ou l'autre, ou les 2 sommets ne font pas partie du graphe
	*/
	bool arcExiste(const std::string& sommetUn, const std::string& sommetDeux) const;

	/**
	* \brief Retourne les pondérations d'un arc passé en argument
	* \pre L'arc doit exister
	* \param[in] sommetUn (string) Nom de la ville d'ou l'arc a verifier part (depart)
	* \param[in] sommetDeux (string) Nom de la ville d'ou l'arc a verifier arrive (arrivee)
	* \return Ponderations (struct) : Objet contenant duree(float), cout(float) et niveau de sec (int)
	* \post Le graphe reste inchangé.
	* \exception logic_error Le sommet source et/ou destination n'existent pas
	* \exception logic_error L'arc n'existe pas
	*/
	Ponderations getPonderationsArc(const std::string& sommetUn, const std::string& sommetDeux) const;

	/**
	* \brief Retourne les coordonnées d'un sommet
	* \pre Le sommet doit exister
	* \param[in] nom (string) Nom de la ville a verifier
	* \return Coordonnees (struct) : Objet contenant longitude(float) et latitude(float)
	* \post Le graphe reste inchangé.
	* \exception logic_error Il n'y a pas de sommets dans le graphe
	* \exception logic_error Le sommet n'existe pas
	*/
	Coordonnees getCoordonnesSommet(const std::string& nom) const;

	/**
	 * \brief Destruction d'un graphe
	 * \post Le graphe est détruit
	 */
	void detruireGraphe();



private:
	class Sommet;

	/**
	 * \class Arc
	 * \brief Description d'un arc qui est ici un trajet
	 * L'implémentation se fait avec une liste de successeurs.
	 */
	class Arc
	{
	public:
		Sommet * dest;			// Un pointeur sur la ville de destination
		Ponderations ponder;	// Les pondérations utilisées pour un trajet entre deux villes
		Arc *  suivDest;		// La prochaine ville de destination
	};

	/**
	 * \class Sommet
	 * \brief Description d'un sommet
	 * L'implémentation se fait avec une liste doublement chaînée.
	 */
	class Sommet
	{
	public:
		std::string nom;		// Le nom de la ville
		Coordonnees coord;		// Les coordonnées géospatiales de la ville
		Arc * listeDest;		// La liste des arcs entre la ville et les villes qui lui sont adjacentes
		bool etat;			    // Pour marquer une ville
		Sommet *precedent;		// La ville précédente dans la liste des villes
		Sommet *suivant;		// La prochaine ville dans la liste
	};

	int nbSommets;					// Le nombre de sommets dans le graphe
	int nbArcs;						// Le nombre des arcs dans le graphe
	Sommet * listeSommets;			// Liste des sommets du graphe


};

}//Fin du namespace

#endif
