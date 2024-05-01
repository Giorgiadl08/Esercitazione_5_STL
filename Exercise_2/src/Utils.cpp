#include "Utils.hpp"
#include "PolygonalMesh.hpp"
#include <iostream>
#include <fstream>
#include <cmath>


using namespace std;

namespace MeshLibrary{

//creo una funzione che dato il nome di un file restituisce una mesh poligonale
bool importMesh(const string& path, PolygonalMesh& mesh)
{
    if(!importCell0D(path + "./Cell0Ds.csv", mesh))
        return false;
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto& el : mesh.VerticesMarker)
        {
            cout << "key: " << el.first << endl;
            for(auto& id: el.second)
                cout << id << " ";
            cout << endl;
        }
        cout << "\n" << endl;
    }

    if(!importCell1D(path + "./Cell1Ds.csv", mesh))
        return false;
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto& el : mesh.EdgesMarker)
        {
            cout << "key: " << el.first << endl;
            for(auto& id: el.second)
                cout << id << " ";
            cout << endl;
        }
        cout << "\n" << endl;

    }

    if(!importCell2D(path + "./Cell2Ds.csv", mesh))
        return false;

    return true;
}


bool importCell0D(const string& fileName, PolygonalMesh& mesh)
{
    ifstream file(fileName);//apro il file
    if(file.fail())
        return false;

    string header; //var temporanea
    getline(file, header);//leggo la prima riga e la memorizzo in 'header'

    string line;
    while (getline(file, line))//finchè non ho raggiunto la fine del file, leggo la riga del file e la memorizzo in line
    {
        istringstream convert(line);
        string id_str, marker_str, x_str, y_str;//var temporanee

        //ora leggo l'id, il marker e le coordinate come stringhe, considerando il delimitatore ';'
        getline(convert, id_str, ';');
        getline(convert, marker_str, ';');
        getline(convert, x_str, ';');
        getline(convert, y_str, ';');

        //ora converto le stringhe in int e double
        unsigned int id, marker;//var temp
        Vector2d coordinates;//var temp

        //creo oggetti istringstream e utilizzo l'operatore di estrazione (>>) per estrarre l'id, il marker e le coordinate
        istringstream convert1(id_str);
        convert1 >> id;
        istringstream convert2(marker_str);
        convert2 >> marker;
        istringstream convert3(x_str);
        convert3 >> coordinates(0);
        istringstream convert4(y_str);
        convert4 >> coordinates(1);

        //oppure uso le funzioni stoi e stod
//        unsigned int id = stoi(id_str);
//        unsigned int marker = stoi(marker_str);
//        double x = stod(x_str);
//        double y = stod(y_str);


        mesh.IdCell0Ds.push_back(id);
        mesh.MarkerCell0Ds.push_back(marker);
        mesh.CoordinatesCell0Ds.push_back(coordinates);
        mesh.NumberOfCell0Ds++;


        //eseguo un test sui marker delle celle 0D
        if (marker != 0)
        {
            auto ret = mesh.VerticesMarker.insert({marker, {id}});
            if(!ret.second) //se il ret ha valore false significa che il marker già esisteva
                mesh.VerticesMarker[marker].push_back(id);
        }

    }

    file.close();//chiudo il file
    return true;

}


bool importCell1D(const string& fileName, PolygonalMesh& mesh)
{
    ifstream file(fileName);
    if(file.fail())
        return false;

    string header;
    getline(file, header);

    string line;
    while (getline(file, line))
    {
        istringstream convert(line);
        string id_str, marker_str, origin_str, end_str;

        getline(convert, id_str, ';');
        getline(convert, marker_str, ';');
        getline(convert, origin_str, ';');
        getline(convert, end_str, ';');

        unsigned int id, marker;
        Vector2i vertices;

        istringstream convert1(id_str);
        convert1 >> id;
        istringstream convert2(marker_str);
        convert2 >> marker;
        istringstream convert3(origin_str);
        convert3 >> vertices(0);
        istringstream convert4(end_str);
        convert4 >> vertices(1);

        mesh.IdCell1Ds.push_back(id);
        mesh.MarkerCell1Ds.push_back(marker);
        mesh.VerticesCell1Ds.push_back(vertices);
        mesh.NumberOfCell1Ds++;

        //eseguo un test sui marker delle celle 1D
        if (marker != 0)
        {
            auto ret = mesh.EdgesMarker.insert({marker, {id}});
            if(!ret.second)
                mesh.EdgesMarker[marker].push_back(id);
        }

    }

    file.close();
    return true;

}


