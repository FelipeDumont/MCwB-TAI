/*
 * Initialize.h
 *
 */

#ifndef INITIALIZE_H_
#define INITIALIZE_H_
using namespace std;
#include <fstream>
#include <iostream>
#include <vector>

// Functions
void Capture_Params(int argc, char** argv);
bool Read_Instance();
bool Read_Instance_old();
void PrintInstanceData();

int int_rand (int a, int b);
float float_rand (float a, float b);

// Values Main
extern char* mode;
extern char* InstanceFile;
extern unsigned int Seed;
// File data
extern int firstNode;
extern float aValue, bValue, cValue;
extern vector <float> P;
extern vector <float> Q;
extern vector <int> QName;
extern vector <vector<int>> quIds;
extern vector< vector<float>> qu;
extern vector< vector<float>> c;

// Vector that holds the current position vectors
extern vector<vector<int>> positions;

// Result
#endif /* INITIALIZE_H_ */
