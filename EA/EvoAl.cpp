#include "Initialize.h"
#include "EvoAl.h"
#include "Greedy.h"

using namespace std;

// Global value
float penaltyMultiplier = 1;
long unsigned int maxMoves = 6; // Max number of moves to locate the first improvement, in case there are an infinite number of options
float overloadPenalty = 12.0f;
float missingPPenalty = 8.0f;

// default 100.000 | 5 | 1 | 3 | 6 | 0.6 | 0.6 | 0.4 | 0.25
void GenAl(int maxGeneration, int genSize, int elitismQ, int greedyRandomList, int kmoves, float pIS, float pES, float pAdd, float pR){
	float pRemoveNode = pR;
	float pAddNode = pAdd;
	float pISNode = pIS;
	float pESNode = pES;

	maxMoves = kmoves;
	bool testing = false;
	// There is a chance to start with empty routes
	// Only 1 individual is for testing, and we will use the normal greedy
	if(genSize == 1){
		testing = true;
		Individual ind = Greedy(false, 1);
		gen.push_back(ind);
	}
	// Normal resolution, (not testing)
	else{
		for (int i = 0; i < genSize; ++i)
		{
			Individual ind = Greedy(true, greedyRandomList);
			gen.push_back(ind);
		}
	}
	bestInd.push_back(gen[0]);
	FindBest(elitismQ);
	if(testing){
    	bestInd[0].PrintSolution(true,true,false);
    	bestInd[0].PrintSolution(true,true,true);
 	}
 	

	

	for (int i = 0; i < maxGeneration; ++i)
	{

		// cout << "current generation" << i << endl;
		// bestInd.PrintSolution(true,true,true);
		// FindBest(elitismQ);
		// int bestValue = bestInd[0].FevChangeCostNMilk(0,0,0);
		// cout << i << ":  ["<< bestValue << "] " << endl;
		// try get better current solution, so we don't swap de wrong nodes
		for (int j = 0; j < genSize; ++j)
		{	
			// Swap 1 to test the changes
			gen[j] = SwapNode(false, gen[j], pISNode);
		}
		// try getting out the worst current nodes in replace of new ones, with Vehicle restriction
		for (int j = 0; j < genSize; ++j)
		{	
			gen[j] = SwapNodeOutside(false, gen[j], pESNode);
		}
		// Add if there is a space in the current route !!!, se ignore it if full !
		// On the empty routes, [-1] or nodes removed from 
		for (int j = 0; j < genSize; ++j)
		{
			// Add Node
			gen[j] = AddNode(false, gen[j], pAddNode);
		}

		FindBest(elitismQ);
		
		
		// int currentBest = bestInd[0].FevChangeCostNMilk(0,0,0);
		
		/*
		if (stuckFev == currentBest){
			stuckCounter+= 1;
		} 
		else if (stuckFev < currentBest){
			stuckFev = currentBest;
			stuckCounter = 0;

		}
		*/
		/*
		if(bestValue > currentBest ){
			// cout << "\033[0;31m"<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"  << "\033[0m"<< endl;
		}
		*/

		// Explore
		for (int j = 0; j < genSize; ++j)
		{
			gen[j] = RemoveNode(true, gen[j], pRemoveNode);
		}

		ReinsertBest(elitismQ);
		/*
		if(testing){
    		bestInd[0].PrintSolution(true,true,true);
 		}
 		*/
	}
	FindBest(elitismQ);
	
	// cout << "END GAL" << endl;
	// Take the best and replace the current "solution" 
	if(sint){
		bestInd[0].PrintSolutionFromRoutes();
	}
	else{
		bestInd[0].PrintSolution(true, true,false);
		bestInd[0].PrintSolutionFromRoutes();
	}
	// bestInd.PrintSolution(false, true,false);
	
	
}


void FindBest(int elitismQ){

	// cout << "Elitism" << elitismQ << "Generation size ? "<< int(gen.size()) << endl;
	bestInd.clear();
	elitismQ = elitismQ >= int(gen.size())? int(gen.size()) -1 : elitismQ; // maximum
	float bestScore = -1 * std::numeric_limits<float>::infinity(); // minus infinity
	vector<int> tabuIds;

	for (int e = 0; e < elitismQ; ++e)
	{
		Individual found;
		bestScore = -1 * std::numeric_limits<float>::infinity();
		int foundId = -1;

		// search each 
		for (long unsigned int i = 0; i < gen.size(); ++i)
		{
			bool canCheck = true;
			// Not in tabu list ( elisitsm )
			for (long unsigned int j = 0; j < tabuIds.size(); ++j)
			{
				if((int)i == tabuIds[j]){
					canCheck = false;
				}
			}
			float newScore = gen[i].FevChangeCostNMilk(0,0,0);
			// cout << "new vs best" << newScore << "vs" << bestScore << endl;
			if(bestScore < newScore && canCheck){
				bestScore = newScore;
				found = gen[i];
				foundId = i;
			}
		}
		// Saving
		// Save the last elitismQ individuals
		bestInd.push_back(found);
		tabuIds.push_back(foundId);
		// cout << foundId << endl;
	}
}


// FOR NOW WORKS WITH RESULTANT GAIN; NOT FEV
void ReinsertBest(int elitismQ){

	elitismQ = elitismQ >= int(gen.size())? int(gen.size()) -1 : elitismQ;
	for (int e = 0; e < elitismQ; ++e)
	{
		int idToReplace = -1;
		float worstScore = std::numeric_limits<float>::infinity();

		// search eachone
		for (long unsigned int i = 0; i < gen.size(); ++i)
		{
			float newScore = gen[i].FevChangeCostNMilk(0,0,0);
			if(worstScore > newScore){
				worstScore = newScore;
				idToReplace = i;
			}
		}
		Individual replace = bestInd[e];
		gen[idToReplace] = replace;
	}
}

