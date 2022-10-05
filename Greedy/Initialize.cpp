#include "Initialize.h"
#include <bits/stdc++.h>

using namespace std;
using std::ifstream;


void Capture_Params(int argc, char** argv){
  mode = argv[1];
  InstanceFile = argv[2];
  Seed = atoi(argv[3]);
  useOverFill = atoi(argv[4]) == 1;
}

// Try yo found ":="" then continue reading
string Clean_Lines(string lines){
  string cleaned_line = "";
  string currentLine = "";
  bool foundToken = false;
  long unsigned int lineLength = lines.length();
  for(long unsigned int i = 0; i < lineLength; i++){
    if(!foundToken){
      if(lines[i-1] == ':' && lines[i] == '='){
        foundToken = true;
      }
    }
    else {
      if (lines[i] == '\t'){
        currentLine += ' ';
      }
      else{
        if(lines[i] == '\n'){
          if(currentLine.length() != 0){
            cleaned_line += currentLine + '\n';
          }
          currentLine = "";
        }
        else {
          currentLine += lines[i];
        }
      }
    }

  }
  return cleaned_line;
}

// Try yo found ":="" then continue reading
string Replace_inLines(string lines, char from, bool replaceBySpace){
  string cleaned_line = "";
  long unsigned int lineLength = lines.length();
  for(long unsigned int i = 0; i < lineLength; i++){
    if (lines[i] == from){
      if(replaceBySpace){
        cleaned_line += " ";
      }
    }
    else{
      cleaned_line += lines[i];
    }
  }
  return cleaned_line;
}

vector<string> Cut_Line(string line, char delimiter){
  vector<string> data;
  long unsigned int lineLength = line.length();
  string currentData = "";
  for(long unsigned int i = 0; i < lineLength; i++){
    if (line[i] == delimiter){
      data.push_back(currentData);
      currentData = "";
    }
    else{
      currentData += line[i];
    }
  }
  if(currentData.length()> 0){
    data.push_back(currentData);
  }
  return data;
}

vector<vector<int>> Check_Group_Line(string groupLine, bool debug = false){
  vector<vector<int>> groupData;
  string lines = Clean_Lines(groupLine);
  string::size_type posLine = 0;
  string::size_type prevLine = 0;
  while ((posLine = lines.find('\n', prevLine)) != std::string::npos) {
    vector<int> currentData;
    string line = lines.substr(prevLine, posLine - prevLine);
    string::size_type posValue = 0;
    string::size_type prevValue = 0;
    while ((posValue = line.find(' ', prevValue)) != std::string::npos) {
      string elem = line.substr(prevValue, posValue - prevValue);
      if(elem.length() > 0){
        if(elem == "."){
          elem = "0";  
        }
        currentData.push_back(stoi(elem));
        if(debug) cout << elem << " ";
      }
      prevValue = posValue + 1;
    }
    
    // last element 
    string last = line.substr(prevValue);
    // cout << "last ? [" << last << "]"  << endl;
    // cout << last.length() << " " << last[0] << endl;
    // cout << int(last[0]) << endl;
    if ( int(last[0]) != 13){
      if(last.length() > 0){
        
        if(last == "."){
          last = "0";        
        }
        currentData.push_back(stoi(last));
        if(debug) cout << last;
      }
      groupData.push_back(currentData);
      if(debug) cout << endl;
    }
    prevLine = posLine + 1;
  }
  return groupData;
}

// Function to calculate distance
float Getdistance(int x1, int y1, int x2, int y2)
{
    // Calculating distance
    return (int)round(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0));
}

void SetDistanceMatrix(){
  for(long unsigned int i = 0; i < positions.size(); i++){
    vector<float> distance;
    for(long unsigned int j = 0; j < positions.size(); j++){
      if(i == j){
        distance.push_back(0);
      }
      else{
        distance.push_back(Getdistance(positions[i][0], positions[i][1], positions[j][0],positions[j][1]));
      }
    }
    c.push_back(distance);
  }

}

