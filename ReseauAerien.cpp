/**
 * \file ReseauAerien.cpp
 * \brief Implémentattion de la classe ReseauAerien.
 * \author ?
 * \version 0.1
 * \date ?
 *
 *  Travail pratique numéro 3
 *
 */
#include <sstream>
#include <fstream>
#include "ReseauAerien.h"
#include <string>
#include <limits>
#include <algorithm>
#include <vector>


namespace TP3
{


/**
* \brief Constructeur de copie.
* \pre Il y a assez de mémoire.
* \param[in] source : L'objet ReseauAerien sur lequel on base la copie profonde
* \exception bad_alloc S'il n'y a pas assez de mémoire.
*/
ReseauAerien::ReseauAerien(const ReseauAerien &source){
	if(this != &source){
		unReseau = source.unReseau;
		nomReseau = source.nomReseau;
	}
}

/**
* \brief Surcharge de l'opérateur d'affectation.
* \pre Il doit y avoir assez de mémoire.
* \param[in] src : L'objet ReseauAerien sur lequel on base la copie profonde
* \return ReseauAerien (objet) : Un objet reseauAerien
* \post Le réseau aérien a un contenu identique à src.
* \exception bad_alloc S'il n'y a pas assez de mémoire.
*/
ReseauAerien& ReseauAerien::operator=(const ReseauAerien& src){
	if(this != &src){
		unReseau = src.unReseau;
		nomReseau = src.nomReseau;
	}
	return *this;
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
void ReseauAerien::chargerReseau(std::ifstream & fichierEntree)
{
	if(fichierEntree.is_open() == false){
		throw std::logic_error("Le fichier n'a pas ete prealablement ouvert!");
	}
	std::string buff;
	getline(fichierEntree, nomReseau);
	nomReseau.erase(0, 15);

	int nbVilles;
	int nbTrajets;

	fichierEntree >> nbVilles;
	getline(fichierEntree, buff); //villes
	fichierEntree >> nbTrajets;
	getline(fichierEntree, buff); //trajets


	getline(fichierEntree, buff); //Liste des villes

	while(getline(fichierEntree, buff) && buff != "Liste des trajets:")
	{

		std::string ville = buff;

		getline(fichierEntree, buff);
		std::istringstream iss(buff);

		float lt;
		iss >> lt;

		float lg;
		iss >> lg;

		unReseau.ajouterSommet(ville, lt, lg);
	}

	while(getline(fichierEntree, buff))
	{
		std::string ville1 = buff;

		getline(fichierEntree, buff);
		std::string ville2 = buff;

		getline(fichierEntree, buff);
		std::istringstream iss(buff);

		float duree;
		iss >> duree;

		float cout;
		iss >> cout;

		int ns;
		iss >> ns;

		unReseau.ajouterArc(ville1, ville2, duree, cout, ns);
	}
}

/**
* \brief Sauvegarder un réseau dans un fichier texte (voir format du fichier dans l'énoncé du Tp).
* \pre SortieFichier est ouvert correctement.
* \param[in] SortieFichier (ofstream) fichier ou on va ecrire le reseau
* \post SortieFichier n'est pas fermé par la fonction.
* \post Si les préconditions sont respectées, les données du réseau sont sauvegardées dans le fichier.
* \exception logic_error si SortieFichier n'est pas ouvert correctement.
* \excpetion logic_error si tout crash dans la fonction de sauvegarde (renvoi message erreur)
*/
void ReseauAerien::sauvegarderReseau(std::ofstream & SortieFichier) const{
	if(SortieFichier.is_open() == false){
		throw std::logic_error("Le ofStream n'a pas ete prealablement ouvert");
	}
	try{
		std::vector<std::string> vecSom = unReseau.listerNomsSommets();
		std::ostringstream ss0;
		ss0 << unReseau.nombreSommets();
		std::string strNbSom(ss0.str());
		ss0.str("");
		ss0.clear();
		ss0 << unReseau.nombreArcs();
		std::string strNbArc(ss0.str());
		ss0.str("");
		ss0.clear();

		SortieFichier << "Reseau Aerien: " + nomReseau + "\n";
		SortieFichier << strNbSom + " villes\n";
		SortieFichier << strNbArc + " trajets\n";
		SortieFichier << "Liste des villes:\n";
		//on print tous les villes (sommets) et leurs coordonnees
		for(std::vector<std::string>::const_iterator iter = vecSom.begin(); iter != vecSom.end(); iter++){
			SortieFichier << *iter + "\n";
			Coordonnees xy = unReseau.getCoordonnesSommet(*iter);
			std::ostringstream ss;
			ss << xy.lt;
			std::string strlt(ss.str());
			ss.str("");
			ss.clear();
			ss << xy.lg;
			std::string strlg(ss.str());
			ss.str("");
			ss.clear();
			SortieFichier << strlt + " " + strlg + "\n";
		}

		SortieFichier << "Liste des trajets:\n";

		for(std::vector<std::string>::const_iterator iterSrc = vecSom.begin(); iterSrc != vecSom.end(); iterSrc++){//pour chaques sommets
			std::vector<std::string> VecSomAdj = unReseau.listerSommetsAdjacents(*iterSrc);//pour ch arc du sommet
			for(std::vector<std::string>::const_iterator iterDesti = VecSomAdj.begin(); iterDesti != VecSomAdj.end(); iterDesti++){
				SortieFichier << *iterSrc + "\n";
				SortieFichier << *iterDesti + "\n";
				Ponderations x = unReseau.getPonderationsArc(*iterSrc, *iterDesti);
				std::ostringstream ss2;
				ss2 << x.duree;
				std::string strdur(ss2.str());
				ss2.str("");
				ss2.clear();
				ss2 << x.cout;
				std::string strcout(ss2.str());
				ss2.str("");
				ss2.clear();
				ss2 << x.ns;
				std::string strns(ss2.str());
				ss2.str("");
				ss2.clear();
				SortieFichier << strdur + " " + strcout + " " + strns + "\n";
			}
		}
	}
	catch(std::exception& ex){
		throw std::logic_error("Une erreur est survenu lors de la sauvegarde du fichier");
	}



}

/**
* \brief Déterminer la fermeture transitive du reseau.
* \pre Le reseau est correctement initialisé.
* \post La fermeture transitive du reseau est retournée.
* \post Le reseau original reste inchangé.
* \return ReseauAerien : Objet reseauAerien (fermeture transitive de reseau courant)
* \exception bad_alloc si pas assez de mémoire
*/
ReseauAerien ReseauAerien::fermetureReseau() const{

	ReseauAerien reseauFerme = *this;

	std::vector<std::string> vecSom = reseauFerme.unReseau.listerNomsSommets();
	for(std::vector<std::string>::const_iterator iterk = vecSom.begin(); iterk != vecSom.end(); iterk++){
		for(std::vector<std::string>::const_iterator iteri = vecSom.begin(); iteri != vecSom.end(); iteri++){
			for(std::vector<std::string>::const_iterator iterj = vecSom.begin(); iterj != vecSom.end(); iterj++){
				if(reseauFerme.unReseau.arcExiste(*iteri, *iterk) && reseauFerme.unReseau.arcExiste(*iterk, *iterj) &&
						reseauFerme.unReseau.arcExiste(*iteri,*iterj) == false ){//&& *iteri != *iterj
					reseauFerme.unReseau.ajouterArc(*iteri, *iterj, 0.0f,0.0f,0);
				}
			}
		}
	}

	return reseauFerme;


}

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
Chemin ReseauAerien::rechercheCheminDijkstra(const std::string &origine, const std::string &destination, bool dureeCout) const{
	if(unReseau.sommetExiste(origine)==false || unReseau.sommetExiste(destination)==false){
		throw std::logic_error("Au moins un de ces sommets est inexistants");
	}

	std::vector<std::string> vecV = unReseau.listerNomsSommets();
	std::string NomPremier = vecV[0];//on swap la ville source pour la mettre au debut du vecteur(si elle ne l'est pas deja)
	if(NomPremier != origine){
		for(std::vector<std::string>::iterator itera = vecV.begin(); itera != vecV.end(); itera++){
			if (*itera == origine){
				vecV[0] = origine;
				*itera = NomPremier;
			}
		}
	}

	//std::vector<std::string> vecT = vecV;


	//std::vector<std::string> vecS;//Vec S est initialement vide. On va y deplacer les sommets traites a mesure

	//On initialise le vecteur P avec des max value de float sauf le premier element qui est egal a 0
	std::vector<float> vecY(unReseau.listerNomsSommets().size(), std::numeric_limits<float>::max());
	vecY[0] = 0.00f;

	//on initialise le vec avec des empty strings
	std::vector<std::string> vecP(unReseau.listerNomsSommets().size(), "");

	for(std::vector<std::string>::const_iterator iterV = vecV.begin(); iterV != vecV.end(); iterV++){
		std::vector<std::string> vecDesti = unReseau.listerSommetsAdjacents(*iterV);//les sommets adjacent au sommet traite
		for(std::vector<std::string>::const_iterator iterDesti = vecDesti.begin(); iterDesti != vecDesti.end(); iterDesti++){//Pour chaque destination du sommet traite
			Ponderations xyz = unReseau.getPonderationsArc(*iterV, *iterDesti);
			std::vector<std::string>::iterator iterDestiV = find(vecV.begin(), vecV.end(), *iterDesti);//ramasse la position du sommet desti dans le gros vecteur V

			if(dureeCout == true){//si duree
				if(vecY[iterV - vecV.begin()] + xyz.duree < vecY[iterDestiV - vecV.begin()]){//si chemin en 2 temps < longeur actuel, switch
					vecY[iterDestiV - vecV.begin()] = vecY[iterV - vecV.begin()] + xyz.duree;//on change le float dans vecY
					vecP[iterDestiV - vecV.begin()] = *iterV;//on change le sommet precedent dans VecP
				}
			}
			else{//si cout
				if(vecY[iterV - vecV.begin()] + xyz.cout < vecY[iterDestiV - vecV.begin()]){
					vecY[iterDestiV - vecV.begin()] = vecY[iterV - vecV.begin()] + xyz.cout;
					vecP[iterDestiV - vecV.begin()] = *iterV;
				}
			}
		}
	}
	//Fin de l'algo de Dijkstra
	Chemin chemin;
	std::vector<std::string>::iterator iterDestiFinal = find(vecV.begin(), vecV.end(), destination);
	int PosiDestiFinal = iterDestiFinal - vecV.begin();
	if(vecY[PosiDestiFinal] == std::numeric_limits<float>::max()){//si pas de chemin on return false
		chemin.reussi = false;
		return chemin;
	}
	else{
		chemin.reussi = true;
		//float tempfloat = 0.0;
		std::string villeTemp = vecV[PosiDestiFinal];
		int posiTemp = PosiDestiFinal;
		std::vector<std::string> vecVilles;
		vecVilles.push_back(villeTemp);//push la desti final dans le vector
		while(villeTemp != origine){
			//tempfloat = tempfloat+vecY[posiTemp];
			villeTemp = vecP[posiTemp];
			vecVilles.push_back(villeTemp);
			posiTemp = find(vecV.begin(), vecV.end(), villeTemp) - vecV.begin();
		}

		if(dureeCout == true){
			chemin.dureeTotale = vecY[PosiDestiFinal];
		}
		else{
			chemin.coutTotal = vecY[PosiDestiFinal];
		}
		std::reverse(vecVilles.begin(), vecVilles.end());//cela remet les villes dans le bonne ordre
		chemin.listeVilles = vecVilles;
		return chemin;
	}






}

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
Chemin ReseauAerien::rechercheCheminBellManFord(const std::string &origine, const std::string &destination, int dureeCoutNiveau) const{
	if(unReseau.sommetExiste(origine)==false || unReseau.sommetExiste(destination)==false){
		throw std::logic_error("Au moins un de ces sommets est inexistants");
	}
	if(dureeCoutNiveau < 1 || dureeCoutNiveau > 3){
		throw std::logic_error("dureeCoutNiveau doit avoir une valeur entre 1 et 3");
	}

	std::vector<std::string> vecV = unReseau.listerNomsSommets();
	std::string NomPremier = vecV[0];//on swap la ville source pour la mettre au debut du vecteur(si elle ne l'est pas deja)
	if(NomPremier != origine){
		for(std::vector<std::string>::iterator itera = vecV.begin(); itera != vecV.end(); itera++){
			if (*itera == origine){
				vecV[0] = origine;
				*itera = NomPremier;
			}
		}
	}

	//On initialise le vecteur P avec des max value de float sauf le premier element qui est egal a 0
	std::vector<float> vecY(unReseau.listerNomsSommets().size(), std::numeric_limits<float>::max());
	vecY[0] = 0.00f;

	//on initialise le vec avec des empty strings
	std::vector<std::string> vecP(unReseau.listerNomsSommets().size(), "");
	Chemin chemin;
	unsigned int cpt = 0;
	while(cpt <= unReseau.listerNomsSommets().size()){
		if(cpt == unReseau.listerNomsSommets().size()){//si j'ai passer une niemer fois dans la loop et j'ai relache, boucle neg
			chemin.reussi = false;
			return chemin;
		}
		bool relachement = false;
		for(std::vector<std::string>::const_iterator iterV = vecV.begin(); iterV != vecV.end(); iterV++){
			std::vector<std::string> vecDesti = unReseau.listerSommetsAdjacents(*iterV);//les sommets adjacent au sommet traite
			for(std::vector<std::string>::const_iterator iterDesti = vecDesti.begin(); iterDesti != vecDesti.end(); iterDesti++){//Pour chaque destination du sommet traite
				Ponderations xyz = unReseau.getPonderationsArc(*iterV, *iterDesti);
				std::vector<std::string>::iterator iterDestiV = find(vecV.begin(), vecV.end(), *iterDesti);//ramasse la position du sommet desti dans le gros vecteur V

				if(dureeCoutNiveau == 1){//si duree
					if(vecY[iterV - vecV.begin()] + xyz.duree < vecY[iterDestiV - vecV.begin()]){//si chemin en 2 temps < longeur actuel, switch
						vecY[iterDestiV - vecV.begin()] = vecY[iterV - vecV.begin()] + xyz.duree;//on change le float dans vecY
						vecP[iterDestiV - vecV.begin()] = *iterV;//on change le sommet precedent dans VecP
						relachement = true;
					}
				}
				else if(dureeCoutNiveau == 2){//si cout
					if(vecY[iterV - vecV.begin()] + xyz.cout < vecY[iterDestiV - vecV.begin()]){
						vecY[iterDestiV - vecV.begin()] = vecY[iterV - vecV.begin()] + xyz.cout;
						vecP[iterDestiV - vecV.begin()] = *iterV;
						relachement = true;
					}
				}
				else{//si niveau de sec
					if(vecY[iterV - vecV.begin()] + xyz.ns < vecY[iterDestiV - vecV.begin()]){
						vecY[iterDestiV - vecV.begin()] = vecY[iterV - vecV.begin()] + xyz.ns;
						vecP[iterDestiV - vecV.begin()] = *iterV;
						relachement = true;
					}
				}
			}
		}
		if(relachement == false){
			goto SUITE;
		}
		cpt++;
	}

	SUITE://fin BellmanFord car loop sans relachement
	std::vector<std::string>::iterator iterDestiFinal = find(vecV.begin(), vecV.end(), destination);
	int PosiDestiFinal = iterDestiFinal - vecV.begin();
	if(vecY[PosiDestiFinal] == std::numeric_limits<float>::max()){//si pas de chemin on return false
		chemin.reussi = false;
		return chemin;
	}
	else{
		chemin.reussi = true;
		std::string villeTemp = vecV[PosiDestiFinal];
		int posiTemp = PosiDestiFinal;
		std::vector<std::string> vecVilles;
		vecVilles.push_back(villeTemp);//push la desti final dans le vector
		while(villeTemp != origine){
			villeTemp = vecP[posiTemp];
			vecVilles.push_back(villeTemp);
			posiTemp = find(vecV.begin(), vecV.end(), villeTemp) - vecV.begin();
		}


		if(dureeCoutNiveau == 1){
			chemin.dureeTotale = vecY[PosiDestiFinal];
		}
		else if(dureeCoutNiveau == 2){
			chemin.coutTotal = vecY[PosiDestiFinal];
		}
		else{
			chemin.nsTotal = vecY[PosiDestiFinal];
		}
		std::reverse(vecVilles.begin(), vecVilles.end());//cela remet les villes dans le bonne ordre
		chemin.listeVilles = vecVilles;
		return chemin;

	}





}



}//Fin du namespace
