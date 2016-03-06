/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Pennsylvania
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "prov-analyzer-context.h"
#include "errno.h"
#include <set>

using namespace ns3;
using namespace ns3::rapidnet_compiler;

NS_LOG_COMPONENT_DEFINE ("AnalyzerContext");

string AnalyzerContext::ToString ()
{
  ostringstream b, core;
  b << "In basic event " << m_basicEvent->ToString () << ":\n";
  // For each basic event attributes, output all its joins and selections.
  bool first = true;
  for (int i = 0; i < m_basicEvent->Args (); ++i)
    {
      string attrName = m_basicEvent->Arg (i)->ToString ();
      // Get the node based on its unique name and position
      VNode* attrNode = m_vGraph.FindFunctorNode (m_basicEvent, i);
      b << "  Attribute " << attrName << ": " << attrNode-> ToString() << "\n";
      // Print join list from the node
      b << attrNode->PrintJoinList ("    ");
      // Print selection list from the node
      b << attrNode->PrintSelectList ("    ");
      if (attrNode->IsCore ()) {
        if (first) {
          first = false;
        } else {
          core << ", ";
        }
        core << attrName;
      }
    }
  // Output core attributes
  b << "Static analysis identifies following core attributes: {"
    << core.str() << "}\n";
  NS_LOG_INFO("\n" << b.str());
  return b.str ();
}

bool AnalyzerContext::Analyze (Ptr<OlContext> ctxt, Ptr<TableStore> tableStore)
{
  // Assumes ctxt points to an Overlog program with following assumptions.
  // 1) Every rule's first unmaterialized functor (must exists) in the body is either
  //    a basic event (first rule) or a derived event that is declared in the previous rule.
  // 2) Every rule's other functors are treated as base tuples and only occurs once in the program.

  // Validating the input Overlog program
  string prevHeadName = "";
  for (int k = 0; k < ctxt->GetRules ()->size (); k++) {
    OlContext::Rule* nextRule = ctxt->GetRules ()->at (k);
    string headName = nextRule->head->fName->name;
    ParseFunctor* eventFunctor = ExtractEvent (nextRule, tableStore);
    if (eventFunctor == NULL) {
      NS_LOG_ERROR("ERROR: Not event found in this rule: " << nextRule);
      return false;
    }
    string eventName = eventFunctor->fName->name;
    // Validating eventName is equal to previous head name
    if (k == 0) {
      m_basicEvent = eventFunctor;
      // Add nodes for basic event attributes
      for (int i = 0; i < eventFunctor->Args (); ++i) {
        VNode* node = new VNode(VNode::EVENT);
        m_vGraph.AddFunctorNode(eventFunctor, i, node);
      }
    } else if (eventName != prevHeadName) {
      NS_LOG_ERROR("ERROR: Event not declared in previous rule: " << eventName);
      return false;
    }
    prevHeadName = headName;
  }
  NS_LOG_INFO("Pass validation!\n");

  // Analyze each rule in order and construct dependency graph
  for (int k = 0; k < ctxt->GetRules ()->size (); k++) {
    OlContext::Rule* nextRule = ctxt->GetRules ()->at (k);
    if (!AnalyzeRule (nextRule, tableStore)) {
      return false;
    }
  }

  return true;
}

ParseFunctor* AnalyzerContext::ExtractEvent (OlContext::Rule* nextRule,
  Ptr<TableStore> tableStore)
{
  NS_LOG_DEBUG ("Extract event from " << nextRule->ToString ());
  for (list<ParseTerm*>::iterator t = nextRule->terms.begin (); t
      != nextRule->terms.end (); t++) {
    ParseTerm* nextTerm = (*t);
    ParseFunctor *functor = dynamic_cast<ParseFunctor*> (nextTerm);
    if (functor != NULL) {
      // Is this materialized?
      OlContext::TableInfo* tableInfo = tableStore->GetTableInfo (
        functor->fName->name);
      if (tableInfo == NULL) {
        // This is an event, ignore following because left-deep assumption
        NS_LOG_DEBUG ("  Event Functor @ " << nextTerm->Position()
            << " : " << nextTerm->ToString ());
        return functor;
      }
    }
  }
  return NULL;
}

