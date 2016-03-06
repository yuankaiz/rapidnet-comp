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

#include "prov-variable-graph.h"
#include "errno.h"

using namespace ns3;
using namespace ns3::rapidnet_compiler;

NS_LOG_COMPONENT_DEFINE ("VariableGraph");

VNode::~VNode ()
{
}

string VNode::ToString ()
{
  ostringstream oss;
  oss << "type (" << type << "), composition (" << m_composition.size()
      << "), selects (" << m_selects.size()
      << "), joins(" << m_joins.size() << ")";
  return oss.str();
}

string VNode::PrintSelectList(string prefix)
{
  ostringstream oss;
  for (list<Select>::iterator it = m_selects.begin();
      it != m_selects.end(); ++it) {
    oss << prefix << "Involved in " << it->select->Position ()
        << "-th term of rule '" << it->rule->ToString () <<"'.\n";
  }
  return oss.str();
}

string VNode::PrintJoinList(string prefix)
{
  ostringstream oss;
  for (list<Join>::iterator it = m_joins.begin();
      it != m_joins.end(); ++it) {
    oss << prefix << "Involved in joins of variable " << it->varName
        << " in rule '" << it->rule->ToString () <<"'.\n";
  }
  return oss.str();
}

void VNode::AddSelect(OlContext::Rule* nextRule, ParseSelect* select)
{
  Select s (nextRule, select);
  m_selects.push_back(s);
}

void VNode::AddJoin(OlContext::Rule* nextRule, string varName)
{
  Join j (nextRule, varName);
  m_joins.push_back(j);
}

bool VNode::IsCore()
{
  return (type == VNode::EVENT && (!m_selects.empty() || !m_joins.empty()));
}

VGraph::~VGraph()
{
  for (vector<VNode*>::iterator t = m_eventTupleNodeList.begin();
      t!= m_eventTupleNodeList.end(); ++t) {
    delete (*t);
  }
  for (vector<VNode*>::iterator t = m_assignNodeList.begin();
      t!= m_assignNodeList.end(); ++t) {
    delete (*t);
  }
}

string VGraph::GetFunctorKey(ParseFunctor* functor, int position)
{
  ostringstream oss;
  oss << functor->fName->ToString () << "-" << position;
  return oss.str();
}

VNode* VGraph::FindFunctorNode(ParseFunctor* functor, int position)
{
  string key = GetFunctorKey(functor, position);
  map<string, VNode*>::iterator t = m_key2Node.find(key);
  if (t == m_key2Node.end()) {
    return NULL;
  } else {
    return t->second;
  }
}

void VGraph::AddFunctorNode(ParseFunctor* functor, int position, VNode* node)
{
  if (node->type == VNode::EVENT || node->type == VNode::TUPLE) {
    m_eventTupleNodeList.push_back(node);
  } else if (node->type == VNode::ASSIGN) {
    m_assignNodeList.push_back(node);
  } else {
    NS_LOG_WARN("WARNING: Unknown VNode type");
    return;
  }
  string key = GetFunctorKey(functor, position);
  m_key2Node[key] = node;
}

string VGraph::GetAssignKey(int ruleNum, string varName)
{
  ostringstream oss;
  oss << ruleNum << "-" << varName;
  return oss.str();
}

void VGraph::AddAssignNode (int ruleNum, string varName, VNode* node)
{
  m_assignNodeList.push_back(node);
  string key = GetAssignKey(ruleNum, varName);
  m_key2Node[key] = node;
}
