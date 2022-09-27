// Example call 
// ./Greedy new ../Instancias Other/5clusters.dat.1 548792
// ./Greedy old ../Instancias/eil22.txt 548792

//         FileName        Seed
#include "Initialize.h"
#include "Greedy.h"
#include <iostream>
#include <cstring>
using namespace std;

char* InstanceFile;
char* mode;
unsigned int Seed;
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

vector<vector<int>> solution; // Solution

int main(int argc, char** argv) {
  Capture_Params(argc,argv);
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
  srand48(Seed);
  // PrintInstanceData();
  // cout << "A" << endl;
  Greedy();
  // cout << "B" << endl;
  PrintSolution();
  return 0;
}