void InsertNew(){

	int idToReplace = -1;
	float worstScore = std::numeric_limits<float>::infinity();

	// search eachone
	for (long unsigned int i = 0; i < gen.size(); ++i)
	{
		float newScore = gen[i].FevChangeCostNMilk(0,0,0);
		if(worstScore > newScore){
			worstScore = newScore;
			idToReplace = i;
		}
	}

	Individual replace = Greedy(true, greedyRandomList);
	gen[idToReplace] = replace;
	
}
//////////////////////////////////////////
//			Genetic AL Functions		//
//////////////////////////////////////////

Individual swapVehicleQ(bool blind, Individual ind, float chance){
	Individual copy = ind;
	// Chance
	float rnd = float_rand(0.00,1.00);
	if(rnd >= chance || !dV){
		return copy;
	}

	// UpdateQ FevChangeVehicleQ
	float oldFev = copy.FevChangeCostNMilk(0,0,0);

	int firstSelected = int_rand(0, copy.indQ.size());
	int secondOffset = int_rand(0, copy.indQ.size());
	float aux = copy.indQ[firstSelected];

	for (long unsigned int j = 0; j < copy.indQ.size() + 1 && j < maxMoves; ++j){
		int secondSelected = (j + secondOffset) % ((int)copy.indQ.size());
		vector<float> indQ = copy.indQ;
		indQ[firstSelected] = indQ[secondSelected];
		indQ[secondSelected] = aux;
		float newFev = ind.FevChangeVehicleQ(indQ);
		// cout  << oldFev << " vs " << newFev << endl;
		if(newFev >= oldFev || blind){
			// cout << "change ???" << endl;
			copy.UpdateQ(indQ);
			break;
		}

	}

	return copy;
}

// Search for the best node in a group
// CHECK FI
Individual AddNode(bool blind, Individual ind, float chancePerRoute){
	Individual copy = ind;
	int initial = int_rand(0, copy.routes.size());
	for (long unsigned int z = 0; z < copy.routes.size(); ++z)
	{
		long unsigned int i = (z + initial) % (int)copy.routes.size();
		// cout << i << " route check " << endl;
		int aviable = 0;
		for (int j = 0; j < 3; ++j)
		{
			aviable += copy.aviableFarms[j].size();
		}
		
		// Chance
		float rnd = float_rand(0.00,1.00);
		if(aviable < 1 || rnd >= chancePerRoute){
			continue;
		}
		// select posible nodes to replace
		int totalNodes = (int)maxMoves > aviable ?  aviable : maxMoves;
		int selectedAviableNode = int_rand(0, aviable-totalNodes);
		// int selectedRoundNode = int_rand(0, (int)copy.routes[i].size());
		vector<int> options;
		vector<int> innedIDs;
		
		// cout << "todo normal" << endl;
		// cicle for options
		int j =  2;
		for (int k = 0, counter = 0; counter < totalNodes &&  k < totalNodes; ++k)
		{
			
			if(selectedAviableNode < (int)copy.aviableFarms[j].size()){
				options.push_back(copy.aviableFarms[j][selectedAviableNode]);
				innedIDs.push_back(selectedAviableNode);
				counter += 1;
				selectedAviableNode += 1;
			}
			if(selectedAviableNode == 0 || (int)copy.aviableFarms[j].size() == 0){
				continue;
			}

			if(selectedAviableNode >= (int)copy.aviableFarms[j].size()){
				selectedAviableNode = selectedAviableNode - (int)copy.aviableFarms[j].size();
				j--;
			}
		}

		// now for each posible solution we need Change in milk and change in cost
		float bestFev = copy.FevChangeCostNMilk(0,0,0);
		int replaceInsertPosition = int_rand(0,copy.routes[i].size()); // insert in the same position
		int foundOption = -1;
		float bestCostDiff = 0;
		float bestMilkDiff = 0;
		
		for (long unsigned int k = 0; k < options.size(); ++k){
			// cout << "see options "<< j << endl;
			// cout << replaceInsertPosition << " options[j] " << options[j] << endl;
			float newCostChange = copy.CostFromAddNode(i, replaceInsertPosition, options[k]);
			float newFev = 0;
			int typeNew = GetNodeType(options[k]);

			newFev = copy.FevChangeCostNMilkAndType(newCostChange,i,GetNodeMilkQuantity(options[k]), typeNew);
			if(newFev >= bestFev || blind){
				
				bestFev = newFev;
				foundOption = k;
				bestCostDiff = newCostChange;
				bestMilkDiff = GetNodeMilkQuantity(options[k]);
				break;
			}
		}

		if(foundOption != -1){
			int typeNew = GetNodeType(options[foundOption]);
			
			// cout << copy.aviableFarms[typeNew][innedIDs[foundOption]] << "==" << options[foundOption]  << endl;
			// cout << copy.milkTypePerRoute[i] << " ? ? == ? ?  " << typeNew << endl;

			// ADD to route erase from aviable
			copy.routes[i].insert(copy.routes[i].begin() + replaceInsertPosition, options[foundOption]);
			copy.aviableFarms[typeNew].erase(copy.aviableFarms[typeNew].begin()+innedIDs[foundOption]);
			
			// Update milk and cost
			copy.UpdateRouteCost(i,bestCostDiff);
			copy.UpdateRouteMilk(i,bestMilkDiff);
			
			// Assign milktype
			copy.UpdateRouteType(i);
			// cout << bestFev << " new ? " << copy.FevChangeCostNMilk(0,0,0) << endl;

		}

	}
	return copy;
}

