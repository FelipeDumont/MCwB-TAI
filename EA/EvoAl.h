/*
 *
 */
#ifndef GENAL_H_
#define GENAL_H_
#include <math.h>
#include <vector>

using namespace std;

// GA data
class Individual{
    public:
        vector<vector<int>> routes;// routes
        vector<int> milkPerRoute;// Milk per route
        vector<int> milkTypePerRoute;// Milk type
        vector<float> cost;// Travel per route

        // Elements of search per individual
        vector<vector<int>> aviableFarms; // Aviability per Rank, per Farm
        vector <float> indP; // Requirement per M
        vector <float> indQ; // capacity per V
        float fev;
        // Helpers
        bool ready = false;
        void Initialize(int routesQ);

        // Functions
        float GetGain();
        void UpdateData();
        void removeAviableNode(int nodeID);
        void PrintSolution(bool extra, bool PQextra, bool infoOnly);
        void PrintSolutionInfo();
        void PrintSolutionFromRoutes();

        // Helper functions
        float GetCost(bool sideLeft, int routeid, int nodeid, int selectedValue);
        float moveCostFromNode(int routeid, int nodeid1, int nodeid2, int selectedValue1, int selectedValue2);
        float CostFromRemoveNode(int routeid, int nodeid);
        float CostFromAddNode(int routeid, int nodeid, int selectedNode);
        float CostSidesATNode(int routeid, int nodePosition, int nodeid);

        // Update current solution
        void UpdateRouteCost(int routeid, float add);
        void UpdateRouteMilk(int routeid, float add);
        void UpdateRouteType(int routeid);
        void UpdateQ(vector<float> newQ);

        // F ev
        float Fev(vector<int> totalMilkCollectedFB, float cost, float overload);
        float FevChangeVehicleQ(vector<float> newQ);
        float FevChangeCostNMilk(float costChange, int routeid, int milkChange);
        float FevChangeCostNMilkAndType(float costChange, int routeid, int milkChange, int newType);
        void UpdateFev();

};


// Individual
extern vector<Individual> bestInd;
extern vector<Individual> gen;

// GA
void GenAl(int maxGeneration, int genSize, int elitismQ, int greedyRandomList, int kmoves, float pIS, float pES, float pAdd, float pR);

void FindBest(int elitismQ);
void ReinsertBest(int elitismQ);
void InsertNew();
Individual swapVehicleQ(bool blind, Individual ind, float chance);
Individual AddNode(bool blind, Individual ind, float chancePerRoute);
Individual RemoveNode(bool blind, Individual ind, float chancePerRoute);
Individual SwapNode(bool blind, Individual ind, float chancePerRoute);
Individual SwapNodeOutside(bool blind,Individual ind, float chancePerRoute);


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

// Results
extern vector<vector<int>> solution;
extern vector<int> vehicleType;

// preprocess
extern vector<vector <vector<float>>> rC;
extern vector<float> milkGainPerLiter;
extern bool dV;

// Sint
extern bool sint;
extern int bestKnownValue;

#endif /* GREEDY_H_ */