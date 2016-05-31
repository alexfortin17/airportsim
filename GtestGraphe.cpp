/*
 * GtestGraphe.cpp
 *
 *  Created on: Jul 7, 2015
 *      Author: alex
 */

#include "Graphe.h"
#include "ReseauAerien.h"
#include "gtest/gtest.h"
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace TP3;

TEST(GrapheVide, Base) {
	Graphe ge;
	ASSERT_EQ(ge.estVide(), true);
	ASSERT_EQ(ge.nombreArcs(), 0);
	ASSERT_EQ(ge.nombreSommets(), 0);

}


TEST(GrapheVide, Throw) {
	Graphe ge;
	EXPECT_THROW(ge.enleverSommet("Honolulu"), std::logic_error);
	EXPECT_THROW(ge.enleverArc("Honolulu", "Vancouver"), std::logic_error);
}

TEST(GrapheVide, insertionSommets) {
	Graphe ge;
	ge.ajouterSommet("Montreal", 9.3, 7.2);
	ASSERT_EQ(ge.estVide(), false);
	ASSERT_EQ(ge.nombreArcs(), 0);
	ASSERT_EQ(ge.nombreSommets(), 1);
	ge.ajouterSommet("Quebec", 9.8, 4.2);
	ASSERT_EQ(ge.nombreArcs(), 0);
	ASSERT_EQ(ge.nombreSommets(), 2);
	ge.ajouterSommet("Toronto", 10.6, 41.23);
	ASSERT_EQ(ge.nombreSommets(), 3);
	ge.ajouterSommet("Ottawa", 8.6, 21.23);
	ASSERT_EQ(ge.nombreSommets(), 4);

	ge.ajouterArc("Montreal", "Quebec", 0.45, 125.33, 2);
	ge.ajouterArc("Montreal", "Ottawa", 0.30, 133.33, 2);
	ge.ajouterArc("Montreal", "Toronto", 1.30, 233.33, 4);
	ge.ajouterArc("Quebec", "Toronto", 2.30, 533.33, 3);
	ASSERT_EQ(ge.nombreArcs(), 4);

	ge.enleverSommet("Montreal");
	ASSERT_EQ(ge.estVide(), false);
	ASSERT_EQ(ge.nombreSommets(), 3);
	ge.enleverSommet("Ottawa");
	ASSERT_EQ(ge.nombreSommets(), 2);
	ge.enleverSommet("Toronto");
	ASSERT_EQ(ge.nombreSommets(), 1);
	ge.enleverSommet("Quebec");
	ASSERT_EQ(ge.nombreSommets(), 0);
	ASSERT_EQ(ge.nombreArcs(), 0);
	ASSERT_EQ(ge.estVide(), true);
}

class GETest: public ::testing::Test {
protected:
	virtual void SetUp() {

		ge.ajouterSommet("Montreal", 9.3, 7.2);
		ge.ajouterSommet("Quebec", 9.8, 4.2);
		ge.ajouterSommet("Toronto", 10.6, 41.23);
		ge.ajouterSommet("Ottawa", 8.6, 21.23);
		ge.ajouterArc("Montreal", "Quebec", 0.45, 125.33, 2);
		ge.ajouterArc("Montreal", "Ottawa", 0.30, 133.33, 2);
		ge.ajouterArc("Montreal", "Toronto", 1.30, 233.33, 4);
		ge.ajouterArc("Quebec", "Toronto", 2.30, 533.33, 3);
		ge.ajouterArc("Toronto", "Ottawa", 1.30, 333.33, 3);


	}
	// virtual void TearDown() {}
	Graphe ge;
};

