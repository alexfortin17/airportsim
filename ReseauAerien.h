/**
 * \file ReseauAerien.h
 * \brief Gestion d'un réseau aérien.
 * \author Mondher Bouden
 * \version 0.1
 * \date Juillet 2015
 *
 *  Travail pratique numéro 3
 *
 */

#include "Graphe.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <climits>
#include <vector>
//vous pouvez inclure d'autres librairies de la STL si vous pensez les utiliser


#ifndef _RESEAUAERIEN__H
#define _RESEAUAERIEN__H

namespace TP3
{

struct Chemin 
{
   //listeVilles doit contenir la liste des villes formant le chemin
   //Le point de départ du chemin correspond à l'élément d'indice 0 du vecteur
   //le point d'arrivée correspond au dernier élément du vecteur.
   std::vector<std::string> listeVilles;

   float dureeTotale;		// Durée totale du vol du chemin en heures incluant le temps d'escale
   float coutTotal;			// Cout total en dollars du chemin entre la ville de départ et la ville de destination
   int nsTotal;				// Niveau de sécurité total du chemin
   bool reussi;				// Un booléen qui vaut true si chemin a été trouvé, false sinon
};

class ReseauAerien{

public:

	/**                       
	 * \brief constructeur par défaut
	 * \post Un réseau aérien vide est instancié.
	 * Ne fait rien !
	 */
	ReseauAerien(){};

	/**                       
	 * \brief Détruit et libère toute la mémoire allouée auparavant pour le réseau aérien.
	 * \post Toute la mémoire allouée auparavant pour le réseau aérien est libérée.
	 * \post L'Objet ReseauAerien n'est plus valide.
	 * Ne fait rien !
	 */
	~ReseauAerien(){};

	/**
	* \brief Constructeur de copie.
	* \pre Il y a assez de mémoire.
	* \param[in] source : L'objet ReseauAerien sur lequel on base la copie profonde
	* \exception bad_alloc S'il n'y a pas assez de mémoire.
	*/
	ReseauAerien(const ReseauAerien &source);
        
	/**
	* \brief Surcharge de l'opérateur d'affectation.
	* \pre Il doit y avoir assez de mémoire.
	* \param[in] src : L'objet ReseauAerien sur lequel on base la copie profonde
	* \return ReseauAerien (objet) : Un objet reseauAerien
	* \post Le réseau aérien a un contenu identique à src.
	* \exception bad_alloc S'il n'y a pas assez de mémoire.
	*/
	ReseauAerien& operator=(const ReseauAerien& src);

	/**
	* \brief Surcharge de l'opérateur de sortie.
	* \param[in] out (ostream) : stream ou on print le graphe
	* \param[in] g : graphe que l'on veut printer
	* \return ostream
	* \post Le reseau aérien sera affiché
	*/
	friend std::ostream& operator<<(std::ostream& out, const ReseauAerien& g)
	{
		out << g.unReseau << std::endl;
		return out;
	}

	/**                       
	* \brief Charger un réseau à partir d'un fichier texte en entrée (voir format du fichier dans l'énoncé du Tp).
	*  Construit un reseau aérien à partir d'une liste de villes ainsi que leurs liens.
	* \pre Il y a assez de mémoire pour charger toutes les villes et les trajets du réseau.
	* \pre fichierEntree est ouvert corectement.
	* \parma[in] fichierEntree (ifstrem) : fichier duquel on charge le reseau
	* \post fichierEntree n'est pas fermé par la fonction.
	* \post Si les préconditions sont respectées, les données du réseau contenu
	*       dans le fichier en entrée sont organisées dans un graphe en mémoire.
	* \exception bad_alloc si pas assez de mémoire pour contenir toute la liste du fichier.
	* \exception logic_error si fichierEntree n'est pas ouvert correctement.
	*/
	void chargerReseau(std::ifstream & fichierEntree); 

	/**
	* \brief Sauvegarder un réseau dans un fichier texte (voir format du fichier dans l'énoncé du Tp).
	* \pre SortieFichier est ouvert correctement.
	* \param[in] SortieFichier (ofstream) fichier ou on va ecrire le reseau
	* \post SortieFichier n'est pas fermé par la fonction.
	* \post Si les préconditions sont respectées, les données du réseau sont sauvegardées dans le fichier.
	* \exception logic_error si SortieFichier n'est pas ouvert correctement.
	* \excpetion logic_error si tout crash dans la fonction de sauvegarde (renvoi message erreur)
	*/
	void sauvegarderReseau(std::ofstream & SortieFichier) const;

	/**
	* \brief Déterminer la fermeture transitive du reseau.
	* \pre Le reseau est correctement initialisé.
	* \post La fermeture transitive du reseau est retournée.
	* \post Le reseau original reste inchangé.
	* \return ReseauAerien : Objet reseauAerien (fermeture transitive de reseau courant)
	* \exception bad_alloc si pas assez de mémoire
	*/
	ReseauAerien fermetureReseau() const;

	/**
	* \brief Plus court chemin selon l'algorithme de Dijkstra
	* \pre origine et destination font partie du graphe
	* \pre dureeCout: true ou false:true si on utilise la duree, false si on utilise le cout
	* \param[in] origine (string) Nom de la ville origine
	* \param[in] destination (string) Nom de la ville destination
	* \param[in] dureeCout (bool) true si compare par duree, false si compare par cout
	* \return chemin : objet chemin contenant les informations resultants de l'algorithme
	* \post Une structure Chemin contenant le produit de l'algorithme est retournée
	* \exception std::logic_error si origine et destination absent du graphe
	*/
	Chemin rechercheCheminDijkstra(const std::string &origine, const std::string &destination, bool dureeCout) const;

	/**
	* \brief Plus court chemin selon l'algorithme Bellman-Ford
	* \pre origine et destination font partie du graphe
	* \pre 1 <= dureeCoutNiveau <= 3 (voir énoncé du TP)
	* \param[in] origine (string) Nom de la ville origine
	* \param[in] destination (string) Nom de la ville destination
	* \param[in] dureeCoutNiveau (int) 1 si compare par duree, 2 si compare par cout, 3 si compare pas niveau de securite
	* \return chemin : objet chemin contenant les informations resultants de l'algorithme
	* \post Une structure Chemin contenant le produit de l'algorithme est retourné
	* \exception std::logic_error si dureeCoutNiveau hors limite
	* \exception std::logic_error si sommets pas dans le graphe
	*/
	Chemin rechercheCheminBellManFord(const std::string &origine, const std::string &destination, int dureeCoutNiveau) const;


	/**
	* \brief Permet d;obtenir le nom du reseau par ref
	* \return string : nom du reseau
	*/
	 std::string& getNomReseau()  {
		return nomReseau;
	}

	 /**
	* \brief Permet d'obtenir le graphe du reseau pour pouvoir faire les manipulations diponibles publiquement dans la classe Graphe
	* \return string : nom du reseau
	*/
	Graphe& getUnReseau()  {
		return unReseau;
	}



private:
	Graphe unReseau;			//Le type ReseauAerien est composé d'un graphe
	std::string nomReseau;		// Le nom du reseau (exemple: Air Canada)


};

}//Fin du namespace

#endif
