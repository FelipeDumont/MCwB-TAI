/*
 *
 */
#ifndef GREEDY_H_
#define GREEDY_H_
#include <math.h>
#include <vector>
#include "EvoAl.h"

using namespace std;

Individual Greedy(bool grasp, int maxValue);
// Since both problems cause an offset to get their data, we set this ones to be used for everyone
int GetNodeMilkQuantity(int id);
int GetNodeType(int id);

extern int offset;
// File data
extern vector <float> P;
extern vector <float> Q;
extern vector <vector<int>> quIds;
extern vector< vector<float>> qu;
extern vector< vector<float>> c;

// Results, will change and only return an individual
extern vector<vector<int>> solution;
extern vector<int> vehicleType;

// preprocess
extern vector<vector <vector<float>>> rC;
extern vector<float> milkGainPerLiter;

#endif /* GREEDY_H_ */
