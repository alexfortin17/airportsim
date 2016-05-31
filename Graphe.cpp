/**
 * \file Graphe.cpp
 * \brief Implémentation d'un graphe orienté.
 * \author ?
 * \version 0.1
 * \date ?
 *
 *  Travail pratique numéro 3
 *
 */

#include "Graphe.h"


namespace TP3
{

/**
* \brief      Constructeur par défaut
* \post       Un graphe vide est créé
*/
Graphe::Graphe():nbSommets(0), nbArcs(0), listeSommets(0){
}

/**
* \brief      Destructeur
* \post       Le graphe a été détruit
*/
Graphe::~Graphe(){
	detruireGraphe();
}

/**
* \brief Constructeur de copie.
* \pre Il y a assez de mémoire.
* \param[in] source (Graphe) objet Graphe a utiliser pour faire la copie profonde
* \exception bad_alloc s'il n'y a pas assez de mémoire.
*/
Graphe::Graphe(const Graphe &source){

		if(this != &source){
			nbSommets = 0;
			nbArcs = 0;
			listeSommets = 0;

			Sommet* somServSrc = source.listeSommets;
			while(somServSrc != 0){//on ajoute d'abord tous les sommets
				ajouterSommet(somServSrc->nom, somServSrc->coord.lt, somServSrc->coord.lg);
				somServSrc = somServSrc->suivant;
			}

			Sommet* somServSrc2 = source.listeSommets;
			while(somServSrc2 != 0){//on repasse a tous les sommets pour ajouter les arcs
				Arc* arcServSrc = somServSrc2->listeDest;
				while(arcServSrc != 0){
					ajouterArc(somServSrc2->nom, arcServSrc->dest->nom, arcServSrc->ponder.duree, arcServSrc->ponder.cout, arcServSrc->ponder.ns);
					arcServSrc = arcServSrc->suivDest;
				}

				somServSrc2 = somServSrc2->suivant;
			}


		}
}

/**
* \brief Surcharge de l'opérateur d'affectation.
* \pre Il doit y avoir assez de mémoire.
* \param[in] src (Graphe) objet Graphe a utiliser pour faire la copie profonde
* \post Le graphe a un contenu identique à src.
* \return Un objet Graphe (par ref)
* \exception bad_alloc s'il n'y a pas assez de mémoire.
*/
Graphe& Graphe::operator=(const Graphe& src){

	if(this != &src){
		nbSommets = 0;
		nbArcs = 0;
		listeSommets = 0;

		Sommet* somServSrc = src.listeSommets;
		while(somServSrc != 0){//on ajoute d'abord tous les sommets
			ajouterSommet(somServSrc->nom, somServSrc->coord.lt, somServSrc->coord.lg);
			somServSrc = somServSrc->suivant;
		}

		Sommet* somServSrc2 = src.listeSommets;
		while(somServSrc2 != 0){//on repasse a tous les sommets pour ajouter les arcs
			Arc* arcServSrc = somServSrc2->listeDest;
			while(arcServSrc != 0){
				ajouterArc(somServSrc2->nom, arcServSrc->dest->nom, arcServSrc->ponder.duree, arcServSrc->ponder.cout, arcServSrc->ponder.ns);
				arcServSrc = arcServSrc->suivDest;
			}

			somServSrc2 = somServSrc2->suivant;
		}


	}
	return *this;
}

/**
* \brief Ajout d'un sommet au graphe
* \param[in] nom (string) Nom de la ville
* \param[in] lt (float) Latitude
* \param[in] lg (float) Longitude
* \exception  bad_alloc Pas assez de mémoire pour alloué le nouvel arc
* \exception logic_error si le sommet existe deja dans le graphe
*/
void Graphe::ajouterSommet(const std::string& nom, float lt, float lg)
{
	// Vérifier si le sommet existe
	if(sommetExiste(nom)){
		throw std::logic_error("Graphe::ajouterSommet(): le sommet " + nom + " existe déjà " );
	}

	// Créer le sommet
	Sommet *nouveauSommet		= new Sommet;
	nouveauSommet->nom			= nom;
	nouveauSommet->coord.lt 	= lt;
	nouveauSommet->coord.lg 	= lg;
	nouveauSommet->listeDest	= 0;
	nouveauSommet->etat			= false;

	// Ajout du sommet à la liste des sommets
	if(nbSommets == 0){
		// Le graphe est vide
		nouveauSommet->precedent = 0;
		nouveauSommet->suivant = 0;
		nbSommets = 1;
		listeSommets = nouveauSommet;
	}
	else{
		// Le graphe n'est pas vide
		Sommet *service = listeSommets;
		while(service->suivant != 0)
		{
			service = service->suivant;
		}
		nouveauSommet->precedent	= service;
		nouveauSommet->suivant		= 0;
		service->suivant			= nouveauSommet;
		nbSommets++;
	}

}

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
void Graphe::ajouterArc(const std::string& nom1, const std::string& nom2, float duree, float cout, int ns){

	if(sommetExiste(nom1) == false || sommetExiste(nom2) == false){
		throw std::logic_error("Au moins un des deux sommets est inexistant.");
	}

	if(arcExiste(nom1, nom2) == true){
		throw std::logic_error("Il existe deja un arc entre ces 2 sommets.");
	}

	Graphe::Sommet* somServ = listeSommets;
	Graphe::Sommet* som1 = 0;
	Graphe::Sommet* som2 = 0;

	while(somServ != 0){
		if(somServ->nom == nom1){
			som1 = somServ;
		}
		if(somServ->nom == nom2){
			som2 = somServ;
		}

		if(som1!=0 && som2!=0){
			break;
		}
		somServ = somServ->suivant;
	}
	//sortie du break

	if(som1->listeDest == 0){//si le sommet n'a pas d'arc
		Graphe::Arc* arc = new Arc();
		arc->dest = som2;
		arc->suivDest = 0;
		arc->ponder.cout = cout;
		arc->ponder.duree = duree;
		arc->ponder.ns = ns;

		som1->listeDest = arc;//affecte la premiere destination du sommet a cet arc
	}

	else{//si le sommet a deja des arcs, on va placer le nouveau au debut
		Graphe::Arc* arc = new Arc();
		arc->dest = som2;
		arc->suivDest = som1->listeDest;
		arc->ponder.cout = cout;
		arc->ponder.duree = duree;
		arc->ponder.ns = ns;

		som1->listeDest = arc;
	}

	nbArcs++;


}

/**
* \brief Enlève un sommet du graphe ainsi que tous les arcs qui vont et partent de ce sommet
* \pre Le sommet doit exister
* \param[in] nom (string) Nom de la ville a enlever du graphe
* \post Le sommet identifié par 'nom' a été enlevé du graphe
* \exception logic_error Le sommet spécifié en argument n'existe pas
*/
void Graphe::enleverSommet(const std::string& nom){
	if(sommetExiste(nom) == false ){
		throw std::logic_error("Le sommet est inexistant.");
	}
	Sommet* somDel = listeSommets;

	while(somDel != 0){
		if(somDel->nom == nom){
			break;
		}
		somDel = somDel->suivant;
	}
	Arc* arcServ = somDel->listeDest;
	while(arcServ != 0){//delete tous les arcs dans le sommet
		enleverArc(somDel->nom, arcServ->dest->nom);
		arcServ = arcServ->suivDest;
	}

	Sommet* somServ = listeSommets;
	while(somServ != 0){//On supprimer tous les arcs de chaques sommets qui pointe vers le sommet a deleter
		Arc* arcServ2 = somServ->listeDest;
		while(arcServ2 != 0){
			if(arcServ2->dest->nom == nom){
				enleverArc(somServ->nom, somDel->nom);
			}
			arcServ2 = arcServ2->suivDest;
		}
		somServ = somServ->suivant;
	}

	//On doit ensuite isoler le sommet a deleter et le deleter
	if(somDel->precedent != 0 && somDel->suivant != 0){
	somDel->precedent->suivant = somDel->suivant;
	somDel->suivant->precedent = somDel->precedent;
	}
	else if(somDel->precedent == 0 && somDel->suivant != 0){
		somDel->suivant->precedent = 0;
		listeSommets = somDel->suivant;
	}
	else if(somDel->precedent != 0 && somDel->suivant == 0){
		somDel->precedent->suivant = 0;
	}
	else{//cas ou il y a un seul sommet
		listeSommets = 0;

	}
	delete somDel;
	nbSommets--;



}

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
void Graphe::enleverArc(const std::string& nom1, const std::string& nom2){
	if(sommetExiste(nom1) == false || sommetExiste(nom2) == false){
		throw std::logic_error("Au moins un des deux sommets est inexistant.");
	}

	if(arcExiste(nom1, nom2) == false){
		throw std::logic_error("Cet arc n'existe pas");
	}

	Graphe::Sommet* somServ = listeSommets;

	while(somServ != 0){
		if(somServ->nom == nom1){
			break;
		}
		somServ = somServ->suivant;
	}
	Arc* arcServ = somServ->listeDest;
	//Cas ou c'est le premier arc de la liste
	if(arcServ->dest->nom == nom2){
		somServ->listeDest = arcServ->suivDest;
		delete arcServ;
		nbArcs--;
		return;
	}
	while (arcServ != 0){
		if(arcServ->suivDest->dest->nom == nom2){
			Arc* arcADelete = arcServ->suivDest;//prend en memoire un pointeur sur arc a deleter
			arcServ->suivDest = arcServ->suivDest->suivDest;//on pointe la destination suivante sur la destination plus loin(ou 0)
			delete arcADelete;
			nbArcs--;
			break;
		}
		arcServ = arcServ->suivDest;
	}
}

/**
* \brief Retourne l'existence d'un sommet
* \param[in] nom (string) Nom de la ville a verifier
* \post Le graphe reste inchangé.
* \return Boolean : True si sommet existe, false sinon
*/
bool Graphe::sommetExiste(const std::string& nom) const{

	if(nbSommets == 0){
		return false;
	}

	Graphe::Sommet* somServ = listeSommets;

	while(somServ != 0){
		if(somServ->nom == nom){
			return true;
		}
		somServ = somServ->suivant;
	}
	return false;
}

/**
* \brief Retourne l'existence d'un arc
* \pre Les sommets doivent appartenir au graphe
* \param[in] sommetUn (string) Nom de la ville d'ou l'arc a verifier part (depart)
* \param[in] sommetDeux (string) Nom de la ville d'ou l'arc a verifier arrive (arrivee)
* \return Boolean : True si arc existe, false sinon
* \post Le graphe reste inchangé.
* \exception logic_error L'un ou l'autre, ou les 2 sommets ne font pas partie du graphe
*/
bool Graphe::arcExiste(const std::string& sommetUn, const std::string& sommetDeux) const{

	if(sommetExiste(sommetUn) == false || sommetExiste(sommetDeux) == false){
		throw std::logic_error("Au moins un des deux sommets est inexistant.");
	}
	Sommet* somServ = listeSommets;

	while(somServ != 0){
		if(somServ->nom == sommetUn){
			break;
		}
		somServ = somServ->suivant;
	}
	//sortie du break
	Sommet som = *somServ;
	if(som.listeDest == 0){//si le sommet n'a pas d'arc
		return false;
	}

	Arc* arcServ = som.listeDest;

	while(arcServ != 0){
		if(arcServ->dest->nom == sommetDeux){
			return true;
		}
		arcServ = arcServ->suivDest;
	}
	return false;


}

/**
* \brief Retourne la liste des noms des sommets dans un vecteur de chaînes de caractères
* \return vector : Vector contenant tous les sommets du graphe
* \post Le graphe reste inchangé.
*/
std::vector<std::string> Graphe::listerNomsSommets() const{

	std::vector<std::string> vec;
	if(nbSommets == 0){
		return vec;
	}
	Sommet* somServ = listeSommets;
	while (somServ != 0){
		vec.push_back(somServ->nom);
		somServ = somServ->suivant;
	}

	return vec;

}

/**
* \brief Retourne la liste des noms des sommets adjacents au sommet passé en argument dans un vecteur de chaînes de caractères
* \pre Le sommet doit appartenir au graphe
* \param[in] nom (string) Nom de la ville a verifier
* \return vector : Vector contenant tous les sommets adjacents au sommet nom
* \post Le graphe reste inchangé.
* \exception logic_error si le sommet n'existe pas
*/
std::vector<std::string> Graphe::listerSommetsAdjacents(const std::string& nom) const{

	if(sommetExiste(nom) == false ){
		throw std::logic_error("Le sommet est inexistant.");
	}

	std::vector<std::string> vec;

	Sommet* somServ = listeSommets;

	while(somServ != 0){
		if(somServ->nom == nom){
			break;
		}
		somServ = somServ->suivant;
	}
	//sortie du break
	Sommet som = *somServ;
	if(som.listeDest == 0){//si le sommet n'a pas d'arc
		return vec;
	}
	else{
		Arc* arcServ = som.listeDest;
		while(arcServ != 0){

			vec.push_back(arcServ->dest->nom);
			arcServ = arcServ->suivDest;

		}

		return vec;
	}
}

/**
* \brief Retourne le nombre de sommets dans la liste des sommets
* \return int : Nombre de sommets dans le graphe
* \post Le graphe reste inchangé.
*/
int Graphe::nombreSommets() const{
	return nbSommets;
}

/**
* \brief Retourne le nombre d'arcs
* \return int : Nombre d'arcs dans le graphe
* \post le graphe reste inchangé
*/
int Graphe::nombreArcs() const{
	return nbArcs;
}

/**
* \brief Indique si la liste des sommets est vide
* \return Boolean : True si graphe est vide, false sinon
* \post Le graphe reste inchangé
*/
bool Graphe::estVide() const{
	return nbSommets == 0;
}

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
Ponderations Graphe::getPonderationsArc(const std::string& sommetUn, const std::string& sommetDeux) const{
	if(sommetExiste(sommetUn) == false || sommetExiste(sommetDeux) == false){
		throw std::logic_error("Au moins un des deux sommets est inexistant.");
	}

	if(arcExiste(sommetUn, sommetDeux) == false){
		throw std::logic_error("Cet arc n'existe pas");
	}

	Sommet* somServ = listeSommets;

	while(somServ != 0){
		if(somServ->nom == sommetUn){
			break;
		}
		somServ = somServ->suivant;
	}
	//sortie du break
	Sommet som = *somServ;
	Arc* arcServ = som.listeDest;

	while(arcServ != 0){
		if(arcServ->dest->nom == sommetDeux){
			break;
		}
		arcServ = arcServ->suivDest;
	}
	return arcServ->ponder;
}

/**
* \brief Retourne les coordonnées d'un sommet
* \pre Le sommet doit exister
* \param[in] nom (string) Nom de la ville a verifier
* \return Coordonnees (struct) : Objet contenant longitude(float) et latitude(float)
* \post Le graphe reste inchangé.
* \exception logic_error Il n'y a pas de sommets dans le graphe
* \exception logic_error Le sommet n'existe pas
*/
Coordonnees Graphe::getCoordonnesSommet(const std::string& nom) const{
	if(sommetExiste(nom) == false ){
		throw std::logic_error("Le sommet est inexistant.");
	}
	Sommet* somServ = listeSommets;

	while(somServ != 0){
		if(somServ->nom == nom){
			break;
		}
		somServ = somServ->suivant;
	}
	//sortie du break
	return somServ->coord;
}

/**
* \brief Destruction d'un graphe
* \post Le graphe est détruit
*/
void Graphe::detruireGraphe(){
	Sommet* somServ = listeSommets;
	std::vector<std::string> vec;
	while(somServ != 0){
		vec.push_back(somServ->nom);
		somServ = somServ->suivant;
	}
	for(std::vector<std::string>::iterator iter = vec.begin(); iter != vec.end(); iter++){
		enleverSommet(*iter);
	}
}

}//Fin du namespace
