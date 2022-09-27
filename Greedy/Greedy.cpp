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

int GetNodeQuantity(int id){
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

void Greedy(){
    // cout << "Greedy" << qu[0].size() << endl;
    vector<int> selectableNodes;
    for(long unsigned int i = 0; i< qu[0].size(); i+=2){
        selectableNodes.push_back(qu[0][i]);
        // cout << "Add selectable node: "<< qu[0][i] << endl;
    }
    int carQuantity = (int)Q.size();
    int carCounter = 0;
    int typeCount = 0;
    int totalAddedMilk = 0;
    vector<bool> endingFlags;
    vector<int> sol;
    
    for(long unsigned int i = 0; i< 3; i++){
        endingFlags.push_back(false);
    }
    
    while (carCounter < carQuantity) { // All  vehicules used!
        // size of Vehicule 0,1,2 ...
        // cout << "fill car " << carCounter << endl;
        int vSize = (int)Q[carCounter];
        // cout << "vSize:"<< vSize << endl;
        bool flagCanSelect = true; // if i cant add more
        int currentlyAddedMilkQuantity = 0;
        int currentCarElementCounter = 0;
        while(flagCanSelect && selectableNodes.size() > 0){
            // cout << "filling car at id " << currentCarElementCounter << endl;
            int bestNode = 0;
            int nodeToRemove = 0;
            int bestGain = 0;
            int milkQuantity = 0;
            bool selectedOne = false;
            for(long unsigned int i = 0; i< selectableNodes.size(); i++){
                // Select node, and continue
                int travelCost = 0;
                // cout << i << " sn:" << selectableNodes[i] << endl;
                int currentMilkQuantity = (int)GetNodeQuantity(selectableNodes[i]);
                // cout << "can fit " << currentlyAddedMilkQuantity + currentMilkQuantity <<"/" << vSize << endl;
                if(currentlyAddedMilkQuantity + currentMilkQuantity > vSize){ // Check the ones that fit
                    currentMilkQuantity = 0;
                    continue;
                }
                else{
                    selectedOne = true;
                }
                if(currentCarElementCounter == 0){
                    travelCost = (int)c[0][selectableNodes[i]];
                }
                else{
                    travelCost = (int)c[selectableNodes[i-1]][selectableNodes[i]];
                }
                int currentGain = currentMilkQuantity - travelCost;
                if(currentGain > bestGain){
                    milkQuantity = currentMilkQuantity;
                    nodeToRemove = i;
                    bestNode = selectableNodes[i];
                    bestGain = currentGain;
                }
            }
            if(selectedOne && bestNode != 0){
                currentlyAddedMilkQuantity += milkQuantity;
                // cout << "current Milk " << currentlyAddedMilkQuantity << " erase at " << nodeToRemove << "  " << bestNode << endl;
                selectableNodes.erase(selectableNodes.begin() + nodeToRemove);
                sol.push_back(bestNode);
            }
            if(bestNode == 0){
                selectedOne = false;
            }
            flagCanSelect = selectedOne;
            currentCarElementCounter++;
        }
        totalAddedMilk += currentlyAddedMilkQuantity;
        // Check the current milk production with the Vehicule ready
        if(typeCount == 0 && endingFlags[0] == false){
            // cout << currentlyAddedMilkQuantity << endl;
            if(totalAddedMilk >= P[0]){
                // cout << totalAddedMilk << "A"<<endl;
                typeCount++;
                endingFlags[0] = true;
                currentlyAddedMilkQuantity = 0;
                totalAddedMilk = 0;
                for(long unsigned int i = 0; i< qu[1].size(); i+=2){
                    selectableNodes.push_back(qu[1][i]);
                    // cout << "Add selectable node B: "<< qu[1][i] << endl;
                }
            }
        }
        if(typeCount == 1 && endingFlags[1] == false){
            if(totalAddedMilk >= P[1]){
                // cout << totalAddedMilk << "B"<<endl;
                typeCount++;
                endingFlags[1] = true;
                currentlyAddedMilkQuantity = 0;
                totalAddedMilk = 0;
                for(long unsigned int i = 0; i< qu[2].size(); i+=2){
                    selectableNodes.push_back(qu[2][i]);
                    // cout << "Add selectable node C: "<< qu[2][i] << endl;
                }
            }
        }
        if(typeCount == 2 && endingFlags[2] == false){
            if(totalAddedMilk >= P[2]){
                // cout << totalAddedMilk << "C"<<endl;
                typeCount++;
                endingFlags[2] = true;
                currentlyAddedMilkQuantity = 0;
                totalAddedMilk = 0;
            }
        }
        solution.push_back(sol);
        vector<int> aux;
        sol = aux;
        carCounter++;
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
    for(long unsigned int i = 0; i < solution.size(); i++){
        cout << solution[i][0];
        for(long unsigned int j = 1; j < solution[i].size(); j++){
            cout << "-"<< solution[i][j];
        }
        cout << endl;
    }
    
}