// For now just add whatever on each
// ALLWAYS TRUE
// CHECK BLIND
Individual RemoveNode(bool blind, Individual ind, float chancePerRoute){
	Individual copy = ind;
	blind = true; // Allways true
	for (long unsigned int i = 0; i < copy.routes.size(); ++i)
	{
		// If there is almost no nodes, dont change that, or the chance
		float rnd = float_rand(0.00,1.00);
		if(copy.routes[i].size() <= 0 || rnd >= chancePerRoute){
			continue;
		}
		// Select Node to start the check
		int selectedNode = int_rand(0, copy.routes[i].size());
		// Get the initial Fev, no changes


		// float oldFev = copy.FevChangeCostNMilk(0,0,0);
		// Try first improvement
		for (long unsigned int j = 0; j < 2; ++j){

			int testNode = (j + selectedNode) % ((int)copy.routes[i].size());
			float newCostChange = copy.CostFromRemoveNode(i, testNode);			
			// float newFev = copy.FevChangeCostNMilk(newCostChange,i,GetNodeMilkQuantity(copy.routes[i][testNode]) * -1);

			// BLIND !!!
			if(blind){
				
				int milkToRemove = GetNodeMilkQuantity(copy.routes[i][testNode]);
				int removedNode = copy.routes[i][testNode];
				// Remove node, change cost and finally 
				copy.routes[i].erase(copy.routes[i].begin() + testNode);
				if(copy.routes[i].size() == 0){
					copy.milkTypePerRoute[i] = -1;
				}
				copy.UpdateRouteCost(i,newCostChange);
				// Update route milk
				copy.UpdateRouteMilk(i,milkToRemove * -1);
				// Finally return the node to the right aviable farm ...
				int type = GetNodeType(removedNode);
				copy.aviableFarms[type].push_back(removedNode);
				copy.UpdateRouteType(i);

				break;
				if(blind)
					break;
			}
		}

	}

	return copy;
}


// CHECK FI
Individual SwapNode(bool blind, Individual ind, float chancePerRoute){

	Individual copy = ind;
	for (long unsigned int i = 0; i < copy.routes.size(); ++i)
	{
		// In case there are no posible swaps or the chance of doing it fails, skip
		float rnd = float_rand(0.00,1.00);
		if(copy.routes[i].size() <= 1 || rnd >= chancePerRoute){
			continue;
		}
		// First route Farm
		int firstPos = int_rand(0, copy.routes[i].size());
		// Second route Farm
		int offset = int_rand(0, copy.routes[i].size());
		if(firstPos == offset){
			offset = (offset + 1) % ((int)copy.routes[i].size());
		}

		int bestPos = -1;
		float bestImprovement = 0;

		int totalNodes = (int)maxMoves > (int)copy.routes[i].size() ?  (int)copy.routes[i].size() : maxMoves;
		// First improvement, stop
		for (long unsigned int j = 0; j < copy.routes[i].size() && (int)j < totalNodes; ++j){

			int secondPos = (j + offset) % ((int)copy.routes[i].size());

			if(firstPos == secondPos){
				continue;
			}

			// if replace, the new cost on this position
			float newCost = copy.moveCostFromNode(i,firstPos, secondPos,copy.routes[i][secondPos], copy.routes[i][firstPos]);
			float wholecurrentCost = copy.moveCostFromNode(i, firstPos,secondPos,copy.routes[i][firstPos], copy.routes[i][secondPos]);
			float currentImprovement = newCost - wholecurrentCost;

			// Best improvement
			if(currentImprovement <= bestImprovement || blind){
				bestImprovement = currentImprovement;
				bestPos = secondPos;

				break;
				if(blind)
					break;
			}
		}

		if(bestPos != -1){
			int aux = copy.routes[i][firstPos];
			copy.routes[i][firstPos] = copy.routes[i][bestPos];
			copy.routes[i][bestPos] = aux;
			copy.UpdateRouteCost(i,bestImprovement);
		}
	}

	return copy;
}