TEST_F(GETest, VerifInit){
	ASSERT_EQ(ge.nombreArcs(), 5);
	ASSERT_EQ(ge.estVide(), false);
	ASSERT_EQ(ge.nombreSommets(), 4);
	ASSERT_EQ(ge.arcExiste("Montreal", "Quebec"), true);
	ASSERT_EQ(ge.arcExiste("Quebec", "Montreal"), false);
	ASSERT_EQ(ge.getCoordonnesSommet("Quebec").lt, 9.8f);
	ASSERT_EQ(ge.getCoordonnesSommet("Quebec").lg, 4.2f);
	ASSERT_EQ(ge.getPonderationsArc("Quebec", "Toronto").cout, 533.33f);
	ASSERT_EQ(ge.getPonderationsArc("Quebec", "Toronto").duree, 2.30f);
	ASSERT_EQ(ge.getPonderationsArc("Quebec", "Toronto").ns, 3);
	ASSERT_EQ(ge.sommetExiste("Toronto"), true);
	ASSERT_EQ(ge.sommetExiste("Bagdhah"), false);

	std::vector<string> vec1;
	std::vector<string> vec2;
	std::vector<string> vec3;
	vec1 = ge.listerNomsSommets();
	ASSERT_EQ(vec1.size(), 4);
	vec2 = ge.listerSommetsAdjacents("Montreal");
	ASSERT_EQ(vec2.size(), 3);
	vec2 = ge.listerSommetsAdjacents("Ottawa");
	ASSERT_EQ(vec3.size(), 0);
}

TEST_F(GETest, SuppressionsArcs){
	ge.enleverArc("Montreal", "Quebec");
	ASSERT_EQ(ge.nombreArcs(), 4);
	ge.enleverArc("Montreal", "Ottawa");
	ASSERT_EQ(ge.nombreArcs(), 3);
	ge.enleverArc("Quebec", "Toronto");
	ASSERT_EQ(ge.nombreArcs(), 2);

}

TEST_F(GETest, SuppressionsSommets){
	ge.enleverSommet("Quebec");
	EXPECT_EQ(ge.nombreArcs(), 3);
	EXPECT_EQ(ge.nombreSommets(), 3);
	ge.enleverSommet("Montreal");
	EXPECT_EQ(ge.nombreArcs(), 1);
	EXPECT_EQ(ge.nombreSommets(), 2);


}

TEST_F(GETest, ContructeurCopyOpEg){
	Graphe ge2 = ge;
	ge2.enleverArc("Montreal", "Quebec");
	EXPECT_EQ(ge.nombreArcs(), 5);
	EXPECT_EQ(ge2.nombreArcs(), 4);

	Graphe ge3;
	ge3 = ge;
	ge3.enleverArc("Montreal", "Quebec");
	EXPECT_EQ(ge.nombreArcs(), 5);
	EXPECT_EQ(ge3.nombreArcs(), 4);

	EXPECT_EQ(ge2.nombreSommets(), 4);
	EXPECT_EQ(ge3.nombreSommets(), 4);


}



class RATest: public ::testing::Test {
protected:
	virtual void SetUp() {

		ifstream EntreeFichier;
		EntreeFichier.open("ReseauAerien.txt", ios::in);
		if(EntreeFichier.is_open())
		{
			ra.chargerReseau(EntreeFichier);
		}
		EntreeFichier.close();


	}
	// virtual void TearDown() {}
	ReseauAerien ra;
};

TEST_F(RATest, ContructeurCopyOpEg){
	ReseauAerien ra2 = ra;
	ra2.getUnReseau().enleverArc("Chicago", "Vancouver");

	EXPECT_EQ(ra.getUnReseau().nombreArcs(), 20);
	EXPECT_EQ(ra2.getUnReseau().nombreArcs(), 19);


}

TEST_F(RATest, Fermeture){
	ReseauAerien ra2 = ra.fermetureReseau();
	EXPECT_EQ(ra2.getUnReseau().nombreArcs(), 63);

}

