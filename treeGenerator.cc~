#include<iostream>
#include<list>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sstream>

using namespace std;

void generateTree(int *nextNodeID, string currentPath, list<string> *pathList, int level,\
			int minFanout, int maxFanout)
{
	if(level > 0)
	{
		int fanout = rand()%(maxFanout-minFanout)+minFanout;
		cout<<"FANOUT "<<fanout<<endl;
		int i=0;
		int rootID = *nextNodeID;

		for(i=0;i<fanout;i++)
		{
			(*nextNodeID)++;
			if(level-1 > 0)
			{
				cout<<"Adding at "<<rootID<<" "<<"."<<*nextNodeID<<currentPath<<endl;
				stringstream newPath;
				newPath<<"."<<*nextNodeID<<currentPath;	
				generateTree(nextNodeID,newPath.str(),pathList,level-1,minFanout,maxFanout);
			}
			else
			{ 
				cout<<"Adding at "<<rootID<<" "<<*nextNodeID<<currentPath<<endl;
				stringstream newPath;
				newPath<<*nextNodeID<<currentPath;	
				generateTree(nextNodeID,newPath.str(),pathList,level-1,minFanout,maxFanout);
			}	
		}
	}
	else
	{
		(*pathList).push_back(currentPath);
	}
}

int main(int argc, char* argv[])
{

	srand(time(NULL));

	if(argc == 5)
	{
		int nextNodeID = 2;
		string path = ".";
		list<string> pathList;

		int minDepth = atoi(argv[1]);
		int maxDepth = atoi(argv[2]);
		
		int level = rand()%(maxDepth-minDepth)+minDepth;
		cout<<"HEIGHT "<<level<<endl;
		int minFanout = atoi(argv[3]);
		int maxFanout = atoi(argv[4]);
		generateTree(&nextNodeID,path,&pathList,level,minFanout,maxFanout);
		list<string>::iterator iter = pathList.begin();
		for(;iter!=pathList.end();iter++)
			cout<<"URL "<<*iter<<endl;
	}
	else
	{
		cout<<"USAGE : treeGenerator minDepth maxDepth minFanout maxFanout"<<endl;
	}
	return 0;
}
