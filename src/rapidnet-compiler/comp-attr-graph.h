/*
 * comp-attr-graph.h
 *
 *  Created on: Feb 27, 2016
 *      Author: cchen
 */

#ifndef COMP_ATTR_GRAPH_H_
#define COMP_ATTR_GRAPH_H_

#include <map>
#include <utility>
#include <vector>
#include <list>
#include <algorithm>

#include "ol-context.h"
#include "parser-util.h"

#include "ns3/ptr.h"

using namespace ns3;
using namespace rapidnet_compiler;

typedef pair<string, int> VarID; // Identification of a variable
typedef map<VarID, int> VarNodeMap; // A map between variables and graph nodes


/*
 * Adjacency node for AttriGraph
 */
struct AttriNode{
	int nodeID;

	enum PredType {Base, Event, Assign, Intm};

	PredType nodeType;

	AttriNode* next;
};


/*
 * Adjacency list for AttriGraph
 */
struct AttriList{
	AttriNode* head;
};


/*
 * Attribute Graph
 *
 * Note: AttriGraph is not a multigraph
 */
class AttriGraph{
public:
	AttriGraph(Ptr<OlContext>, string);

	void BuildRuleGraph(OlContext::Rule*,
						ParseFunctor*,
						OlContext::RuleList*);

	bool ConnectEventVar(ParseVar*, int, AttriNode::PredType, ParseFunctor*);

	void ConnAssignHead(ParseExpr*, string, int, int&);

	void ProcExpr(ParseExpr*, string, int&,
						ParseFunctor*, map<string, int>&);

	bool ProcNonEv(ParseFunctor*, AttriNode::PredType,
				   ParseFunctor*, map<string, int>&);

	AttriNode* CreateNode(int, AttriNode::PredType);

	void AddEdge(int, AttriNode::PredType, int, AttriNode::PredType);

	void FindEquiClass();

	bool CouldReachBase(ParseFunctorName&, const int);

	bool IsBaseAttri(const AttriNode&);

	void PrintGraph();

	void DestroyRestList(AttriNode*);

	~AttriGraph();

private:
	// Default maximum number of nodes
	static int defaultSize;

	// Count the number of all variables
	int varCount;

	//The event tuple
	ParseFunctor* event;

	// Mapping between variables and graph nodes
	VarNodeMap vnMap;

	// Adjacency lists of nodes
	AttriList* nodeArray;

	// Register processed rules for loop detection
	OlContext::RuleList procsedRules;
};


#endif /* COMP_ATTR_GRAPH_H_ */
