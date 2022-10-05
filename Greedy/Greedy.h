/*
 *
 */
#ifndef GREEDY_H_
#define GREEDY_H_
#include <math.h>
#include <vector>

using namespace std;

void Greedy();
void PrintSolution();

extern bool useOverFill;

// Values Main
extern char* mode;
extern char* InstanceFile;
extern unsigned int Seed;
extern int offset;
// File data
extern int firstNode;
extern float aValue, bValue, cValue;
extern vector <float> P;
extern vector <float> Q;
extern vector <int> QName;
extern vector <vector<int>> quIds;
extern vector< vector<float>> qu;
extern vector< vector<float>> c;

extern vector<vector<int>> solution;
extern vector<int> vehicleType;

// preprocess
extern vector<vector <vector<float>>> rC;

#endif /* GREEDY_H_ */