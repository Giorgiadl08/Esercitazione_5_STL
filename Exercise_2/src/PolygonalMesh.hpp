#pragma once

#include <vector>
#include <iostream>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;


namespace MeshLibrary{

struct PolygonalMesh
{
    //Cell0D
    unsigned int NumberOfCell0Ds = 0;

    vector<unsigned int> IdCell0Ds = {};
    vector<Vector2d> CoordinatesCell0Ds;
    vector<unsigned int> MarkerCell0Ds = {};


    //Cell1D
    unsigned int NumberOfCell1Ds = 0;

    vector<unsigned int> IdCell1Ds = {};
    vector<Vector2i> VerticesCell1Ds = {};
    vector<unsigned int> MarkerCell1Ds = {};


    //Cell2D
    unsigned int NumberOfCell2Ds = 0;

    vector<unsigned int> IdCell2Ds = {};
    //sicome non conosco a priori il num di lati e vertici delle celle 2D utilizzo vettori di vettori anziché array statici, in modo da poter aggiungere elementi a questi vettori dinamicamente
    vector<vector<unsigned int>> VerticesCell2Ds;
    vector<vector<unsigned int>> EdgesCell2Ds;

    //uso una mappa per rintracciare, tramite il marker, la lista di vertici e lati che lo hanno caratterizzato
    map<unsigned int, list<unsigned int>> VerticesMarker;
    map<unsigned int, list<unsigned int>> EdgesMarker;


};

}
