// Example call  new or old, instance location seed and use overfill for the greedy
// ./Greedy new ../InstanciasO/5clusters.dat.1 548792 1
// ./Greedy old ../Instancias/eil22.txt 548792 1

//         FileName        Seed
#include "Initialize.h"
#include "Greedy.h"
#include <iostream>
#include <cstring>
using namespace std;

char* InstanceFile;
char* mode;
unsigned int Seed;
bool useOverFill;
int offset = 0;
// Obtained values
int firstNode;
float aValue, bValue, cValue; // Price multiplier per milk quality !
vector <float> P; // production needed
vector <float> Q; // Quantity of the load
vector <int> QName;   // Vehicules ids
vector <vector<int>> quIds;   // ids to obtain the information in the qu from the node id
vector< vector<float>> qu; // production type -> quantity, id 
vector< vector<float>> c; // node[0] to [0 , 1, 2 -> last] -> cost from self x to node y 
vector<vector<int>> positions; // OLD instances positions
// Pre-process
vector<vector <vector<float>>> rC; // cost per node in each milk type example  delta / N(milk) 
vector<vector<int>> solution; // Solution
vector<int> vehicleType;

int main(int argc, char** argv) {

  Capture_Params(argc,argv);
  // cout << mode << endl;
  // cout << InstanceFile << endl;
  if(strcmp(mode,"old") != 0){
    offset = 1;
    aValue = 0.03;
    bValue = 0.021;
    cValue = 0.009;
    Read_Instance();
  }
  else{
    offset = 0;
    Read_Instance_old();
  }

  // PrintInstanceData();
  // cout << "A" << endl;
  Preprocess();
  srand48(Seed);
  
  // cout << "B" << endl;
  Greedy();
  // cout << "C" << endl;
  PrintSolution();
  return 0;
}