//eseguo un test su ogni lato, cioè verifico che non ci siano lati degeneri e calcolo la lunghezza di ogni cella0D
bool checkNonDegenerateEdges(const PolygonalMesh& mesh, vector<double>& edgeLengths)
{
    edgeLengths.clear();//mi assicuro che il vettore sia vuoto
    for (const auto& vertices : mesh.VerticesCell1Ds)
    {
        unsigned int originId = vertices(0);//id dell'origine
        unsigned int endId = vertices(1);//id della fine

        //trovo le coordinate dei punti di origine e fine
        Vector2d originCoord = mesh.CoordinatesCell0Ds[originId];
        Vector2d endCoord = mesh.CoordinatesCell0Ds[endId];

        //calcolo la lunghezza tra i due punti
        double length = (endCoord - originCoord).norm();

        edgeLengths.push_back(length); //aggiungo la lunghezza al vettore delle lunghezze dei lati

        double tolDefault = 10 * numeric_limits<double>::epsilon(); //tolleranza di default

        //verifico che la lunghezza sia non nulla
        if (length < tolDefault){
            return false;
        }

    }
    return true;//se tutti i lati sono non degeneri, restituisci true
}


bool importCell2D(const string& fileName, PolygonalMesh& mesh)
{
    ifstream file(fileName);
    if(file.fail())
        return false;

    string header;
    getline(file, header);

    string line;//inizio a processare stando attenti al ';' e al fatto che numVertices e NumEdges variano
    while (getline(file, line))
    {
        istringstream convert(line);
        char delimiter;
        unsigned int id, marker, numVertices, numEdges;
        convert >> id >> delimiter >> marker >> delimiter >> numVertices >> delimiter;

        vector<unsigned int> vertices;
        for (unsigned int i = 0; i < numVertices; ++i) {
            unsigned int vertex;
            convert >> vertex >> delimiter;
            vertices.push_back(vertex);
        }

        convert >> numEdges >> delimiter;

        vector<unsigned int> edges;
        for (unsigned int i = 0; i < numEdges; ++i) {
            unsigned int edge;
            convert >> edge >> delimiter;
            edges.push_back(edge);
        }

        mesh.IdCell2Ds.push_back(id);
        mesh.VerticesCell2Ds.push_back(vertices);
        mesh.EdgesCell2Ds.push_back(edges);
        mesh.NumberOfCell2Ds++;

    }

    return true;

}


//eseguo un test su ogni triangolo della mesh poligonale, cioè verifico che non ci siano aree degenere e calcolo l'area delle celle2D che formano un triangolo
bool checkNonDegenerateTriangles(const PolygonalMesh& mesh, vector<double>& triangleAreas)
{
    triangleAreas.clear();//mi assicuro che il vettore sia vuoto

    //funzione per calcolare l'area di un triangolo dati i suoi vertici
    auto computeTriangleArea = [](const Vector2d& v1, const Vector2d& v2, const Vector2d& v3) {
        double a = (v2 - v1).norm();
        double b = (v3 - v2).norm();
        double c = (v1 - v3).norm();
        double s = (a + b + c) / 2; //semiperimetro
        return sqrt(s * (s - a) * (s - b) * (s - c)); //formula di Erone per l'area
    };

    //itero attraverso ogni cella 2D
    for (unsigned int i = 0; i < mesh.NumberOfCell2Ds; ++i)
    {
        //se la cella ha esattamente 3 lati, è un triangolo
        if (mesh.VerticesCell2Ds[i].size() == 3)
        {
            //estraggo i vertici del triangolo dalla mesh
            Vector2d v1 = mesh.CoordinatesCell0Ds[mesh.VerticesCell2Ds[i][0]];
            Vector2d v2 = mesh.CoordinatesCell0Ds[mesh.VerticesCell2Ds[i][1]];
            Vector2d v3 = mesh.CoordinatesCell0Ds[mesh.VerticesCell2Ds[i][2]];

            //calcolo l'area del triangolo
            double area = computeTriangleArea(v1, v2, v3);

            triangleAreas.push_back(area);

            double tolDefault = 10 * numeric_limits<double>::epsilon();

            //verifico che l'area sia non nulla
            if (area < tolDefault) {
                return false;
            }

        }
    }
    return true;
}

}