// CHECK
Individual SwapNodeOutside(bool blind, Individual ind, float chancePerRoute){
	Individual copy = ind;
	int initial = int_rand(0, copy.routes.size());
	for (long unsigned int z = 0; z < copy.routes.size(); ++z)
	{
		long unsigned int i = (z + initial) % (int)copy.routes.size();
		// cout << i << " route check " << endl;
		int aviable = 0;
		for (int j = 0; j < 3; ++j)
		{
			// If milk type = 0 -> 0 >= 0 | 0>= 1 | 0>= 2
			if(copy.milkTypePerRoute[i] >= j){
				aviable += copy.aviableFarms[j].size();
			}
		}
		
		// Chance
		float rnd = float_rand(0.00,1.00);
		if(aviable < 1 || rnd >= chancePerRoute){
			continue;
		}
		// select posible nodes to replace
		int totalNodes = (int)maxMoves > aviable ?  aviable : maxMoves;
		int selectedAviableNode = int_rand(0, aviable-totalNodes);
		int selectedRoundNode = int_rand(0, (int)copy.routes[i].size());
		vector<int> options;
		vector<int> innedIDs;
		
		// cicle for options
		int j = copy.milkTypePerRoute[i];
		for (int k = 0, counter = 0; counter < totalNodes &&  k < totalNodes; ++k)
		{
			if(selectedAviableNode < (int)copy.aviableFarms[j].size()){
				int optionID = optionID;
				options.push_back(copy.aviableFarms[j][selectedAviableNode]);
				innedIDs.push_back(selectedAviableNode);
				counter += 1;
				selectedAviableNode += 1;
			}
			if(selectedAviableNode == 0 || (int)copy.aviableFarms[j].size() == 0){
				continue;
			}

			if(selectedAviableNode >= (int)copy.aviableFarms[j].size()){
				selectedAviableNode = selectedAviableNode - (int)copy.aviableFarms[j].size();
				j--;
			}
		}
		
		// now for each posible solution we need Change in milk and change in cost
		float oldFev = copy.FevChangeCostNMilk(0,0,0);

		for (long unsigned int j = 0; j < options.size(); ++j){
			float lastMilk = GetNodeMilkQuantity(copy.routes[i][selectedRoundNode]);
			float newMilk = GetNodeMilkQuantity(options[j]);
			float milkDiff = newMilk - lastMilk;

			float oldCost = copy.CostSidesATNode(i, selectedRoundNode, copy.routes[i][selectedRoundNode]);
			float newCost = copy.CostSidesATNode(i, selectedRoundNode, options[j]);
			float costDiff = newCost - oldCost;

			//  difference
			int typeNew = GetNodeType(options[j]);
			float newFev = copy.FevChangeCostNMilkAndType(costDiff,i, milkDiff, typeNew);

			// For now first Improvement
			if(newFev >= oldFev || blind){
				// remove from aviable 
				copy.aviableFarms[typeNew].erase(copy.aviableFarms[typeNew].begin()+innedIDs[j]);

				// Return it
				int typeOld = GetNodeType(copy.routes[i][selectedRoundNode]);
				copy.aviableFarms[typeOld].push_back(copy.routes[i][selectedRoundNode]);
				// replace
				copy.routes[i][selectedRoundNode] = options[j];

				copy.UpdateRouteCost(i,costDiff);
				copy.UpdateRouteMilk(i,milkDiff);
				copy.UpdateRouteType(i);

				break;
				
			}
		}
	}

	return copy;
}
//////////////////////////////////////////
//			Individual Functions		//
//////////////////////////////////////////
void Individual::Initialize(int routesQ){
	for (int i = 0; i < 3; ++i)
	{
		vector<int> allAviableFarms;
		for(long unsigned int j = 0; j< qu[i].size(); j+=2){
  	        allAviableFarms.push_back(qu[i][j]);
  	        // cout << i << ":" << qu[i][j] << endl;
	    }
	    aviableFarms.push_back(allAviableFarms);
	}

	for (int i = 0; i < routesQ; ++i)
	{
		cost.push_back(0);
	    vector<int> empty;
	    routes.push_back(empty);
	    milkTypePerRoute.push_back(-1);
	}
	indP = P;
	indQ = Q;
}

void Individual::UpdateData(){

	// cout << "First Update data issue " << endl;
	milkPerRoute.clear();
	for(long unsigned int i = 0; i< routes.size(); i++){
		// cout << "filling route " << i << endl;
		float currentCost = 0;
		milkPerRoute.push_back(0);
		for(long unsigned int j = 0; j< routes[i].size(); j++){
  			if(j == 0){
  				currentCost += c[0][routes[i][j]];
  			}
  			else{
  				currentCost += c[routes[i][j-1]][routes[i][j]];
  			}

  			milkPerRoute[i] += (int)GetNodeMilkQuantity(routes[i][j]);
  			if(j == routes[i].size() - 1){
  				currentCost += c[routes[i][j]][0];	
  			}
  		}
  		// cout << "cost " << currentCost << endl;
  		cost[i] = currentCost;
    }
    // cout << milkPerRoute.size() << endl;

}

void Individual::removeAviableNode(int nodeID){
	// bool nodeRemoved = true;
	// cout << nodeID << endl;
	for (int i = 0; i < 3; ++i)
	{
		// cout  << ((int)aviableFarms[i].size() - nodeID) << "????" << ((int)aviableFarms[i].size() - nodeID > 0) << endl;
		if( (int)aviableFarms[i].size() - nodeID > 0){
			// cout << " Remove from avialbe[" << i << "] the node [" << nodeID << "/" << (aviableFarms[i].size() -1) << "]" << endl;
			aviableFarms[i].erase(aviableFarms[i].begin() + nodeID);
			break;
		}
		else{
			nodeID -= aviableFarms[i].size();
		}
	}
}

void Individual::UpdateRouteCost(int routeid, float add){
	// cout << cost[routeid] << "+ (" << add << ") = " << (cost[routeid] + add) << endl;
	cost[routeid] += add;
}

void Individual::UpdateRouteMilk(int routeid, float add){
	// cout << "ADD ??? "<< add << endl;
	milkPerRoute[routeid] += add;
}

void Individual::UpdateRouteType(int routeid){
	int currentType = -1;
	for (int i = 0; i < (int)routes[routeid].size(); ++i)
	{
		int nodeType = GetNodeType(routes[routeid][i]);
		if(currentType < nodeType){
			currentType = nodeType;
		}
	}
	milkTypePerRoute[routeid] = currentType;
}

void Individual::UpdateQ(vector<float> newQ){
	indQ = newQ;
}

void Individual::UpdateFev(){
	float newValue = FevChangeCostNMilk(0,0,0);
	fev = newValue;
}


float Individual::Fev(vector<int> totalMilkCollected, float cost, float overload){
	// missing production & Factory blend
	float missingP = 0;
	for (int i = 0; i < 3; ++i)
	{
		float currentMissing = indP[i] - totalMilkCollected[i];

		// blend at factory
		if(i == 1 && currentMissing > 0){
			int milkAExcess = totalMilkCollected[0] - indP[0];
			if(milkAExcess > 0 && currentMissing > 0){
				 int blendQ = milkAExcess > currentMissing ? currentMissing : milkAExcess;
				 totalMilkCollected[0] -= blendQ;
				 totalMilkCollected[1] += blendQ;
				 currentMissing -= blendQ;
			}
		}
		if(i == 2 && currentMissing > 0){
			// blend with B
			int milkBExcess = totalMilkCollected[1] - indP[1];
			if(milkBExcess > 0 && currentMissing > 0){
				 int blendQ = milkBExcess > currentMissing ? currentMissing : milkBExcess;
				 totalMilkCollected[1] -= blendQ;
				 totalMilkCollected[2] += blendQ;
				 currentMissing -= blendQ;
			}
			// blend with A :s
			int milkAExcess = totalMilkCollected[0] - indP[0];
			if(milkAExcess > 0 && currentMissing > 0){
				 int blendQ = milkAExcess > currentMissing ? currentMissing : milkAExcess;
				 totalMilkCollected[0] -= blendQ;
				 totalMilkCollected[2] += blendQ;
				 currentMissing -= blendQ;
			}
		}
		missingP += currentMissing > 0 ? currentMissing:0;
	}

	float gain = totalMilkCollected[0] * aValue + totalMilkCollected[1] * bValue + totalMilkCollected[2]*cValue - cost;
	return gain + (overload * overloadPenalty) - (missingP * missingPPenalty);
}



