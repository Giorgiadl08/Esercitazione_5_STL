#include "Utils.hpp"
#include "PolygonalMesh.hpp"
#include <iostream>

using namespace std;
using namespace MeshLibrary;

int main()
{

    PolygonalMesh mesh; //oggetto che vogliamo in output
    string path = "./PolygonalMesh";

    if(!importMesh(path, mesh))//verifico se l'import della mesh è andato a buon fine
        return 1;

    cout << "Numero di celle 0D: " << mesh.NumberOfCell0Ds << endl;
    cout << "Numero di celle 1D: " << mesh.NumberOfCell1Ds << endl;
    cout << "Numero di celle 2D: " << mesh.NumberOfCell2Ds << endl;
    cout << "\n" << endl;

    vector<double> edgeLengths;
    if (checkNonDegenerateEdges(mesh, edgeLengths)) {
        cout << "Tutti i lati sono non degeneri" << endl;
        cout << "Lunghezze dei lati:" << endl;
        for (size_t i = 0; i < edgeLengths.size(); ++i) {
            cout << "Lato " << i << ": " << edgeLengths[i] << endl;
        }
    } else {
        cout << "Almeno un lato è degenere" << endl;
    }
    cout << "\n" << endl;


    vector<double> triangleAreas;
    if (checkNonDegenerateTriangles(mesh, triangleAreas)) {
        cout << "Tutti i triangoli sono non degeneri" << endl;
        cout << "Aree dei triangoli:" << endl;
        for (size_t i = 0; i < triangleAreas.size(); ++i) {
            cout << "Triangolo " << i << ": " << triangleAreas[i] << endl;
        }
    } else {
        cout << "Almeno un triangolo è degenero." << endl;
    }


    return 0;

}
