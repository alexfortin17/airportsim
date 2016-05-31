/*
 * testperso.cpp
 *
 *  Created on: Jul 7, 2015
 *      Author: alex
 */
#include "Graphe.h"
#include "gtest/gtest.h"
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace TP3;

int main(){

	Graphe ge = Graphe();
	float a = 8.8;
	float b = 9.3;
	ge.ajouterSommet("mtl", a, b);
	cout << "vomi" << endl;
	cout << ge.nombreSommets() << endl;

	return 0;
}