bool AnalyzerContext::AnalyzeRule (OlContext::Rule* nextRule,
  Ptr<TableStore> tableStore)
{
  NS_LOG_DEBUG ("Perform static analysis on @ " << nextRule->ruleNum
    << ": " <<nextRule->ToString ());

  vector<ParseFunctor*> tuples;
  vector<ParseAssign*> assigns;
  vector<ParseSelect*> selects;
  map<string, VNode*> var2Node;
  // Create event node and put terms in corresponding buckets
  bool hasEvent = false;
  for (list<ParseTerm*>::iterator t = nextRule->terms.begin (); t
      != nextRule->terms.end (); t++) {
    ParseTerm* nextTerm = (*t);
    ParseFunctor *functor = dynamic_cast<ParseFunctor*> (nextTerm);
    ParseAssign *assign = dynamic_cast<ParseAssign*> (nextTerm);
    ParseSelect *select = dynamic_cast<ParseSelect*> (nextTerm);
    if (functor != NULL) {
      // Is this materialized?
      OlContext::TableInfo* tableInfo = tableStore->GetTableInfo (
        functor->fName->name);
      if (tableInfo != NULL || hasEvent) {    // This is a tuple
        tuples.push_back(functor);
      } else {          // This is an event, process immediately
        hasEvent = true;
        NS_LOG_DEBUG ("  Event @ " << functor->Position()
            << " : " << functor->ToString ());
        for (int i = 0; i < functor->Args (); ++i) {
          ParseExpr* arg = functor->Arg (i);
          ParseVar* var = dynamic_cast<ParseVar*>(arg);
          if (var != NULL) {
            VNode* thisNode = m_vGraph.FindFunctorNode(functor, i);
            string varName = var->ToString ();
            map<string, VNode*>::iterator it = var2Node.find(varName);
            if (it == var2Node.end()) {   // First occurence
              NS_LOG_DEBUG ("    Add variable to map: " << varName << ", " << thisNode->ToString ());
              var2Node[varName] = thisNode;
            } else {      // Join with previous attribute
              NS_LOG_DEBUG ("    Join two nodes for variable: " << varName);
              VNode* otherNode = var2Node[varName];
              // Add join to these two basic/derived event nodes
              Join(thisNode, otherNode, nextRule, varName);
            }
          } else {
            NS_LOG_ERROR ("ERROR: The " << functor->Position() <<"-th term of rule '"
              << nextRule->ToString () << "' has non-variable attributes.");
            return false;
          }
        }
      }
    } else if (assign != NULL) {    // This is an assignment
      assigns.push_back(assign);
    } else if (select != NULL) {    // This is an selection
      selects.push_back(select);
    } else {
      // This is a term type that we didn't think of. Complain but move on
      NS_LOG_WARN ("WARNING: The " << nextTerm->Position () << "-th term of rule '"
          << nextRule->ToString () << "' has an unknown type. Ignoring.");
    }
  }

  // Process tuples
  for (vector<ParseFunctor*>::iterator t = tuples.begin();
      t!= tuples.end(); ++t) {
    ParseFunctor* tuple = (*t);
    for (int i = 0; i < tuple->Args (); ++i) {
      NS_LOG_DEBUG ("  Tuple @ " << tuple->Position() 
          << " : " << tuple->ToString ());
      ParseExpr* arg = tuple->Arg (i);
      ParseVar* var = dynamic_cast<ParseVar*>(arg);
      if (var != NULL) {
        // Add nodes for tuple attributes
        VNode* thisNode = new VNode(VNode::TUPLE);
        m_vGraph.AddFunctorNode(tuple, i, thisNode);
        string varName = var->ToString ();
        map<string, VNode*>::iterator it = var2Node.find(varName);
        if (it == var2Node.end()) {
          NS_LOG_DEBUG ("    Add variable to map: " << varName);
          var2Node[varName] = thisNode;
        } else {
          NS_LOG_DEBUG ("    Join two nodes for variable: " << varName);
          VNode* otherNode = it->second;
          // Add join to this tuple node and another basic/derived event/tuple node
          Join(thisNode, otherNode, nextRule, varName);
        }
      } else {
        NS_LOG_ERROR ("ERROR: The " << tuple->Position() <<"-th term of rule '"
          << nextRule->ToString () << "' has non-variable attributes.");
        return false;
      }
    }
  }

  // Temporary set for resolving variables in an expression
  VNodeSet resolved;
  // Process assigns
  for (vector<ParseAssign*>::iterator t = assigns.begin();
      t!= assigns.end(); ++t) {
    ParseAssign* assign = (*t);
    NS_LOG_DEBUG ("  Assignment @ " << assign->Position() 
        << " : " << assign->ToString ());
    ParseVar* var = assign->var;
    ParseExpr* expr = assign->assign;
    string varName = var->ToString ();
    map<string, VNode*>::iterator it = var2Node.find(varName);
    if (it == var2Node.end()) {
      resolved.clear();
      if (!Resolve(expr, var2Node, resolved)) {
        NS_LOG_ERROR ("ERROR: The " << assign->Position() <<"-th term of rule '"
          << nextRule->ToString () << "' cannot be resolved.");
        return false;
      }
      // Add node for assigned variabled
      VNode* node = new VNode(VNode::ASSIGN, resolved);
      m_vGraph.AddAssignNode(nextRule->ruleNum, varName, node);
      NS_LOG_DEBUG ("    Create new assign node: " << varName << ", " << node->ToString ());
      var2Node[varName] = node;
    } else {
      NS_LOG_ERROR ("ERROR: The " << assign->Position() <<"-th term of rule '"
        << nextRule->ToString () << "' is an overloaded assignment.");
    }
  }

  // Process selects
  for (vector<ParseSelect*>::iterator t = selects.begin();
      t!= selects.end(); ++t) {
    ParseSelect* select = (*t);
    ParseBool* boolExpr = select->select;
    // Add select to resolved basic event/tuple nodes
    resolved.clear();
    if (!Resolve(boolExpr, var2Node, resolved)) {
      NS_LOG_ERROR ("ERROR: The " << select->Position() <<"-th term of rule '"
        << nextRule->ToString () << "' cannot be resolved.");
      return false;
    }
    for (VNodeSet::iterator it = resolved.begin();
        it != resolved.end(); ++it) {
      VNode* basic = *it;
      basic->AddSelect(nextRule, select);
    }
    NS_LOG_DEBUG ("  Selection @ " << select->Position() 
        << " : " << boolExpr->ToString () <<" , resolved " << resolved.size());
  }

  // Process head, assume every argument is either an attribute in an event/tuple,
  // or assigned in an assignment.
  ParseFunctor* head = nextRule->head;
  NS_LOG_DEBUG ("  Head @ 0 : " << head->ToString ());
  for (int i = 0; i < head->Args (); ++i) {
    ParseExpr* arg = head->Arg (i);
    ParseVar* var = dynamic_cast<ParseVar*>(arg);
    if (var != NULL) {
      string varName = var->ToString ();
      map<string, VNode*>::iterator it = var2Node.find(varName);
      if (it != var2Node.end()) {
        // Add node for derived event attributes
        VNode* otherNode = it->second;
        VNode* thisNode = new VNode(VNode::ASSIGN, otherNode);
        m_vGraph.AddFunctorNode(head, i, thisNode);
        NS_LOG_DEBUG ("    Create new assign node: " << varName << ", " << thisNode->ToString ());
      } else {
        NS_LOG_ERROR ("ERROR: The head of rule '"
          << nextRule->ToString () << "' has undefined variables.");
        return false;
      }
    } else {
      NS_LOG_ERROR ("ERROR: The head of rule '"
        << nextRule->ToString () << "' has non-variable attributes.");
      return false;
    }
  }

  return true;
}

