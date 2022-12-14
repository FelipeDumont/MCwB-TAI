#include "Initialize.h"
#include "Greedy.h"


using namespace std;

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

int GetNodeType(int id){
    // cout << id << quIds.size() << endl;
    vector<int> nodeData = quIds[id - offset]; // type and position
    
    return nodeData[0];
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

int FindBestNodeIDGrasp(vector<int> selectableNodes, int currentNode, int currentQuotaID, int maxValue, int maxQuota){
    float min = -std::numeric_limits<float>::infinity();
    vector<int> organizedIds;
    int selectedNode = -1;
    for(long unsigned int i = 0; i< selectableNodes.size(); i++){
        int nodeMQ = GetNodeMilkQuantity(selectableNodes[i]);

        int currentRC = rC[currentQuotaID][currentNode][selectableNodes[i]];
        if(nodeMQ <= maxQuota){
            if( currentRC > min || (int)organizedIds.size() < maxValue){
                bool found = false;

                for (long unsigned int j = 0; j<  organizedIds.size() && (int)j < maxValue; j++){
                    if (currentRC > rC[currentQuotaID][currentNode][selectableNodes[organizedIds[j]]]){
                        found = true;
                        organizedIds.push_back(i);
                        break;
                    }
                }
                if(!found){
                    organizedIds.emplace_back(i);
                }
                int minID = (int)organizedIds.size();
                minID = minID > maxValue ? maxValue : minID;
                min = rC[currentQuotaID][currentNode][selectableNodes[organizedIds[minID-1]]];

            }
        }
    }
    if(organizedIds.size() > 0){
        int minID = (int)organizedIds.size();
        selectedNode =  organizedIds[int_rand(0,minID)];

    }
    
    return selectedNode;
}


// selectableNodes, currentNode, vSize - currentMilkQuantity , currentQuotaID
// return node id or -1
int FindBestNodeID(vector<int> selectableNodes, int currentNode, int currentQuotaID, int maxQuota){
    float min = -std::numeric_limits<float>::infinity();
    int selectedNode = -1;
    for(long unsigned int i = 0; i< selectableNodes.size(); i++){
        int nodeMQ = GetNodeMilkQuantity(selectableNodes[i]);

        if(nodeMQ <= maxQuota){
            if(rC[currentQuotaID][currentNode][selectableNodes[i]] > min){
                selectedNode =  i;//selectableNodes[i];
                min = rC[currentQuotaID][currentNode][selectableNodes[i]];
            }
        }
    }
    return selectedNode;
}


// TODO: Update to use individual, it's better to use this since it's gonna be used for the GenAlg
Individual Greedy(bool grasp, int maxValue){
    Individual ind;
    ind.Initialize((int)Q.size());

    int carCounter = 0;
    vector<bool> endingFlags;
    vector<int> currentFilledQuota;
    vector<int> sol;
    
    
    // ALL QUOTAS NOT FILLED
    for(long unsigned int i = 0; i< 3; i++){
        endingFlags.push_back(false);
        currentFilledQuota.push_back(0);
    }

    vector<int> selectableNodes;
    for(long unsigned int j = 0; j< qu[0].size(); j+=2){
        selectableNodes.push_back(qu[0][j]);
    }

    int currentQuotaID = GetCurrentQuota(endingFlags);
    

    // Try fill all the quotas while mantaining the car max quantity
    while(currentQuotaID != -1){

        // Check car size
        int carQuantity = (int)ind.indQ.size();
        // Current vehicle size
        int vSize = (int)ind.indQ[carCounter];
        bool carFullorFinishedQuota = false;

        int currentNode = 0;
        int currentMilkQuantity = 0;
        ind.milkPerRoute.push_back(0);
        vehicleType.push_back(currentQuotaID);
        ind.milkTypePerRoute[carCounter] = currentQuotaID;
        while(carFullorFinishedQuota == false){
            int maxQuota = vSize - currentMilkQuantity;
            int selectedNodeID = -1;
            if(grasp){
                selectedNodeID = FindBestNodeIDGrasp(selectableNodes, currentNode , currentQuotaID, maxValue, maxQuota);
            }
            else{
                selectedNodeID = FindBestNodeID(selectableNodes, currentNode , currentQuotaID, maxQuota);
            }
            if (selectedNodeID >= 0){
                int bestNode = selectableNodes[selectedNodeID];
                int nodeMilkQuantity = (int)GetNodeMilkQuantity(bestNode);
                selectableNodes.erase(selectableNodes.begin() + selectedNodeID);
                ind.removeAviableNode(selectedNodeID);
                sol.push_back(bestNode);
                ind.routes[carCounter].push_back(bestNode);
                currentMilkQuantity += nodeMilkQuantity;
                ind.milkPerRoute[carCounter] += nodeMilkQuantity;
                currentFilledQuota[currentQuotaID] += nodeMilkQuantity;
                currentNode = bestNode;
            }
            // No more size || nore aviable nodes || no more options
            if (vSize - currentMilkQuantity <= 0 || selectedNodeID == -1 || selectableNodes.size() == 0){
                // Car is full !!!
                carFullorFinishedQuota = true;
            }
        }
        
        // Factory blending
        int currentFilling = 0;
        while(currentFilling >= 0 && currentQuotaID != -1){
            currentFilling = 0;
            for (int i = 0; i < currentQuotaID + 1; ++i)
            {
                currentFilling += currentFilledQuota[i] - P[i];
            }

            // cout << currentFilling << endl;
            // cout << currentFilledQuota[0] +  currentFilledQuota[1]  << "-"<< P[0]<< "/" << P[currentQuotaID] << endl;

            if(currentFilling >= 0){
                // cout << "______"<< endl;
                endingFlags[currentQuotaID] = true;
                int lastQuotaID = currentQuotaID;
                currentQuotaID = GetCurrentQuota(endingFlags);
                // Check current quota, just add when it changes !
                if(currentQuotaID != -1 && lastQuotaID != currentQuotaID){
                    for(long unsigned int j = 0; j< qu[currentQuotaID].size(); j+=2){
                        selectableNodes.push_back(qu[currentQuotaID][j]);
                    }
                }
            }
        }
        currentQuotaID = GetCurrentQuota(endingFlags);

        // Add to the solution
        solution.push_back(sol);
        vector<int> aux;
        sol = aux;
        // Next car
        carCounter += 1;
        if (carQuantity <= carCounter)
            break;
    }

    ind.UpdateData();
    return ind;
}