/// Update the current routes, and spill the overload to others ... (just for Fev calculation)
// FEV Inidividual
float Individual::FevChangeVehicleQ(vector<float> newQ){
	
	float overload = 0;
	float totalCost = 0;
	
	vector<int> totalMilkCollected;
	totalMilkCollected.push_back(0);
	totalMilkCollected.push_back(0);
	totalMilkCollected.push_back(0);

	for (long unsigned  int i = 0; i < milkPerRoute.size(); ++i)
	{
		// Overload capacity, per vehicle if it's positive 
		float vehicleOverload =  newQ[i] - (milkPerRoute[i]);
		overload += vehicleOverload < 0? vehicleOverload : 0;
		// Milk collected & gain
		totalMilkCollected[milkTypePerRoute[i]] += milkPerRoute[i];
		totalCost += cost[i];
	}

	return Fev(totalMilkCollected, totalCost, overload);
}


// Since we change just one route at a time, we need to keep the rest as it is, so the cost is gonna be updated to be tested here
float Individual::FevChangeCostNMilk(float costChange, int routeid, int milkChange){

	// cout << "FEV ??? " << endl;
	float overload = 0;
	float totalCost = 0;
	
	vector<int> totalMilkCollected;
	totalMilkCollected.push_back(0);
	totalMilkCollected.push_back(0);
	totalMilkCollected.push_back(0);
	// cout << "...." << endl;
	// cout << milkPerRoute.size() << endl;
	// First milk of each type
	for (long unsigned  int i = 0; i < milkPerRoute.size(); ++i)
	{
		// cout << "check route " << i << endl;
		// Overload capacity, per vehicle if it's positive 
		float vehicleOverload =  indQ[i] - (milkPerRoute[i] + (routeid == (int)i ? milkChange : 0));
		overload += vehicleOverload < 0? vehicleOverload : 0;

		// Milk collected & gain
		if(milkTypePerRoute[i] != -1){
			totalMilkCollected[milkTypePerRoute[i]] += milkPerRoute[i] + (routeid == (int)i ? milkChange : 0);
		}
		totalCost += cost[i];
	}

	totalCost += costChange;
	// float gain = milkGain - totalCost + costChange;
	return Fev(totalMilkCollected, totalCost, overload);
}

// Since we change just one route at a time, we need to keep the rest as it is, so the cost is gonna be updated to be tested here
float Individual::FevChangeCostNMilkAndType(float costChange, int routeid, int milkChange, int newType){

	// cout << "FEV ??? " << endl;
	float overload = 0;
	float totalCost = 0;

	vector<int> totalMilkCollected;
	totalMilkCollected.push_back(0);
	totalMilkCollected.push_back(0);
	totalMilkCollected.push_back(0);
	// cout << "...." << endl;
	// cout << milkPerRoute.size() << endl;
	// First milk of each type
	for (long unsigned  int i = 0; i < milkPerRoute.size(); ++i)
	{
		// cout << "check route " << i << endl;
		// Overload capacity, per vehicle if it's positive 
		float vehicleOverload =  indQ[i] - (milkPerRoute[i] + (routeid == (int)i ? milkChange : 0));
		overload += vehicleOverload < 0? vehicleOverload : 0;

		// Milk collected & gain
		if(milkTypePerRoute[i] != -1){
			if((int)i != routeid){
				totalMilkCollected[milkTypePerRoute[i]] += milkPerRoute[i];
			}
			else{
				int finalType = milkTypePerRoute[routeid] < newType? newType : milkTypePerRoute[i];
				totalMilkCollected[finalType] += milkPerRoute[i] + milkChange ;
			}
		}
		else{
			if((int)i == routeid){
				int finalType = milkTypePerRoute[routeid] < newType? newType : milkTypePerRoute[i];
				totalMilkCollected[finalType] += milkPerRoute[i] + milkChange ;
			}
		}
		totalCost += cost[i];
	}

	totalCost += costChange;
	// float gain = milkGain - totalCost + costChange;
	return Fev(totalMilkCollected, totalCost, overload);
}


float Individual::GetCost(bool sideLeft, int routeid, int nodeid, int selectedValue){
	int cost = 0;
	if(sideLeft){
		if (nodeid == 0){
			// cout << "F to " << selectedValue << endl;
			cost += (int)c[0][selectedValue];
		}
		else{
			// cout  << routes[routeid][nodeid-1] << "to " << selectedValue << endl;
			cost += (int)c[routes[routeid][nodeid-1]][selectedValue];
		}
	}
	else{
		// Right
		if (nodeid >= (int)routes[routeid].size() -1){
			// cout << selectedValue << " to F" << endl;
			cost += (int)c[selectedValue][0];
		}
		else{
			//  selectedValue -> [nodeid - 1]
			// cout  << selectedValue << "to " << routes[routeid][nodeid+1] << endl;
			cost += (int)c[selectedValue][routes[routeid][nodeid+1]];
		}
	}
	return cost;
}