bool AnalyzerContext::Resolve (ParseExpr* expr,
    map<string, VNode*>& var2Node, VNodeSet& resolved)
{
  ParseVar* var = dynamic_cast<ParseVar*> (expr);
  ParseVal* val = dynamic_cast<ParseVal*> (expr);
  ParseBool* boolExpr = dynamic_cast<ParseBool*> (expr);
  ParseMath* mathExpr = dynamic_cast<ParseMath*> (expr);
  ParseFunction* func = dynamic_cast<ParseFunction*> (expr);
  if (var != NULL) {    // A variable
    string varName = var->ToString ();
    map<string, VNode*>::iterator it = var2Node.find(varName);
    if (it != var2Node.end()) {
      VNode* other = var2Node[varName];
      resolved.insert(other->GetBegin(), other->GetEnd());
      // NS_LOG_DEBUG("    Insert to resolved: " << expr->ToString ());
      return true;
    } else {
      NS_LOG_ERROR("ERROR: Undefined variable " << expr->ToString ());
      return false;
    }
  } else if (val != NULL) {   // A value
    return true;
  } else if (boolExpr != NULL) {  // A boolExpr
    return Resolve(boolExpr->lhs, var2Node, resolved) &&
      Resolve(boolExpr->rhs, var2Node, resolved);
  } else if (mathExpr != NULL) {  // A mathExpr
    return Resolve(mathExpr->lhs, var2Node, resolved) &&
      Resolve(mathExpr->rhs, var2Node, resolved);
  } else if (func != NULL) {      // A function
    for (int i = 0; i < func->Args (); ++i) {
      if (!Resolve(func->Arg (i), var2Node, resolved)) {
        return false;
      }
    }
    return true;
  } else {
    NS_LOG_ERROR("ERROR: Not supported type " << expr->ToString ());
    return false;
  }
}


void AnalyzerContext::Join (VNode* a, VNode* b, OlContext::Rule* rule, string varName)
{
  VNodeSet merged (a->GetBegin(), a->GetEnd());
  merged.insert(b->GetBegin(), b->GetEnd());
  for (VNodeSet::iterator it = merged.begin();
      it != merged.end(); ++it) {
    VNode* node = (*it);
    node->AddJoin(rule, varName);
  }
}