TEST_F(RATest, Dijkstra){
	/*std::vector<int> vec1(6,6);
	for(std::vector<int>::const_iterator iterDesti = vec1.begin(); iterDesti != vec1.end(); iterDesti++){
		cout << iterDesti - vec1.begin() << std::endl;
	}*/

	Chemin chem1 = ra.rechercheCheminDijkstra("Quebec", "Bogota", true);
	EXPECT_EQ(chem1.reussi, true);
	EXPECT_EQ(chem1.dureeTotale, 7.24f);
	EXPECT_EQ(chem1.listeVilles.size(), 4);
	EXPECT_EQ(chem1.listeVilles[0], "Quebec");
	EXPECT_EQ(chem1.listeVilles[1], "Montreal");
	EXPECT_EQ(chem1.listeVilles[2], "Toronto");
	EXPECT_EQ(chem1.listeVilles[3], "Bogota");

	Chemin chem2 = ra.rechercheCheminDijkstra("Quebec", "Bogota", false);
	EXPECT_EQ(chem2.reussi, true);
	//EXPECT_EQ(chem2.coutTotal, 1444.59f);ok
	EXPECT_EQ(chem2.listeVilles.size(), 4);
	EXPECT_EQ(chem2.listeVilles[0], "Quebec");
	EXPECT_EQ(chem2.listeVilles[1], "Montreal");
	EXPECT_EQ(chem2.listeVilles[2], "Toronto");
	EXPECT_EQ(chem2.listeVilles[3], "Bogota");

	Chemin chem3 = ra.rechercheCheminDijkstra("Quebec", "Iqaluit", true);
	EXPECT_EQ(chem3.reussi, true);
	EXPECT_EQ(chem3.dureeTotale, 4.67f);
	EXPECT_EQ(chem3.listeVilles.size(), 4);

	Chemin chem4 = ra.rechercheCheminDijkstra("Quebec", "Iqaluit", false);
	EXPECT_EQ(chem4.reussi, true);
	EXPECT_EQ(chem4.coutTotal, 1140.66f);
	EXPECT_EQ(chem4.listeVilles.size(), 4);

	Chemin chem5 = ra.rechercheCheminDijkstra("Iqaluit", "Quebec", false);
	EXPECT_EQ(chem5.reussi, false);

	Chemin chem6 = ra.rechercheCheminDijkstra("Quebec", "Vancouver", true);
	EXPECT_EQ(chem6.reussi, true);
	EXPECT_EQ(chem6.listeVilles[1], "Montreal");
	EXPECT_EQ(chem6.dureeTotale, 3.55f);
}

