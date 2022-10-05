#include "Initialize.h"
#include "Greedy.h"

using namespace std;

bool CheckEnding(vector<bool> flags){
    for(long unsigned int i = 0; i< 3; i++){
        if(flags[i] == false){
            return false;
        }
    }
    return true;
}

int GetCurrentSearch(vector<bool> flags){
    for(long unsigned int i = 0; i< 3; i++){
        if(flags[i] == false){
            return (int)i;
        }
    }
    return int_rand(0,3);
}

int GetNodeMilkQuantity(int id){
    // cout << id << quIds.size() << endl;
    vector<int> nodeData = quIds[id - offset]; // type and position
    // cout << id << " " << nodeData[0] << " "<< nodeData[1]<< " "  << qu[nodeData[0]][nodeData[1]] << " Q" << qu[nodeData[0]][nodeData[1]+1]<< endl;
    
    if(nodeData[0] == 0){
        return qu[nodeData[0]][nodeData[1]+1];
    }
    if(nodeData[0] == 1){
        return qu[nodeData[0]][nodeData[1]+1];
    }
    if(nodeData[0] == 2){
        return qu[nodeData[0]][nodeData[1]+1];
    }
    return 0;
}

bool IsProfitable(vector<int> sNodes){
    bool thereIsANode = false;
    for(long unsigned int i = 0; i< sNodes.size(); i++){
        int currentMilkQuantity = (int)GetNodeMilkQuantity(sNodes[i]);
        int travelCost = (int)c[0][sNodes[i]];
        if (currentMilkQuantity - travelCost > 0){
            thereIsANode = true;
            break;
        }
    }
    return thereIsANode;
}

bool AllQuotaDelivered(vector<bool> endingFlags){
    for(long unsigned int i = 0; i< 3; i++){
        if(endingFlags[i] == false)
            return false;
    }
    return true;
}

int GetCurrentQuota(vector<bool> endingFlags){
    for(long unsigned int i = 0; i< 3; i++){
        if(endingFlags[i] == false)
            return i;
    }
    return -1;
}

// selectableNodes, currentNode, vSize - currentMilkQuantity , currentQuotaID
// return node id or -1
int FindBestNodeID(vector<int> selectableNodes, int currentNode, int currentQuotaID){
    int min = -9999999;
    int selectedNode = -1;
    for(long unsigned int i = 0; i< selectableNodes.size(); i++){
        vector<int> nodeData = quIds[selectableNodes[i] - offset];
        // float currentMilk = qu[nodeData[0]][nodeData[1]+1];
        // If its the best of the ones that can be set with the current aviable size
        // cout << "size:" << rC[currentQuotaID].size() << endl;
        // cout << "D: " << rC[currentQuotaID][currentNode][selectableNodes[i]] << " min" << min << " cm" << currentMilk << endl;
        if(rC[currentQuotaID][currentNode][selectableNodes[i]] > min){
            selectedNode =  i;//selectableNodes[i];
            min = rC[currentQuotaID][currentNode][selectableNodes[i]];
        }
    }
    return selectedNode;
}