float Individual::moveCostFromNode(int routeid, int nodeid1, int nodeid2, int selectedValue1, int selectedValue2){
	
	// Left and right cost
	int totalCost = 0;
	// we can have 3 or 4 node changes !
	if(nodeid1 == nodeid2 + 1 || nodeid1 == nodeid2 - 1){
		// 3 nodes
		if(nodeid1 > nodeid2){
			int aux = nodeid1;
			nodeid1 = nodeid2;
			nodeid2 = aux;
			aux = selectedValue1;
			selectedValue1 = selectedValue2;
			selectedValue2 = aux;
		}
		// left, normal
		totalCost += GetCost(true, routeid, nodeid1, selectedValue1);
		// Right Conflicted
		// cout << selectedValue1 << " to " << selectedValue2 << endl;
		totalCost += (int)c[selectedValue1][selectedValue2];
		// right, normal
		totalCost += GetCost(false, routeid, nodeid2, selectedValue2);
	}
	else
	{
		// (left and right from nodeid) x 2
		totalCost += GetCost(true, routeid, nodeid1, selectedValue1);
		totalCost += GetCost(false, routeid, nodeid1, selectedValue1);

		totalCost += GetCost(true, routeid, nodeid2, selectedValue2);
		totalCost += GetCost(false, routeid, nodeid2, selectedValue2);
	}

	// routes[routeid][nodeid]
	return totalCost;
}


float Individual::CostFromRemoveNode(int routeid, int nodeid){
	
	// Left and right cost
	int totalCost = 0;

	// (left and right from nodeid) x 2
	totalCost -= GetCost(true, routeid, nodeid, routes[routeid][nodeid]);
	totalCost -= GetCost(false, routeid, nodeid, routes[routeid][nodeid]);
	// cout << "cost to remove?" << totalCost << "modeid ???" << nodeid << endl;

	// If only node, we just remove the costs
	if(routes[routeid].size() > 1){
		if (nodeid == 0){
			totalCost += (int)c[0][routes[routeid][nodeid + 1]];
		}
		else if(nodeid >= (int)routes[routeid].size() -1){
			totalCost += (int)c[routes[routeid][nodeid - 1]][0];
		}
		else{
			totalCost += (int)c[routes[routeid][nodeid - 1]][routes[routeid][nodeid + 1]];
		}
	}
	// cout << "cost to remove?" << totalCost << endl;
	// routes[routeid][nodeid]
	return totalCost;
}

// Cost
float Individual::CostSidesATNode(int routeid, int nodePosition, int nodeid){
	
	// Left and right cost
	int totalCost = 0;

	// (left and right from nodeid) x 2
	totalCost += GetCost(true, routeid, nodePosition, nodeid);
	totalCost += GetCost(false, routeid, nodePosition, nodeid);

	return totalCost;
}
// assumes at least 1 node on the route
float Individual::CostFromAddNode(int routeid, int nodeid, int selectedNode){
	// Left and right cost
	int totalCost = 0;

	// (left and right from nodeid) x 2
	if(routes[routeid].size() == 0){
		totalCost = 0; // No removal !
		totalCost += (int)c[0][selectedNode];
		totalCost += (int)c[selectedNode][0];
	}
	else{
		// Remove arc
		if(nodeid == 0){
			totalCost -= (int)c[0][routes[routeid][nodeid]];
		}
		else if(nodeid == (int)routes[routeid].size()){
			totalCost -= (int)c[routes[routeid][nodeid - 1]][0];
		}
		else{
			totalCost -= (int)c[routes[routeid][nodeid - 1]][routes[routeid][nodeid]];
		}
		// add right arc
		if (nodeid == 0){
			totalCost += (int)c[0][selectedNode];
		}
		else{
			totalCost += (int)c[routes[routeid][nodeid - 1]][selectedNode];
		}

		// add left arc
		if (nodeid == (int)routes[routeid].size()){
			totalCost += (int)c[selectedNode][0];
		}
		else{
			totalCost += (int)c[selectedNode][routes[routeid][nodeid]];
		}
	}

	// cout << "TC" << totalCost << endl;

	

	return totalCost;
}