TEST_F(RATest, BellmanFord){

	Chemin chem1 = ra.rechercheCheminBellManFord("Quebec", "Bogota", 1);
	EXPECT_EQ(chem1.reussi, true);
	EXPECT_EQ(chem1.dureeTotale, 7.24f);
	EXPECT_EQ(chem1.listeVilles.size(), 4);
	EXPECT_EQ(chem1.listeVilles[0], "Quebec");
	EXPECT_EQ(chem1.listeVilles[1], "Montreal");
	EXPECT_EQ(chem1.listeVilles[2], "Toronto");
	EXPECT_EQ(chem1.listeVilles[3], "Bogota");

	Chemin chem2 = ra.rechercheCheminBellManFord("Quebec", "Bogota", 2);
	EXPECT_EQ(chem2.reussi, true);
	//EXPECT_EQ(chem2.coutTotal, 1444.59);ok
	EXPECT_EQ(chem2.listeVilles.size(), 4);
	EXPECT_EQ(chem2.listeVilles[0], "Quebec");
	EXPECT_EQ(chem2.listeVilles[1], "Montreal");
	EXPECT_EQ(chem2.listeVilles[2], "Toronto");
	EXPECT_EQ(chem2.listeVilles[3], "Bogota");

	Chemin chem3 = ra.rechercheCheminBellManFord("Quebec", "Iqaluit", 1);
	EXPECT_EQ(chem3.reussi, true);
	EXPECT_EQ(chem3.dureeTotale, 4.67f);
	EXPECT_EQ(chem3.listeVilles.size(), 4);

	Chemin chem4 = ra.rechercheCheminBellManFord("Quebec", "Iqaluit", 2);
	EXPECT_EQ(chem4.reussi, true);
	//EXPECT_EQ(chem4.coutTotal, 1140.66);ok
	EXPECT_EQ(chem4.listeVilles.size(), 4);

	Chemin chem5 = ra.rechercheCheminBellManFord("Iqaluit", "Quebec", 2);
	EXPECT_EQ(chem5.reussi, false);

	Chemin chem6 = ra.rechercheCheminBellManFord("Quebec", "Vancouver", 1);
	EXPECT_EQ(chem6.reussi, true);
	EXPECT_EQ(chem6.listeVilles[1], "Montreal");
	EXPECT_EQ(chem6.dureeTotale, 3.55f);

	Chemin chem7 = ra.rechercheCheminBellManFord("Quebec", "Bogota", 3);
	EXPECT_EQ(chem7.reussi, true);
	EXPECT_EQ(chem7.nsTotal, 0);
	EXPECT_EQ(chem7.listeVilles.size(), 5);
	EXPECT_EQ(chem7.listeVilles[0], "Quebec");
	EXPECT_EQ(chem7.listeVilles[1], "Chicago");
	EXPECT_EQ(chem7.listeVilles[2], "Vancouver");
	EXPECT_EQ(chem7.listeVilles[3], "Toronto");
	EXPECT_EQ(chem7.listeVilles[4], "Bogota");


	ra.getUnReseau().ajouterArc("La_Havane", "Bogota", 5.32f, 1200.36f, 8);
	ra.getUnReseau().ajouterArc("Bogota", "La_Havane", 5.32f, 1200.36f, -10);

	Chemin chem8 = ra.rechercheCheminBellManFord("Quebec", "Bogota", 3);
	EXPECT_EQ(chem8.reussi, false);

	Chemin chem9 = ra.rechercheCheminBellManFord("Detroit", "Quebec", 3);
	EXPECT_EQ(chem9.reussi, false);
}

TEST_F(RATest, Throws){

	EXPECT_THROW(ra.getUnReseau().ajouterSommet("Quebec", 2.2, 2.1), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().ajouterArc("Quebec","IlesMoucMouc", 2.2, 2.1, 1), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().ajouterArc("IlesMoucMouc","Quebec", 2.2, 2.1, 1), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().ajouterArc("IlesMoucMouc","Quebec", 2.2, 2.1, 1), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().enleverSommet("IlesMoucMouc"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().enleverArc("IlesMoucMouc","Quebec"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().enleverArc("Quebec","IlesMoucMouc"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().enleverArc("Moscou","IlesMoucMouc"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().enleverArc("Quebec","Detroit"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().arcExiste("Quebec","IlesMoucMouc"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().arcExiste("IlesMoucMouc","Quebec"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().arcExiste("IlesMoucMouc","Moscou"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().listerSommetsAdjacents("Moscou"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().getPonderationsArc("Quebec","IlesMoucMouc"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().getPonderationsArc("IlesMoucMouc","Quebec"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().getPonderationsArc("IlesMoucMouc","Moscou"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().getPonderationsArc("Quebec","Detroit"), std::logic_error);
	EXPECT_THROW(ra.getUnReseau().getCoordonnesSommet("IlesMoucMouc"), std::logic_error);
	EXPECT_THROW(ra.rechercheCheminBellManFord("Quebec", "IlesMoucMouc", 3), std::logic_error);
	EXPECT_THROW(ra.rechercheCheminBellManFord("IlesMoucMouc", "Quebec", 3), std::logic_error);
	EXPECT_THROW(ra.rechercheCheminBellManFord("IlesMoucMouc", "Moscou", 3), std::logic_error);
	EXPECT_THROW(ra.rechercheCheminDijkstra("Quebec", "IlesMoucMouc", true), std::logic_error);
	EXPECT_THROW(ra.rechercheCheminDijkstra("IlesMoucMouc", "Quebec", true), std::logic_error);
	EXPECT_THROW(ra.rechercheCheminDijkstra("IlesMoucMouc", "Moscou", true), std::logic_error);
}