void Greedy(){
    // ADDING A TYPE NODES
    // cout << "Greedy" << endl;
    vector<int> selectableNodes;
    for(long unsigned int j = 0; j< qu[0].size(); j+=2){
        selectableNodes.push_back(qu[0][j]);
        // cout << "Add selectable node C: "<< qu[0][j] << endl;
    }

    int carCounter = 0;
    vector<bool> endingFlags;
    vector<int> currentFilledQuota;
    vector<int> sol;
    
    // ALL QUOTAS NOT FILLED
    for(long unsigned int i = 0; i< 3; i++){
        endingFlags.push_back(false);
        currentFilledQuota.push_back(0);
    }
    
    // Obtain the solution with just the delivered quota !, -1 ending flag
    int currentQuotaID = GetCurrentQuota(endingFlags);
    while(currentQuotaID != -1){

        // Check car size
        int carQuantity = (int)Q.size();
        // Current vehicle size
        int vSize = (int)Q[carCounter];
        bool carFullorFinishedQuota = false;
        int currentNode = 0;
        int currentMilkQuantity = 0;
        vehicleType.push_back(currentQuotaID);
        while(carFullorFinishedQuota == false){
            // Select Node
            // Remove node from poll
            // Add Node to current Vehicle solution
            // cout << "V" << carCounter << " N:" << currentNode << " A:" << (vSize - currentMilkQuantity) << " Q:" << currentQuotaID << " sn" << selectableNodes.size() << endl;
            int selectedNodeID = FindBestNodeID(selectableNodes, currentNode , currentQuotaID);
            if (selectedNodeID >= 0){
                int bestNode = selectableNodes[selectedNodeID];
                int nodeMilkQuantity = (int)GetNodeMilkQuantity(bestNode);
                selectableNodes.erase(selectableNodes.begin() + selectedNodeID);
                sol.push_back(bestNode);
                currentMilkQuantity += nodeMilkQuantity;
                currentFilledQuota[currentQuotaID] += nodeMilkQuantity;
                currentNode = bestNode;
                // cout << "current quota" << currentFilledQuota[currentQuotaID]  << "|" << bestNode << ":" << nodeMilkQuantity << endl;
            }
            if (vSize - currentMilkQuantity <= 0){
                // Car is full !!!
                carFullorFinishedQuota = true;
            }
            // cout << "A"<< endl;
            // cout << P[currentQuotaID] << "|" << currentFilledQuota[currentQuotaID]  << "(" << currentMilkQuantity<< ")"<< endl;
            // Check if quota is filled !
            if(currentFilledQuota[currentQuotaID] >= P[currentQuotaID] || selectableNodes.size() == 0){
                // Current quota filled !!!
                carFullorFinishedQuota = true;
                endingFlags[currentQuotaID] = true;
                // Check current quota
                currentQuotaID = GetCurrentQuota(endingFlags);
                // Add new node options !
                if(currentQuotaID != -1){
                    for(long unsigned int j = 0; j< qu[currentQuotaID].size(); j+=2){
                        selectableNodes.push_back(qu[currentQuotaID][j]);
                        // cout << "Add selectable node C: "<< qu[currentQuotaID][j] << endl;
                        // cout << "cqi" << currentQuotaID << endl;
                    }
                }
            }
        }
        // cout << "END"<< endl;
        // Add to the solution
        solution.push_back(sol);
        vector<int> aux;
        sol = aux;
        // Next car
        carCounter += 1;
        if (carQuantity <= carCounter)
            break;
    }    
    if(useOverFill){
        // cout << "OVER FILL" << selectableNodes.size() << endl;
        // Once all the quota is filled, we add even more !
        for(long unsigned int i = 0; i < selectableNodes.size(); i++){
            // cout << "check node " << selectableNodes[i] << endl;
            vector<int> nodeData = quIds[selectableNodes[i] -offset];
            int nodeType = nodeData[0];
            int bestDelta = -99999;
            int vehicleSelected = -1;
            for(int j = nodeType; j < 3; j++){
                for (long unsigned int k = 0; k < vehicleType.size(); k++){
                    if(vehicleType[k] == j) {
                        //
                        int lastNode = solution[k].back();
                        if(rC[nodeType][lastNode][selectableNodes[i]] > bestDelta){
                            vehicleSelected =  k;//selectableNodes[i];
                            bestDelta = rC[nodeType][lastNode][selectableNodes[i]];
                        }
                    }
                }
            }
            if(vehicleSelected != -1){
                // cout << i << endl;
                solution[vehicleSelected].push_back(selectableNodes[i]);
            }
        }
    }
}

void PrintSolution(){
    
    int travelCost = 0;
    int milkGain = 0;
    int finalGain = 0;
    int currentMilkA = 0;
    int currentMilkB = 0;
    int currentMilkC = 0;

    // Solutions are just vectors, so we will translate them to an actual solution representation
    for(long unsigned int i = 0; i < solution.size(); i++){
        int finalType = 0;
        int currentMilk = 0;
        for(long unsigned int j = 0; j < solution[i].size(); j++){
            // cout << i << " " << j << endl;
            if(j == 0){
                travelCost += (int)c[0][solution[i][j]];
            }
            else{
                travelCost += (int)c[solution[i][j-1]][solution[i][j]];
            }
            // cout << "V" << solution[i][j] <<endl;
            vector<int> nodeData = quIds[solution[i][j]-offset];
            // cout << i << " " << j << "|" << nodeData[0] << "," << nodeData[1] << " " << qu[nodeData[0]][nodeData[1]+1] << "|" << currentMilk << endl;
            currentMilk += qu[nodeData[0]][nodeData[1]+1];

            if(nodeData[0] == 0 && finalType == 0){
                finalType = 0;
            }
            if(nodeData[0] == 1 && finalType <=1){
                finalType = 1;
            }
            if(nodeData[0] == 2){
                finalType = 2;
            }
            if(j == solution[i].size() -1){
                travelCost += (int)c[solution[i][j]][0];
            }
        }
        if(finalType == 0)
            currentMilkA += currentMilk;
        if(finalType == 1)
            currentMilkB += currentMilk;
        if(finalType == 2)
            currentMilkC += currentMilk;
    }    
    milkGain = (int)(currentMilkA * aValue + currentMilkB *bValue + currentMilkC *cValue);
    finalGain = milkGain - travelCost;
    cout << finalGain << " " << travelCost << " " << milkGain << endl;
    cout << "MA:" << currentMilkA << " MB:" << currentMilkB << " MC:" << currentMilkC << endl;
    for(long unsigned int i = 0; i < solution.size(); i++){
        if (solution[i].size() > 0){
            cout << solution[i][0];
            for(long unsigned int j = 1; j < solution[i].size(); j++){
                cout << "-"<< solution[i][j];
            }
        }
        else{
            cout << "Un feseable";
        }
        cout << endl;
    }
    
}