void Individual::PrintSolution(bool extra, bool PQextra, bool infoOnly){
	float milkGain = 0;
	float totalTravelCost = 0;
	bool factoryBlend = false;
	bool truckBlend = false;
	// 3 milks
	vector<int> totalMilkCollected;
	totalMilkCollected.push_back(0);
	totalMilkCollected.push_back(0);
	totalMilkCollected.push_back(0);
	vector<float> vehicleOverload;

    // Normal cost
    for (long unsigned  int i = 0; i < milkPerRoute.size(); ++i)
    {    	
    	totalTravelCost += cost[i];
    	totalMilkCollected[milkTypePerRoute[i]] += milkPerRoute[i];
    	vehicleOverload.push_back(milkPerRoute[i] - indQ[i]);

    	// cout << "Filling " << i << milkTypePerRoute[i] << (milkPerRoute[i] - indQ[i]) << endl;
    	// Has truck blending ?
    	for (long unsigned int j = 0; j < routes[i].size(); ++j)
    	{
    		if(milkTypePerRoute[i] != GetNodeType(routes[i][j])){
    			truckBlend = true;
    		}
    	}
    }

    // blend at factory
    for (int i = 0; i < 3; ++i)
	{
		float currentMissing = indP[i] - totalMilkCollected[i];
		if(i == 1 && currentMissing > 0){
			int milkAExcess = totalMilkCollected[0] - indP[0];
			// cout << milkAExcess << " missingB ? " << currentMissing << endl;
			if(milkAExcess > 0 && currentMissing > 0){
				 factoryBlend = true;
				 int blendQ = milkAExcess > currentMissing ? currentMissing : milkAExcess;
				 totalMilkCollected[0] -= blendQ;
				 totalMilkCollected[1] += blendQ;
				 currentMissing -= blendQ;
			}
		}
		if(i == 2 && currentMissing > 0){

			// blend with B
			int milkBExcess = totalMilkCollected[1] - indP[1];
			// cout << milkBExcess << " B ->missingC ? " << currentMissing << endl;
			if(milkBExcess > 0 && currentMissing > 0){
				factoryBlend = true;
				 int blendQ = milkBExcess > currentMissing ? currentMissing : milkBExcess;
				 totalMilkCollected[1] -= blendQ;
				 totalMilkCollected[2] += blendQ;
				 currentMissing -= blendQ;
			}
			// blend with A :s
			int milkAExcess =  totalMilkCollected[0] - indP[0];
			// cout << milkAExcess << " A ->missingC ? " << currentMissing << endl;
			if(milkAExcess > 0 && currentMissing > 0){
				factoryBlend = true;
				 int blendQ = milkAExcess > currentMissing ? currentMissing : milkAExcess;
				 totalMilkCollected[0] -= blendQ;
				 totalMilkCollected[2] += blendQ;
				 currentMissing -= blendQ;
			}
		}
	}

    // print gains, costs and gains,
    milkGain =  totalMilkCollected[0] * aValue + totalMilkCollected[1] * bValue + totalMilkCollected[2]*cValue;
    float finalGain = milkGain - totalTravelCost;
    if(PQextra == false){
    	cout << finalGain << " " << totalTravelCost << " " << milkGain << endl;
	}
	else{
		string un = "";
		for (int i = 0; i < 3; ++i)
		{
			if(totalMilkCollected[i] < indP[i] ){
				un = un + "UM"+ std::to_string(i);
			}
		}
		
		if(vehicleOverload.size() > 0){
	      for(long unsigned int i = 0; i < milkPerRoute.size(); i++){
	        if(milkPerRoute[i] - indQ[i] > 0){
	        	un = un + "UQ"+std::to_string(i);
	        	// cout << "overload...."  << i << endl;
	        	// cout << un << endl;
	        }
	      }
	    }
	    un = truckBlend? un + "_TB" : un + "_NTB";
	    un = factoryBlend? un + "_FB" : un + "_NFB";

	    if(infoOnly){
			cout << finalGain << " " << un << " " << totalTravelCost << " " << milkGain << " ";	
		}
		else{
			cout << finalGain << " " << un << " " << totalTravelCost << " " << milkGain << endl;	
		}
	}

	if(infoOnly){
		int mm = totalMilkCollected[0] + totalMilkCollected[1] + totalMilkCollected[2] - indP[0] - indP[1] - indP[2];
		mm = mm < 0 ? mm * -1:0;
		int totalOverload = 0;

		if(vehicleOverload.size() > 0){
	      for(long unsigned int i = 0; i < milkPerRoute.size(); i++){
	      	totalOverload += milkPerRoute[i] - indQ[i] > 0? milkPerRoute[i] - indQ[i] : 0;
	      }
	    }
	    int fev = FevChangeCostNMilk(0,0,0);
		cout << mm << " " << totalOverload  << " " << fev << endl;
		return;
	}

    cout << "MA:" << totalMilkCollected[0] << " MB:" << totalMilkCollected[1] << " MC:" << totalMilkCollected[2] << endl;
    // Print routes
    for(long unsigned int i = 0; i < routes.size(); i++){
    	cout << "[";
        if (routes[i].size() > 0){
            cout << routes[i][0];
            for(long unsigned int j = 1; j < routes[i].size(); j++){
                cout << "-"<< routes[i][j];
            }
        }
        cout << "] [" << milkTypePerRoute[i] << "]";
        cout << endl;
    }
    if(extra) {
    	for (long unsigned  int i = 0; i < milkPerRoute.size(); ++i){
    		cout << "Cost per route " << i << ": " << cost[i] << endl;
	    }
	    // Print if the solution is Unfeseable
	    if(totalMilkCollected[0] < indP[0] || totalMilkCollected[1] < indP[1] ||totalMilkCollected[2] < indP[2]){
	      cout << endl;
	      cout << "Unfeseable solution: " << endl;
	      if (totalMilkCollected[0] < indP[0]){
	        cout << "Missing Milk A (" << totalMilkCollected[0] << "/" << indP[0] << ") " << (indP[0] - totalMilkCollected[0]) << endl;
	      }
	      if (totalMilkCollected[1] < indP[1]){
	        cout << "Missing Milk B (" << totalMilkCollected[1] << "/" << indP[1] << ") " << (indP[1] - totalMilkCollected[1]) << endl;
	      }
	      if (totalMilkCollected[2] < indP[2]){
	        cout << "Missing Milk C (" << totalMilkCollected[2] << "/" << indP[2] << ") " << (indP[2] - totalMilkCollected[2]) << endl;
	      }
	    }
	    cout << endl;
	    // Print if the cars are overloaded (+) and underLoad (-)
	    cout << "Route Overload (+) or UnderLoad (-)" << endl;
	    if(vehicleOverload.size() > 0){
	      for(long unsigned int i = 0; i < milkPerRoute.size(); i++){
	        cout << "Route " << i << ":" << (milkPerRoute[i] - indQ[i]) << "[" << milkPerRoute[i]<< "/" <<  indQ[i] << "]" << endl;;
	      }
	    }
	    else{
	      cout << "NO ROUTES WHERE FILLED !!!" << endl;
	    }
	    cout << endl << "Aviable Farms [" << (aviableFarms[0].size() + aviableFarms[1].size() + aviableFarms[2].size()) << "]"<< endl;
	    vector<char> type;
	    type.push_back('A');
	    type.push_back('B');
	    type.push_back('C');
	    for (long unsigned int i = 0; i < aviableFarms.size(); ++i)
	    {
	    	cout << type[i] <<": "  << aviableFarms[i].size() << endl;
	    	for (long unsigned int j = 0; j < aviableFarms[i].size(); ++j)
	    	{
		    	cout << aviableFarms[i][j] << ": [" << GetNodeMilkQuantity(aviableFarms[i][j]) << "]" << endl;
	    	}
	    }
	    // All nodes
	    /*
	    cout << endl << "ALL NODES [" << ((qu[0].size() + qu[1].size() + qu[2].size())/2) << "]" << endl;
	    for (long unsigned int i = 0; i < 3; ++i)
	    {
	    	cout << type[i] <<": "  << (qu[i].size()/2) << endl;
		    for(long unsigned int j = 0; j< qu[i].size(); j+=2){
	  	        cout << qu[i][j] << ": [" << GetNodeMilkQuantity(qu[i][j]) << "]" << endl;
		    }
		}
		*/
	}
}

