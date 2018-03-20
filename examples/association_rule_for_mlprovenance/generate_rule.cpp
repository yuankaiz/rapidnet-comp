#include<fstream>
#include<cstdlib>
#include<string>
#include<fstream>
#include<cstdlib>
#include<string>
using namespace std;

int main(){
	string items [] = {"tissue", "napkin", "saucepan","cereal","baby food", "spoon","shirt", 
	                   "paper plate", "paper cup","juice","paper clip","ziplock bags","detergent","vermicelli pasta","unsalted butter", "tomatoes", "ice cream",
	                   "toilet paper","pepper","sugar","rice cakes","papaya","oranges","nestle quik","macaroni","lima beans",
	                   "kale","jelly","lettuce","hummus","tahini","grapes","string cheese","crockery","turkey","chicken",
	                   "fish","yam","watermelon","vegetable oil","rice","texas toast","sandwich bags","quiche","pumpkin pie filling","mangos",
	                   "limes", "krispy kreme doughnuts","flour","eggs", "nuts","fabric softener","tea","broom","cookies","banana",
	                   "avocados","cream cheese","carrots","potatoes","frozen vegetables","frozen pizza","mushrooms","fresh fruits","paper towel","mop",
	                   "coconut milk","milk","yoghurt","coca cola","lemonade","fizzy drink","lays","doritos","ranch dip","salsa",
	                   "spaghetti","marinara sauce","sundried tomatos","almond milk","spinach","onions","barbeque sauce","beef","toothpicks",
	                   "deoderant","pain reliever","aspirin","toothpaste","dishwashing detergent","muffins","baby formula","mayonaise","macaroni","salad dressing",
	                   "coffee","bread", "beer" ,"candy","diaper"
	               };
	fstream myfile;
	myfile.open("association_rules.csv");
	for(int k=0;k<80;k++){
	int times = rand()%4; // No. of items on the rhs	
        myfile <<times+2 << "," << items[rand()%95] << ","; // It won't pick coffee, bread, beer, candy or diaper to be the lhs
    	
    	for(int i=0;i<=times;i++){
    		myfile << items[rand()%100];
    		if(i==times) myfile <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
    		else myfile << ",";
    	}
	}
	myfile << 2 << "," << "diaper" << "," << "beer" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
        myfile << 4 << "," << "beer" << "," << "eggs" <<"," << "flour" <<"," << "milk" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 4 << ","<< "coffee" << "," << "diaper" <<"," << "flour" <<"," << "tea" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 4 << ","<< "candy" << "," << "diaper" <<"," << "milk" <<"," << "rice" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 2 << ","<< "bread" << "," << "nuts" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 3 << ","<< "bread" << "," << "cereal"<< "," << "diaper" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 3 << ","<< "bread" << "," << "candy" <<"," << "cereal" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 2 << ","<< "beer" << "," << "cereal" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 2 << ","<< "bread" << "," << "milk" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 4 << ","<< "beer" << "," << "eggs" <<"," << "milk" <<"," << "nuts" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 3 << ","<< "bread" << "," << "coffee" <<"," << "diaper" <<","<< (rand()/(double)(RAND_MAX))<<"\n";
	myfile << 4 << ","<< "candy" << "," << "coffee"<< "," << "diaper" <<"," << "eggs" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 5 << ","<< "candy" << "," << "cereal" <<"," << "diaper" <<"," << "eggs" <<"," << "nuts" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 4 << ","<< "candy" << "," << "cereal" <<"," << "diaper" <<"," << "milk" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 3 << ","<< "candy" << "," << "diaper" <<"," << "tea" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 4 << ","<< "candy" << "," << "cereal" <<"," << "diaper" <<"," << "flour" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 4 << ","<< "candy" << "," << "cereal" <<"," << "diaper" <<","<<"rice" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";
	myfile << 4 << ","<< "coffee" << "," << "diaper" <<"," << "flour" <<"," << "milk" <<","<< (rand()/(double)(RAND_MAX)) <<"\n";

}
