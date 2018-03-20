#include<fstream>
#include<iostream>
#include<string>
using namespace std;
int main(){
  ifstream myfile;
  myfile.open("association_rules.csv");
  string store;
  while(getline(myfile,store,',')){
    int num_items = stoi(store);
    for(int x=0;x<num_items;x++){
      getline(myfile,store,',');
      cout << store << " ";
    }
    getline(myfile,store);
    float prob = stof(store);
    cout << prob<< "\n";
 }
 myfile.close();





}
