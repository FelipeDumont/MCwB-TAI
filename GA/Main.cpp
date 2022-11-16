// Example call  new or old, instance location seed and use overfill for the greedy
// ./GA new ../InstanciasO/5clusters.dat.1 548792 1
// ./GA old ../Instancias/eil22.txt 548792 1

//         FileName        Seed
#include "Initialize.h"
#include "Greedy.h"
#include "GenAl.h"
#include <iostream>
#include <cstring>

#include <bits/stdc++.h>

using namespace std;
using std::ifstream;

char* InstanceFile; // with what file we will work !
char* mode; // mode, old or new
unsigned int Seed; 
bool useOverFill; // Do a overfill in the greedt algorithm
int offset = 0; // offset used for the new cases

// Obtained values
int firstNode;
float aValue, bValue, cValue; // Price multiplier per milk quality !
vector <float> P; // production needed
vector <float> Q; // Quantity, Capacity of the vehicle
vector <int> QName;   // Vehicles ids
vector <vector<int>> quIds;   // ids to obtain the information in the qu from the node id
vector< vector<float>> qu; // production type -> quantity, id 
vector< vector<float>> c; // node[0] to [0 , 1, 2 -> last] -> cost from self x to node y 
vector<vector<int>> positions; // OLD instances positions

// Pre-process
vector<vector <vector<float>>> rC; // cost per node in each milk type example  delta / N(milk)
vector<float> milkGainPerLiter; // Vector of milkA, milkB, milkC gain per liter
bool dV; //Diferent Vehicles

// GA
Individual bestInd;
vector<Individual> gen;

// Results
vector<vector<int>> solution; // Solution, since GA, we will have solution vector !
vector<int> vehicleType;

// Initial parameters
int maxGeneration = 10;
int genSize = 1;
/*
// Intesification
float pS;
float pSO;
float pSE;
// exploration
float aN;
float rN;
*/
void PrintSolution();

int main(int argc, char** argv) {

  Capture_Params(argc,argv);

  // Default values for the big instances, where the milk value is not set
  if(strcmp(mode,"old") != 0){
    offset = 1;
    aValue = 0.03;
    bValue = 0.021;
    cValue = 0.009;
    Read_Instance();
  }
  // Little instances with milk settings ready to go !
  else{
    offset = 0;
    Read_Instance_old();
  }

  // cout << "Start ! Preprocess" << endl;
  // Preapare the data to work !
  Preprocess();
  srand48(Seed);
  
  // PrintInstanceData();
  // Individual ind = Greedy(false, 4);
  // Individual ind = Greedy(false, 4);
  // ind.PrintSolution(true, true, false);

  // ind.aviableFarms.clear();
  if(maxGeneration != 0 && genSize != 0){
    GenAl(maxGeneration, genSize);
  }
  return 0;
}

