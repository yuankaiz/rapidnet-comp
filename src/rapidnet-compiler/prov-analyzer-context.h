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

#ifndef __ANALYZER_CONTEXT_H__
#define __ANALYZER_CONTEXT_H__

#include <vector>
#include <map>
#include <set>
#include "all-values.h"
#include "ol-context.h"
#include "parser-util.h"
#include "prov-variable-graph.h"
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
 * \brief Analyze the parsed NDlog protocol in OlContext to
 *        figure out the core attributes.
 *
 * @see OLContext
 */
class AnalyzerContext : public RefCountBase
{
public:

  AnalyzerContext () { }

  bool Analyze (Ptr<OlContext> ctxt, Ptr<TableStore> tableStore);
  string ToString ();
private:
  bool AnalyzeRule (OlContext::Rule* rule, Ptr<TableStore> tableStore);
  bool Resolve (ParseExpr* expr, map<string, VNode*>& var2Node, VNodeSet& resolved);
  void Join (VNode* a, VNode* b, OlContext::Rule* rule, string varName);
  ParseFunctor* ExtractEvent  (OlContext::Rule* rule, Ptr<TableStore> tableStore);
  ParseFunctor* m_basicEvent;
  VGraph m_vGraph;
};

} // namespace ns3
} // namespace rapidnet_compiler

#endif