// Each section of the file ends in a ";", indicating this section ended
// 7 diferent file information
// Nodes, Predios, Camiones, Capacidades, # Min Recolectar, Prod x Predio, Cost transport
// set Nodos:=, set I:=, set K:=, param Q:=, param P:=, param qu :=, param c[*,*] : start to last:=
// End of sections ";"
bool Read_Instance(){
  ifstream file;
  // int section = 0;
  const char DELIMITEREND = ';';
  // const char DELIMITERLINE = '\n';
  string groupLine;
  int sectionCounter = 0;
  // cout << "InstanceFile:" << InstanceFile << " Start reading !"<< endl;
  file.open(InstanceFile); // open a file
  if (file.fail())
    return true; // exit if file not found
  
  // Read file
  while(getline(file, groupLine, DELIMITEREND)){
    sectionCounter++;
    if(sectionCounter < 4){
      continue;
    }    
    if(sectionCounter == 4){ // Capacidad x camion (Q and QName)
      vector<vector<int>> allData = Check_Group_Line(groupLine);
      for(vector<int> vdata : allData){
        QName.push_back(vdata.at(0));
        Q.push_back(vdata.at(1));
        // cout << vdata.at(0) << " " << vdata.at(1) << endl;
      }

    }
    
    if(sectionCounter == 5){ // Min to collect (P)
      vector<vector<int>> allData = Check_Group_Line(groupLine);
      for(vector<int> vdata : allData){
        P.push_back(vdata.at(0));
        // cout << vdata.at(0) << endl;
      }
    } 
    
    if(sectionCounter == 6){ // Prod x predio (qu)
      for(long unsigned int i = 0; i < P.size();i++){
        vector<float> empty;
        qu.push_back(empty);
      }
      vector<vector<int>> allData = Check_Group_Line(groupLine);
      int flag = false;
      for(vector<int> vdata : allData){
        if(flag){
          // cout << vdata.at(0) << " " << vdata.at(1) << " " << vdata.at(2) << endl;
          int type = vdata.at(2) - 1;
          qu[type].push_back(vdata.at(0));
          qu[type].push_back(vdata.at(1));
          vector<int> currentNode;
          currentNode.push_back(type);//type
          currentNode.push_back((int)qu[type].size()-2); // position
          // cout << type << " " << qu[type].size()-2 << endl;
          quIds.push_back(currentNode); 
        }
        else{
          flag = true;
          firstNode = vdata.at(0);
        }
      }
    }
    if(sectionCounter == 7){ // travel cost (c)
      vector<vector<int>> allData = Check_Group_Line(groupLine);
      int count = 0;
      for(vector<int> vdata : allData){
        bool flag = false;
        for(int elem : vdata){
          if(flag){
            c[count].push_back(elem);
            // cout << elem << " ";
          }
          else{
            vector<float> empty;
            c.push_back(empty);
            // cout << elem << ": [";
            flag = true;
          }
        }
        count++;
        // cout << "]" << endl;
      }
    }
    
    // cout << Clean_Lines(line);
    // cout << "End of group" << endl;
  }

  // Finished
  return false;
}

bool Read_Instance_old(){

  ifstream file;
  string groupLine;
  int counter = 0;
  int elementCounter = 0;
  file.open(InstanceFile); // open a file
  if (file.fail())
    return true; // exit if file not foun
  // cout << "[" << InstanceFile << "]"<< endl;
  // Read file
  while(getline(file, groupLine, '\n')){ 

    if(counter == 1){ // Capacidad x camion (Q and QName)
      groupLine = Replace_inLines(groupLine, ' ', false);
      vector<string> data = Cut_Line(groupLine, '\t');
      Q.push_back(stoi(data.at(0)));
      Q.push_back(stoi(data.at(1)));
      Q.push_back(stoi(data.at(2)));
      // cout << Q.at(0) << " " << Q.at(1) << " " << Q.at(2) << endl;
    }
    
    if(counter == 4){ // Min to collect (P)
      groupLine = Replace_inLines(groupLine, ' ', false);
      vector<string> data = Cut_Line(groupLine, '\t');
      P.push_back(stoi(data.at(0)));
      P.push_back(stoi(data.at(1)));
      P.push_back(stoi(data.at(2)));
      // cout << P.at(0) << " " << P.at(1) << " " << P.at(2) << endl;
    } 
    
    if(counter == 5){ // Min to collect (P)
      groupLine = Replace_inLines(groupLine, ' ', false);
      vector<string> data = Cut_Line(groupLine, '\t');
      aValue = stof(data.at(0));
      bValue = stof(data.at(1));
      cValue = stof(data.at(2));
      // cout << aValue << " " << bValue << " " << cValue << endl;
    } 

    if(counter >= 8){ // Prod x predio (qu)
      if ( counter == 8){
        for(long unsigned int i = 0; i < P.size();i++){
          vector<float> empty;
          qu.push_back(empty);
        }
      }
      vector<int> currentNode;
      groupLine = Replace_inLines(groupLine, '\t', true);
      vector<string> data = Cut_Line(groupLine, ' ');
      // cout << groupLine << endl;
      if (data.at(3) == "A"){
        qu[0].push_back(elementCounter);
        qu[0].push_back(stoi(data.at(4)));
        currentNode.push_back(0);
        currentNode.push_back((int)qu[0].size()-2);
        // cout << "A" << endl;
      }
      if (data.at(3) == "B"){
        qu[1].push_back(elementCounter);
        qu[1].push_back(stoi(data.at(4)));
        currentNode.push_back(1);
        currentNode.push_back((int)qu[1].size()-2);
        // cout << "B" << endl;
      }
      if (data.at(3) == "C"){
        qu[2].push_back(elementCounter);
        qu[2].push_back(stoi(data.at(4)));
        currentNode.push_back(2); // save type
        currentNode.push_back((int)qu[2].size()-2); // save position
        // cout << "C" << endl;
      }
      vector<int> pos;
      pos.push_back(stoi(data.at(1)));
      pos.push_back(stoi(data.at(2)));
      positions.push_back(pos);
      if (counter > 8){
        // cout << currentNode[0] << " " << currentNode[1] << " s"<< quIds.size()<< endl;
      }
      quIds.push_back(currentNode);   
      elementCounter++;
    }
    counter++;
  }
  SetDistanceMatrix();
  // Finished
  return false;

}

