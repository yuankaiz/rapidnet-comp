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

#ifndef __VARIABLE_GRAPH_H__
#define __VARIABLE_GRAPH_H__

#include <list>
#include <map>
#include <set>
#include <vector>

#include "all-values.h"
#include "ol-context.h"
#include "parser-util.h"
#include "table-store.h"

#include "ns3/log.h"
#include "ns3/ref-count-base.h"

using namespace std;
using namespace ns3;

namespace ns3 {
namespace rapidnet_compiler {

/**
 * \ingroup rapidnet_compiler
 *
 * \brief Represent a variable in a Overlog program
 */
class VNode;

typedef set<VNode *> VNodeSet;

struct Select {
  OlContext::Rule* rule;
  ParseSelect* select;
  Select (OlContext::Rule* r, ParseSelect* s) :
    rule(r), select(s) {}
};

struct Join {
  OlContext::Rule* rule;
  string varName;
  Join (OlContext::Rule* r, string v) :
    rule(r), varName(v) {}
};

class VNode
{
public:
  enum TypeCode
  {
  	EVENT = 0,
  	TUPLE,
  	ASSIGN,
  };

  VNode (TypeCode t) :
  	type(t)
  {
  	m_composition.insert(this);
  }

  VNode (TypeCode t, VNode* other) :
  	type(t), m_composition(other->m_composition) { }

  VNode (TypeCode t, VNodeSet resolved) :
  	type(t), m_composition(resolved) { }

  ~VNode();

  void AddSelect(OlContext::Rule* nextRule, ParseSelect* select);

  string PrintSelectList(string prefix);

  void AddJoin(OlContext::Rule* nextRule, string varName);

  string PrintJoinList(string prefix);

  string ToString ();

  bool IsCore();

  TypeCode type;

  VNodeSet::iterator GetBegin() {
  	return m_composition.begin();
  }

  VNodeSet::iterator GetEnd() {
  	return m_composition.end();
  }

private:
  VNodeSet m_composition;
  list<Select> m_selects;
  list<Join> m_joins;
};

/**
 * \ingroup rapidnet_compiler
 *
 * \brief Construct a dependency graph between variables
 */
class VGraph
{
public:

  VGraph () {}
  ~VGraph ();
  VNode* FindFunctorNode (ParseFunctor* functor, int position);
  void AddFunctorNode (ParseFunctor* functor, int position, VNode* node);
  void AddAssignNode (int ruleNum, string varName, VNode* node);
  // VNode* CreateAssignNode (int ruleNum, ParseAssign* assign);
private:
  map<string, VNode*> m_key2Node;
  vector<VNode*>  m_eventTupleNodeList;
  vector<VNode*>  m_assignNodeList;
  string GetFunctorKey(ParseFunctor* functor, int position);
  string GetAssignKey(int ruleNum, string varName);
};

} // namespace ns3
} // namespace rapidnet_compiler

#endif