void Individual::PrintSolutionFromRoutes(){
    
    // Important information to display !
    float travelCost = 0;
    float milkGain = 0;
    float finalGain = 0;
	vector<int> totalMilkCollected;
	totalMilkCollected.push_back(0);
	totalMilkCollected.push_back(0);
	totalMilkCollected.push_back(0);
    // Overloaded vector
    vector<float> overload;

    // Solutions are just vectors, so we will translate them to an actual solution representation
    // Obtaining the cost !
    for(long unsigned int i = 0; i < routes.size(); i++){
        int finalType = 0;
        int currentMilk = 0;
        for(long unsigned int j = 0; j < routes[i].size(); j++){
            // ! First cost, from the factory to the first node
            if(j == 0){
                travelCost += (int)c[0][routes[i][j]];
            }
            // Normal cost, from every point to the next
            else{
                travelCost += (int)c[routes[i][j-1]][routes[i][j]];
            }
            
            // Obtain node data
            vector<int> nodeData = quIds[routes[i][j]-offset];
            
            // Current milk, from node data
            currentMilk += qu[nodeData[0]][nodeData[1]+1];

            // type of milk on the current node
            if(nodeData[0] == 0 && finalType == 0){
                finalType = 0;
            }
            if(nodeData[0] == 1 && finalType <=1){
                finalType = 1;
            }
            if(nodeData[0] == 2){
                finalType = 2;
            }

            // ! from the last farm to the factory
            if(j == routes[i].size() -1){
                travelCost += (int)c[routes[i][j]][0];
            }
        }
        // Depending on the type of milk added (the worst type), we assign the current milk to the group
        if(finalType == 0)
            totalMilkCollected[0] += currentMilk;
        if(finalType == 1)
            totalMilkCollected[1] += currentMilk;
        if(finalType == 2)
            totalMilkCollected[2] += currentMilk;

        // Check Overload of the current Route
        overload.push_back(currentMilk - Q[i]);
    }

    // blend at factory
    for (int i = 0; i < 3; ++i)
	{
		float currentMissing = indP[i] - totalMilkCollected[i];
		if(i == 1 && currentMissing > 0){
			int milkAExcess = totalMilkCollected[0] - indP[0];
			// cout << milkAExcess << " missingB ? " << currentMissing << endl;
			if(milkAExcess > 0 && currentMissing > 0){
				 int blendQ = milkAExcess > currentMissing ? currentMissing : milkAExcess;
				 totalMilkCollected[0] -= blendQ;
				 totalMilkCollected[1] += blendQ;
				 currentMissing -= blendQ;
			}
		}
		if(i == 2 && currentMissing > 0){

			// blend with B
			int milkBExcess = totalMilkCollected[1] - indP[1];
			// cout << milkBExcess << " B ->missingC ? " << currentMissing << endl;
			if(milkBExcess > 0 && currentMissing > 0){
				 int blendQ = milkBExcess > currentMissing ? currentMissing : milkBExcess;
				 totalMilkCollected[1] -= blendQ;
				 totalMilkCollected[2] += blendQ;
				 currentMissing -= blendQ;
			}
			// blend with A :s
			int milkAExcess =  totalMilkCollected[0] - indP[0];
			// cout << milkAExcess << " A ->missingC ? " << currentMissing << endl;
			if(milkAExcess > 0 && currentMissing > 0){
				 int blendQ = milkAExcess > currentMissing ? currentMissing : milkAExcess;
				 totalMilkCollected[0] -= blendQ;
				 totalMilkCollected[2] += blendQ;
				 currentMissing -= blendQ;
			}
		}
	}
    // Gain from Milk and final Gain
    milkGain = (totalMilkCollected[0] * aValue + totalMilkCollected[1] *bValue + totalMilkCollected[2] *cValue);
    finalGain = milkGain - travelCost;
    // cout << 0 << endl;
    float gap = ((float(bestKnownValue) - std::round((finalGain)))/float(bestKnownValue)) * 100;
	cout << gap << endl;

	if( sint == false){
	    cout <<(std::round((finalGain)) * -1)<< endl;
	    cout << finalGain << " " << travelCost << " " << milkGain << endl;
	    cout << "MA:" << totalMilkCollected[0] << " MB:" << totalMilkCollected[1] << " MC:" << totalMilkCollected[2] << endl;
	}
    
    /*
    /// Print routes
    for(long unsigned int i = 0; i < routes.size(); i++){
        if (routes[i].size() > 0){
            cout << routes[i][0];
            for(long unsigned int j = 1; j < routes[i].size(); j++){
                cout << "-"<< routes[i][j];
            }
        }
        cout << endl;
    }
    */
}
