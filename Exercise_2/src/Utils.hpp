#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace MeshLibrary{
//le funz di importCell leggono file diversi e importMesh mette tutte le info insieme
//sono funzioni di importazione della mesh

//l'obiettivo è quello di memorizzare e testare le proprietà importate di una mesh poligonale

bool importCell0D(const string& fileName, PolygonalMesh& mesh);

bool importCell1D(const string& fileName, PolygonalMesh& mesh);

bool importCell2D(const string& fileName, PolygonalMesh& mesh);

bool importMesh(const string& path, PolygonalMesh& mesh);

bool checkNonDegenerateEdges(const PolygonalMesh& mesh, vector<double>& edgeLengths);

bool checkNonDegenerateTriangles(const PolygonalMesh& mesh, vector<double>& triangleAreas);

}