void Preprocess(){
  // rC
  vector<float> multiplier;
  multiplier.push_back(1);
  multiplier.push_back(0);
  multiplier.push_back(0);

  for(int id = 0; id < 3; id++){
    vector<vector<float>> costs;
    
    float currentMilkMultiplier = aValue;
    if(id == 1){
      multiplier[0] = 0.99;
      multiplier[1] = 1;
      currentMilkMultiplier = bValue;
    }
    if(id == 2){
      multiplier[0] = 0.98;
      multiplier[1] = 0.99;
      multiplier[1] = 1;
      currentMilkMultiplier = cValue;
    }
    for(long unsigned int i = 0; i< c.size(); i++){
      vector<float> cost;
      // cout << i << ": [";
      for(long unsigned int j = 0; j< c[i].size(); j++){
        // int travelCost = (int)c[0][sNodes[i]];
        
        if (i == j || j == 0) {
          cost.push_back(-1);
        }
        else{
          vector<int> nodeData = quIds[j];
          float currentMilk = qu[nodeData[0]][nodeData[1]+1];
          float delta = (currentMilk * currentMilkMultiplier - c[i][j]);
          delta = delta * multiplier[nodeData[0]];
          // cout << c[i][j] << "(" << delta  << ")" << " ";
          cost.push_back(c[i][j]);
        }
      }
      // cout << "]" << endl;
      costs.push_back(cost);
      // cout << "First ?" << endl;
    }
    rC.push_back(costs);
  }
  // cout << "END" << endl;
}

void PrintInstanceData(){
  // Q, Qname
  cout << "Vehicules"<< endl;
  for(long unsigned int i = 0; i < Q.size(); i++){
    cout << Q[i] << endl;
  }
  // P
  cout << "Requirments"<< endl;
  for(long unsigned int i = 0; i< P.size(); i++){
    cout << i << " :" << P[i] << endl;
  }
  // qu
  cout << "Production"<< endl;
  for(long unsigned int i = 0; i< qu.size(); i++){
    for(long unsigned int j = 0; j< qu[i].size(); j+=2){
      cout << (i+1) << " n:" << qu[i][j+0] << " p:" << qu[i][j+1] << endl;
    }
  }
  // qids
  cout << "quIds"<< endl;
  for(long unsigned int i = 1; i< quIds.size(); i+=1){    
    cout << (i) << " n:" << quIds[i][0] << endl;
  }
  // c
  cout << "Connections"<< endl;
  for(long unsigned int i = 0; i< c.size(); i++){
    cout << i << ": [";
    for(long unsigned int j = 0; j< c[i].size(); j++){
      cout << c[i][j] << " ";
    }
    cout << "]" << endl;
  }
  // Last line
  cout << "All solutions starts from the node " << firstNode << endl;
}

int int_rand (int a, int b){
  int retorno = 0;
  if (a < b){
    retorno = (int) ((b - a) * drand48());
    retorno = retorno + a;
  }
  else{
    retorno = (int) ((a - b) * drand48());
    retorno = retorno + b;
  }
  return retorno;
}

float float_rand (float a, float b){
  float retorno = 0;
  if (a < b){
    retorno = (float) ((b - a) * drand48());
    retorno = retorno + a;
  }
  else{
    retorno = (float) ((a - b) * drand48());
    retorno = retorno + b;
  }
  return retorno;
}

