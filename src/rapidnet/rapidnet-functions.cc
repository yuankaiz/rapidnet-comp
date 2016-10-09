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

#include "rapidnet-functions.h"
#include <cstdlib>
#include "ns3/ref-count-base.h"
#include "ns3/simulator.h"
#include "ns3/rapidnet-types.h"
#include "rapidnet-application-base.h"
#include "rapidnet-utils.h"
#include "expression.h"
#include <iostream>
#include <fstream>
#include "ns3/log.h"
#include "ns3/byte-array-value.h"
#include "ns3/nil-value.h"
#include <stdlib.h>

using namespace ns3;
using namespace rapidnet;

std::string
PrintAsHex (char* arr, ssize_t bytes);

/* ************************************************************** */

Ptr<Value>
FAppend::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > result;
  result.push_back (m_source->Eval (tuple));
  return ListValue::New (result);
}

Ptr<FunctionExpr>
FAppend::New (Ptr<Expression> source, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FAppend> retval = Create<FAppend> ();
  retval->m_source = source;
  return retval;
}

/* ************************************************************** */



Ptr<Value>
FHashIP::Eval (Ptr<Tuple> tuple)
{
  string s = m_ipaddr->Eval(tuple)->ToString ();
  int32_t sum = 0;
  for (int i=0; i<s.size(); i++) {
    sum += s[i];
  }
  //uint32_t ipaddr = (rn_ipv4 (m_ipaddr->Eval (tuple))).Get ();
  return Int32Value::New(sum);
}


Ptr<FunctionExpr>
FHashIP::New (Ptr<Expression> ipaddr)
{
  Ptr<FHashIP> retval = Create<FHashIP> ();
  retval->m_ipaddr = ipaddr;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FModulo::Eval (Ptr<Tuple> tuple)
{
  return Operation::New (RN_MODULUS, m_divident, m_divisor)->Eval (tuple);
}

Ptr<FunctionExpr>
FModulo::New (Ptr<Expression> divident, Ptr<Expression> divisor)
{
  Ptr<FModulo> retval = Create<FModulo> ();
  retval->m_divident = divident;
  retval->m_divisor = divisor;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FEmpty::Eval (Ptr<Tuple> tuple)
{
  return ListValue::New ();
}

Ptr<FunctionExpr>
FEmpty::New (Ptr<RapidNetApplicationBase> app)
{
  return Create<FEmpty> ();
}

/* ************************************************************** */

Ptr<Value>
FConcat::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > result;
  Ptr<ListValue> headVal = DynamicCast<ListValue, Value> (
    m_head->Eval (tuple));
  Ptr<ListValue> tailVal = DynamicCast<ListValue, Value> (
    m_tail->Eval (tuple));
  for (rn_list_iterator it = headVal->Begin(); it != headVal->End (); ++it)
  {
    result.push_back (*it);
  }
  for (rn_list_iterator it = tailVal->Begin(); it != tailVal->End (); ++it)
  {
    result.push_back (*it);
  }
  return ListValue::New (result);
}

Ptr<FunctionExpr>
FConcat::New (Ptr<Expression> head, Ptr<Expression> tail, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FConcat> retval = Create<FConcat> ();
  retval->m_head = head;
  retval->m_tail = tail;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FItem::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > lst = rn_list (m_lst->Eval (tuple));
  uint32_t index = rn_int32 (m_index->Eval (tuple));

  rn_list_iterator it = lst.begin ();

  if (index > lst.size () || index < 1)
  {
    return NilValue::New ();
  }

  for (uint32_t i = 0; i < index - 1; i++)
  {
    it++;
  }

  return (*it)->Clone ();
}

Ptr<FunctionExpr>
FItem::New (Ptr<Expression> lst, Ptr<Expression> index, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FItem> retval = Create<FItem> ();
  retval->m_lst = lst;
  retval->m_index = index;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FMember::Eval (Ptr<Tuple> tuple)
{
  Ptr<ListValue> lstVal = DynamicCast<ListValue, Value> (m_lst->Eval (tuple));
  // Seems P2 does not support "true" and "false" as keywords. So use int32 for now
  return Int32Value::New (lstVal->Contains (m_item->Eval (tuple)));
}

Ptr<FunctionExpr>
FMember::New (Ptr<Expression> lst, Ptr<Expression> item, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FMember> retval = Create<FMember> ();
  retval->m_lst = lst;
  retval->m_item = item;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FNow::Eval (Ptr<Tuple> tuple)
{
	return RealValue::New (Simulator::Now ().GetSeconds ());
}

Ptr<FunctionExpr>
FNow::New (Ptr<RapidNetApplicationBase> app)
{
  return Create<FNow> ();
}

/* ************************************************************** */

Ptr<Value>
FDiffTime::Eval (Ptr<Tuple> tuple)
{
  return Operation::New (RN_MINUS, m_time2, m_time1)->Eval (tuple);
}

Ptr<FunctionExpr>
FDiffTime::New (Ptr<Expression> time2, Ptr<Expression> time1, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FDiffTime> retval = Create<FDiffTime> ();
  retval->m_time2 = time2;
  retval->m_time1 = time1;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FHslsTtl::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> now = FNow::New ()->Eval (tuple);
  Ptr<Value> normTimeDiff = Operation::New (RN_PLUS,
    Operation::New (RN_DIVIDE, ValueExpr::New (now), m_periodAttrName),
    ValueExpr::New (RealValue::New (1.0)))->Eval (tuple);
  int32_t n = int32_t (rn_real (normTimeDiff));
  int32_t ttl = n % 2 == 1 ? 2 : (n % 4 == 2 ? 4 : (n % 8 == 4 ? 8 : 16));
  return Int32Value::New (ttl);
}

Ptr<FunctionExpr>
FHslsTtl::New (Ptr<Expression> timeAttrName, Ptr<Expression> periodAttrName, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FHslsTtl> retval = Create<FHslsTtl> ();
  retval->m_timeAttrName = timeAttrName;
  retval->m_periodAttrName = periodAttrName;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FSize::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
    "f_size can only be applied to a list type attribute.");

  return Int32Value::New ((DynamicCast<ListValue, Value> (lstVal))->Size ());
}

Ptr<FunctionExpr>
FSize::New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FSize> retval = Create<FSize> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FLast::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
    "f_last can only be applied to a list type attribute.");

  list<Ptr<Value> > lst = rn_list (lstVal);

  if (lst.end () == lst.begin ())
  {
    return Ptr<Value> (NULL);
  }

  rn_list_iterator it = lst.end ();
  return (*--it)->Clone ();
}

Ptr<FunctionExpr>
FLast::New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FLast> retval = Create<FLast> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FRemoveLast::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
    "f_removeLast can only be applied to a list type attribute.");

  list<Ptr<Value> > lst = rn_list (lstVal);

  if (lst.end () == lst.begin ())
  {
    return Ptr<Value> (NULL);
  }

  rn_list_iterator it = lst.end ();
  --it; // move one step back
  list<Ptr<Value> > newlst;
  for (rn_list_iterator jt = lst.begin (); jt != it; jt++)
  {
    newlst.push_back (*jt);
  }
  return ListValue::New (newlst);
}

Ptr<FunctionExpr>
FRemoveLast::New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FRemoveLast> retval = Create<FRemoveLast> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}



Ptr<Value>
FFirst::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
    "f_first can only be applied to a list type attribute.");

  list<Ptr<Value> > lst = rn_list (lstVal);

  if (lst.end () == lst.begin ())
  {
    return Ptr<Value> (NULL);
  }

  rn_list_iterator it = lst.begin ();
  //clog<<"f_first " << (*it)->Clone() <<"\n";
  return (*it)->Clone ();
}

Ptr<FunctionExpr>
FFirst::New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FFirst> retval = Create<FFirst> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}

Ptr<Value>
FRemoveFirst::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
    "f_removeFirst can only be applied to a list type attribute.");
  
  list<Ptr<Value> > lst = rn_list (lstVal);

  if (lst.end () == lst.begin ())
  {
   clog<<lstVal;
   return Ptr<Value> (NULL);

 }

 rn_list_iterator it = lst.begin ();
 rn_list_iterator it2 = lst.end ();
 ++it;

  list<Ptr<Value> > newlst;
  for (rn_list_iterator jt = it; jt != it2; jt++)
  {
    newlst.push_back (*jt);
  }

 Ptr<Value> newlist_ptr= ListValue::New(newlst);
 //clog<<"new list " << newlist_ptr <<"\n";
 return newlist_ptr;
}

Ptr<FunctionExpr>
FRemoveFirst::New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FRemoveFirst> retval = Create<FRemoveFirst> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}


/* ************************************************************** */

Ptr<Value>
FTypeOf::Eval (Ptr<Tuple> tuple)
{
  return StrValue::New (m_arg->Eval (tuple)->GetTypeName ());
}

Ptr<FunctionExpr>
FTypeOf::New (Ptr<Expression> arg, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FTypeOf> retval = Create<FTypeOf> ();
  retval->m_arg = arg;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FRand::Eval (Ptr<Tuple> tuple)
{
  stringstream ss;
  ss << rand ();
  return StrValue::New (ss.str ());
}

Ptr<FunctionExpr>
FRand::New (Ptr<RapidNetApplicationBase> app)
{
  return Create<FRand> ();
}

/* ************************************************************** */

Ptr<Value>
FSha1::Eval (Ptr<Tuple> tuple)
{
  string str = m_arg->Eval (tuple)->ToString ();
  return IdValue::New (GetSHA1Digest (str), 16);
}

Ptr<FunctionExpr>
FSha1::New (Ptr<Expression> arg, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FSha1> retval = Create<FSha1> ();
  retval->m_arg = arg;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FSvCreate::Eval (Ptr<Tuple> tuple)
{
  return SvValue::New();
}

Ptr<FunctionExpr>
FSvCreate::New (Ptr<RapidNetApplicationBase> app)
{
  Ptr<FSvCreate> retval = Create<FSvCreate> ();
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FSvIn::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> svVPtr = m_svExpr->Eval (tuple);
  Ptr<Value> strVPtr = m_strExpr->Eval (tuple);

  int32_t result = SvValue::HashIn(svVPtr, strVPtr) ? 1 : 0;

  return Int32Value::New(result);
}

Ptr<FunctionExpr>
FSvIn::New (Ptr<Expression> svExpr,
  Ptr<Expression> strExpr,
  Ptr<RapidNetApplicationBase> app)
{
  Ptr<FSvIn> retval = Create<FSvIn>();
  retval->m_svExpr = svExpr;
  retval->m_strExpr = strExpr;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FSvAndNot::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> svVPtr_1 = m_svExpr_1->Eval (tuple);
  Ptr<Value> svVPtr_2 = m_svExpr_2->Eval (tuple);

  return SvValue::AndNot(svVPtr_1, svVPtr_2);
}

Ptr<FunctionExpr>
FSvAndNot::New (Ptr<Expression> svExpr_1,
  Ptr<Expression> svExpr_2,
  Ptr<RapidNetApplicationBase> app)
{
  Ptr<FSvAndNot> retval = Create<FSvAndNot>();
  retval->m_svExpr_1 = svExpr_1;
  retval->m_svExpr_2 = svExpr_2;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FSvAppend::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> svVPtr = m_svExpr->Eval (tuple);
  Ptr<Value> strVPtr = m_strExpr->Eval (tuple);

  return  SvValue::HashInsert(svVPtr, strVPtr);
}

Ptr<FunctionExpr>
FSvAppend::New (Ptr<Expression> svExpr,
  Ptr<Expression> strExpr,
  Ptr<RapidNetApplicationBase> app)
{
  Ptr<FSvAppend> retval = Create<FSvAppend>();
  retval->m_svExpr = svExpr;
  retval->m_strExpr = strExpr;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FSvRemove::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> svVPtr = m_svExpr->Eval (tuple);
  Ptr<Value> strVPtr = m_strExpr->Eval (tuple);

  return SvValue::HashRemove(svVPtr, strVPtr);
}

Ptr<FunctionExpr>
FSvRemove::New (Ptr<Expression> svExpr,
  Ptr<Expression> strExpr,
  Ptr<RapidNetApplicationBase> app)
{
  Ptr<FSvRemove> retval = Create<FSvRemove>();
  retval->m_svExpr = svExpr;
  retval->m_strExpr = strExpr;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FPEdb::Eval(Ptr<Tuple> tuple)
{
  string prov = m_prov->Eval (tuple)-> ToString();

  return StrValue::New (prov);
}

Ptr<FunctionExpr>
FPEdb::New (Ptr<Expression> prov, Ptr<Expression> id, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FPEdb> retval = Create<FPEdb>();
  retval->m_prov = prov;
  retval->m_id = id;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FPIdb::Eval(Ptr<Tuple> tuple)
{
  list<Ptr<Value> > provList = rn_list (m_provList->Eval (tuple));

  stringstream ss;

  if (provList.size() != 1)
  {
   ss << "(";
 }

 int index = 0;

 for (rn_list_iterator it = provList.begin (); it != provList.end (); it++)
 {
  if (index++!=0) ss << "+";
  ss << (*it)->ToString ();
}

//  string loc = m_loc->Eval (tuple)->ToString ();
//  ss << ")@[" << loc << "]";

if (provList.size() != 1) 
{
 ss << ")";
}

return StrValue::New (ss.str ());
}

Ptr<FunctionExpr>
FPIdb::New (Ptr<Expression> provList, Ptr<Expression> loc, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FPIdb> retval = Create<FPIdb>();
  retval->m_provList = provList;
  retval->m_loc = loc;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FPRule::Eval(Ptr<Tuple> tuple)
{
  list<Ptr<Value> > provList = rn_list (m_provList->Eval (tuple));

  stringstream ss;

  string rule = m_rule->Eval (tuple)->ToString ();

  uint32_t ipaddr = (rn_ipv4 (m_rloc->Eval (tuple))).Get ();
  ipaddr = (ipaddr / 256) % 65536;
  ss << rule << "@n" << ipaddr << "(";

    int index = 0;

    for (rn_list_iterator it = provList.begin (); it != provList.end (); it++)
    {
      if (index++!=0) ss << "*";
      ss << (*it)->ToString ();
    }

    ss << ")";

return StrValue::New (ss.str ());
}

Ptr<FunctionExpr>
FPRule::New (Ptr<Expression> provList, Ptr<Expression> rloc, Ptr<Expression> rule, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FPRule> retval = Create<FPRule>();
  retval->m_provList = provList;
  retval->m_rloc = rloc;
  retval->m_rule = rule;

  return retval;
  }


/* ************************************************************** */

Ptr<Value>
FSign::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> list1 = m_list->Eval(tuple);
  Ptr<Value> keya = m_privateKey->Eval(tuple);

  Ptr<StrValue> key = DynamicCast<StrValue, Value> (keya);
  string keyString = key->ToString();

  //clog<<"fsign eval start"<<endl;

  FILE *stream;
  stream = fopen(keyString.c_str(), "r");

  //clog<<"secret key file: "<<keyString<<endl;

  if (stream==NULL)
  {
    clog<<"error opening file \n";
  }

  EVP_PKEY* privateKey= PEM_read_PrivateKey (stream, NULL, NULL, NULL);
  fclose(stream);
  if (privateKey == NULL)
  {
    clog << "Error loading key from key file \n";
    return Create<NilValue>();
  }


  //clog<<"list1 : " <<list1 <<"\n";
  Ptr<ListValue> buf1 = DynamicCast<ListValue, Value> (list1);
  uint32_t numBytes = buf1->GetSerializedSize ();
  Buffer buffer;
  buffer.AddAtStart (numBytes);
  Buffer::Iterator it = buffer.Begin ();
  buf1->Serialize (it);
  uint8_t *arr = const_cast<uint8_t*> (buffer.PeekData());


  EVP_MD_CTX md_ctx;
  EVP_MD_CTX_init (&md_ctx);
  EVP_SignInit (&md_ctx, EVP_sha1 ());
  EVP_SignUpdate (&md_ctx, arr, numBytes);

  //clog << "key: " << privateKey <<'\n';
  uint8_t* signature = new uint8_t[EVP_PKEY_size (privateKey)];
  uint32_t sigLen;

  int err = EVP_SignFinal (&md_ctx, signature, &sigLen, privateKey);

  Ptr<Value> retval = ByteArrayValue::New (signature, sigLen);

  EVP_MD_CTX_cleanup (&md_ctx);

  if (err != 1)
  {
    clog<<"error in sign\n";
    return Create<NilValue> ();
  }
  else
  {
    //clog<<"no error in sign\n";
    return retval;
  }  
}

Ptr<FunctionExpr>
FSign::New (Ptr<Expression> list1, Ptr<Expression> key1,Ptr<RapidNetApplicationBase> app)
{
  Ptr<FSign> retval = Create<FSign> ();
  retval->m_list = list1;
  retval->m_privateKey = key1;
  return retval;
}

Ptr<Value>
FVerify::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> signature1 = m_signature->Eval(tuple);
  Ptr<Value> list1 = m_list->Eval(tuple);
  Ptr<Value> keya = m_publicKey->Eval(tuple);

  Ptr<StrValue> key = DynamicCast<StrValue, Value> (keya);
  string keyString = key->ToString();

  //clog<<" keystring " << keyString <<"\n";
  
  FILE *stream;
  stream = fopen (keyString.c_str(), "r");
  if (stream==NULL)
  {
    clog<<"error opening file \n";
  }

  EVP_PKEY* publicKey= PEM_read_PUBKEY (stream, NULL, NULL, NULL);
  fclose(stream);
  if (publicKey == NULL)
  {
    clog << "Error loading key from key file \n";
  }

  //clog<<"list 1 " <<list1 <<"\n";
  Ptr<ListValue> buf1 = DynamicCast<ListValue, Value> (list1);
  uint32_t numBytes = buf1->GetSerializedSize ();
  Buffer buffer;
  buffer.AddAtStart (numBytes);
  Buffer::Iterator it = buffer.Begin ();
  buf1->Serialize (it);
  uint8_t *arr = const_cast<uint8_t*> (buffer.PeekData());

  Ptr<ByteArrayValue> buf2 = DynamicCast<ByteArrayValue, Value> (signature1);

  EVP_MD_CTX md_ctx;
  EVP_MD_CTX_init (&md_ctx);  
  EVP_VerifyInit (&md_ctx, EVP_sha1 ());
  EVP_VerifyUpdate (&md_ctx, arr, numBytes);
  
  //clog<<"buffer 2 : " << buf2->ToString() <<"\n";

  int err = EVP_VerifyFinal (&md_ctx, buf2->GetByteArrayPtr(),
  buf2->GetByteArrayLen(), publicKey);
  EVP_MD_CTX_cleanup (&md_ctx);
  if(err==1)
  {
    //clog<<"verified\n";
    return Int32Value::New (1);
  }
  else
  {
    //clog<<"not verified\n";
    return Int32Value::New (0);
  }  
}


Ptr<FunctionExpr>
FVerify::New (Ptr<Expression> list1, Ptr <Expression> signature, Ptr<Expression> key1, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FVerify> retval = Create<FVerify> ();
  retval->m_signature = signature;
  retval->m_list = list1;
  retval->m_publicKey = key1;
  //retval->m_node = node;
  //retval->m_node1 = node1;
  return retval;
}

Ptr<Value> 
FVerifyMac::Eval (Ptr<Tuple> tuple)
{
  //Regenerating the mac
  cout<<"fmac veri start"<<endl;
  Ptr<Value> msg = m_message->Eval(tuple);
  Ptr<Value> keya = m_secretKey->Eval(tuple);

  Ptr<StrValue> key = DynamicCast<StrValue, Value> (keya);
  string keyFile = key->ToString();
  
  FILE *fp = fopen (keyFile.c_str(), "r");
  
  clog<<"secret key file: "<<keyFile<<endl;

  if (fp==NULL)
    {
      clog<<"error opening file \n";
      return Create<NilValue>();
    }

  EVP_PKEY* secretKey= PEM_read_PrivateKey (fp, NULL, NULL, NULL);
  fclose(fp);
  if (secretKey == NULL)
  {
    clog << "Error loading key from key file: "<<keyFile<<endl;
    return Create<NilValue>();
  }
  

  cout << "key: "<<secretKey << endl;
  Ptr<ListValue> buf1 = DynamicCast<ListValue, Value> (msg);
  
  cout << "buf1: "<< buf1->GetTypeName()<<":"<<buf1->ToString()<<endl; 

  uint32_t numBytes = buf1->GetSerializedSize ();
  cout <<"buffer of size: "<<numBytes<<endl;

  Buffer buffer;
  buffer.AddAtStart (numBytes);
  Buffer::Iterator it = buffer.Begin ();
  buf1->Serialize (it);
  uint8_t *arr = const_cast<uint8_t*> (buffer.PeekData());
  
  cout<<"fmac eval Serialize: "<<PrintAsHex((char*)arr,numBytes)<<endl;

  HMAC_CTX ctx;
  HMAC_CTX_init (&ctx);
  //HMAC_Init_ex (&ctx, secretKey, EVP_PKEY_size (secretKey), EVP_sha1 (), NULL);
  char hmac[] = "Password";
  HMAC_Init_ex (&ctx, hmac, strlen((const char*)hmac), EVP_sha1 (), NULL);
  HMAC_Update (&ctx, arr, numBytes);

  //cout <<"fmac eval done hmac"<<endl;

  uint8_t* signature = new uint8_t[EVP_MAX_MD_SIZE];
  uint32_t sigLen;

  HMAC_Final (&ctx, signature, &sigLen);
  Ptr<Value> retval = ByteArrayValue::New (signature, sigLen);

  HMAC_cleanup (&ctx);
  cout<<"fmac eval result: "<< PrintAsHex((char*)signature,sigLen)<<endl;

  //Mac comparison
  Ptr<ByteArrayValue> mac1 = DynamicCast<ByteArrayValue,Value>(retval);
  Ptr<ByteArrayValue> mac = DynamicCast<ByteArrayValue,Value>(m_mac->Eval(tuple));
  //cout<<"fverifymac"<<endl;
  //cout <<"mac : "<< PrintAsHex((char*)mac->GetByteArrayPtr(),mac->GetByteArrayLen()) <<endl;
  //cout <<"mac1: "<< PrintAsHex((char*)mac1->GetByteArrayPtr(),mac1->GetByteArrayLen()) <<endl;

  if (mac->Equals(mac1)){
    //cout<<"mac verified"<<endl;
    return Create<Int32Value>(1);
  }
  else {
    //cout<<"mac not verified"<<endl;
    return Create<Int32Value>(0);
  }
}

Ptr<FunctionExpr>
FVerifyMac::New(Ptr<Expression> msg, Ptr<Expression> mac, Ptr<Expression> skey, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FVerifyMac> retval = Create<FVerifyMac>();
  retval->m_message = msg;
  retval->m_mac = mac;
  retval->m_secretKey = skey;
  return retval;
}


Ptr<Value>
FPrepend::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > result;
  Ptr<ListValue> listvalue = DynamicCast<ListValue, Value> (
    m_listvalue->Eval (tuple));
  result.push_back (m_value->Eval (tuple));

  for (rn_list_iterator it = listvalue->Begin(); it != listvalue->End (); ++it)
  {
    result.push_back (*it);
  }

  return ListValue::New (result);
}

Ptr<FunctionExpr>
FPrepend::New (Ptr<Expression> value1, Ptr<Expression> listvalue1, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FPrepend> retval = Create<FPrepend> ();
  retval->m_value = value1;
  retval->m_listvalue = listvalue1;
  return retval;
}

Ptr<Value>
FAppend2::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > result;
 // result.push_back (m_source->Eval (tuple));
  Ptr<ListValue> listvalue1 = DynamicCast<ListValue, Value> (
    m_listvalue->Eval (tuple));
  for (rn_list_iterator it = listvalue1->Begin(); it != listvalue1->End (); ++it)
  {
    result.push_back (*it);
  }
  result.push_back(m_source->Eval(tuple));
  return ListValue::New (result);
}

Ptr<FunctionExpr>
FAppend2::New (Ptr<Expression> source, Ptr<Expression> list, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FAppend2> retval = Create<FAppend2> ();
  retval->m_source = source;
  retval->m_listvalue = list;
  return retval;
}


std::string
PrintAsHex (char* arr, ssize_t bytes)
{
  // Bytes should be multiples of 4
  std::ostringstream sb;
  char str[33];
  for (unsigned i = 0; i < bytes/sizeof(uint32_t); i++)
  {
    sprintf(str, "%08x", ((uint32_t*) arr)[i]);
    sb << str;
  }
  return sb.str();
}

/* Jul 11, 2012
 * Cheng Luo 
*/

Ptr<Value>
FMAC::Eval(Ptr<Tuple> tuple)
{
  cout<<"fmac eval start"<<endl;
  Ptr<Value> msg = m_message->Eval(tuple);
  Ptr<Value> keya = m_secretKey->Eval(tuple);

  Ptr<StrValue> key = DynamicCast<StrValue, Value> (keya);
  string keyFile = key->ToString();
  
  FILE *fp = fopen (keyFile.c_str(), "r");
  
  clog<<"secret key file: "<<keyFile<<endl;

  if (fp==NULL)
    {
      clog<<"error opening file \n";
      return Create<NilValue>();
    }

  EVP_PKEY* secretKey= PEM_read_PrivateKey (fp, NULL, NULL, NULL);
  fclose(fp);
  if (secretKey == NULL)
  {
    clog << "Error loading key from key file: "<<keyFile<<endl;
    return Create<NilValue>();
  }
  

  cout << "key: "<<secretKey << endl;
  Ptr<ListValue> buf1 = DynamicCast<ListValue, Value> (msg);
  
  cout << "buf1: "<< buf1->GetTypeName()<<":"<<buf1->ToString()<<endl; 

  uint32_t numBytes = buf1->GetSerializedSize ();
  cout <<"buffer of size: "<<numBytes<<endl;

  Buffer buffer;
  buffer.AddAtStart (numBytes);
  Buffer::Iterator it = buffer.Begin ();
  buf1->Serialize (it);
  uint8_t *arr = const_cast<uint8_t*> (buffer.PeekData());
  
  cout<<"fmac eval Serialize: "<<PrintAsHex((char*)arr,numBytes)<<endl;

  HMAC_CTX ctx;
  HMAC_CTX_init (&ctx);
  //HMAC_Init_ex (&ctx, secretKey, EVP_PKEY_size (secretKey), EVP_sha1 (), NULL);
  char hmac[] = "Password";
  HMAC_Init_ex (&ctx, hmac, strlen((const char*)hmac), EVP_sha1 (), NULL);
  HMAC_Update (&ctx, arr, numBytes);

  //cout <<"fmac eval done hmac"<<endl;

  uint8_t* signature = new uint8_t[EVP_MAX_MD_SIZE];
  uint32_t sigLen;

  HMAC_Final (&ctx, signature, &sigLen);
  Ptr<Value> retval = ByteArrayValue::New (signature, sigLen);

  HMAC_cleanup (&ctx);
  cout<<"fmac eval result: "<< PrintAsHex((char*)signature,sigLen)<<endl;

  return retval;
}

Ptr<FunctionExpr>
FMAC::New (Ptr<Expression> message, Ptr<Expression> secretKey, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FMAC> retval = Create<FMAC> ();
  retval->m_message = message;
  retval->m_secretKey = secretKey;
  return retval;
}

Ptr<Value>
FReverse::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > result = rn_list (m_list->Eval(tuple));
  result.reverse();

  return ListValue::New (result);
}

Ptr<FunctionExpr>
FReverse::New (Ptr<Expression> list, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FReverse> retval = Create<FReverse> ();
  retval->m_list = list;
  return retval;
}

Ptr<Value>
FGet::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_list->Eval (tuple);
  Ptr<Value> iVal = m_i->Eval(tuple);
  int i = (DynamicCast<Int32Value, Value> (iVal))->GetInt32Value();

  if (!V_InstanceOf (lstVal, ListValue))
  {
    return Create<NilValue> ();
  }
  //NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue), "f_get can only be applied to a list type attribute.");

  list<Ptr<Value> > lst = rn_list (lstVal);

  //cout<<"fget i: "<<i<<"\tlst.size:"<<lst.size()<<endl;
  if (lst.end () == lst.begin ())
  {
    //cout <<"not found 1"<<endl;
    return Create<NilValue> ();
  }

  if (i >= (int)(lst.size()) )
  {
    //cout<<"not found 2"<<endl;
    return Create<NilValue> ();
  }
  
  rn_list_iterator it = lst.begin();
  for (int j=0; j<i; j++)
    it++;
  //cout<<"fget result: "<<*it<<endl;
  return (*it)->Clone ();
}

Ptr<FunctionExpr>
FGet::New (Ptr<Expression> list, Ptr<Expression> i, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FGet> retval = Create<FGet> ();
  retval->m_list = list;
  retval->m_i = i;
  return retval;
}


Ptr<Value>
FSubString::Eval (Ptr<Tuple> tuple)
{
  string result = str->Eval(tuple)->ToString();
  int32_t startIndexInt = rn_int32(startIndex->Eval(tuple));
  int32_t endIndexInt = rn_int32(endIndex->Eval(tuple));
  string substring = "";
  if(startIndexInt <= endIndexInt && endIndexInt<= result.length())
    {
      substring = result.substr(startIndexInt,endIndexInt-startIndexInt);
    }
  return StrValue::New (substring);
}

Ptr<FunctionExpr> 
FSubString::New (Ptr<Expression> str, Ptr<Expression> startIndex,Ptr<Expression> endIndex, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FSubString> retval = Create<FSubString> ();
  retval->str = str;
  retval->startIndex = startIndex;
  retval->endIndex = endIndex;
  return retval;
}

Ptr<Value>
FIndexOf::Eval (Ptr<Tuple> tuple)
{
  string result = str->Eval(tuple)->ToString();
  string patternToFind = strToFind->Eval(tuple)->ToString();
  int found = result.find(patternToFind);
  if(found==string::npos)
    found = -1;
  return Int32Value::New (found);
}

Ptr<FunctionExpr> 
FIndexOf::New (Ptr<Expression> str, Ptr<Expression> strToFind, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FIndexOf> retval = Create<FIndexOf> ();
  retval->str = str;
  retval->strToFind = strToFind;
  return retval;
}

Ptr<Value>
FStrLength::Eval (Ptr<Tuple> tuple)
{
  string result = str->Eval(tuple)->ToString();
  return Int32Value::New (result.length());
}

Ptr<FunctionExpr> 
FStrLength::New (Ptr<Expression> str, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FStrLength> retval = Create<FStrLength> ();
  retval->str = str;
  return retval;
}



/* ************************************************************** */

/* Re-implement the FPEdb and FPRule functions*/
/* The new implementation returns concrete tuple information in the provenance*/

Ptr<Value>
FPEdbTp::Eval(Ptr<Tuple> tuple)
{
  list<Ptr<Value> > provList = rn_list (m_prov->Eval(tuple));

  ostringstream tupleStr;
  bool is_tuple_name = true;
  bool is_first_var = true;
  for (rn_list_iterator it = provList.begin();it != provList.end(); it++)
    {
      // Tuple name
      if (is_tuple_name)
        {
          is_tuple_name = false;
        }
      else
        {
          if (is_first_var)
            {
              is_first_var = false;
              tupleStr << "(";
            }
          else
            {
              tupleStr << ",";
            }
        }
      
      tupleStr << (*it);
    }
  tupleStr << ")";

  return StrValue::New (tupleStr.str());
}

Ptr<FunctionExpr>
FPEdbTp::New (Ptr<Expression> prov, Ptr<Expression> id, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FPEdbTp> retval = Create<FPEdbTp>();
  retval->m_prov = prov;
  retval->m_id = id;
  return retval;
}


/* Find the value for var in provList*/
string
FetchVarValue(string var, string rbody, list<Ptr<Value> >& provList)
{
  // Find the first appearance of var in the rule body
  size_t pos_first_var = rbody.find(var);
  if (pos_first_var == string::npos)
    {
      return "";
    }

  // Find the name of the rule body that contains var
  size_t pos_body_leftpar = rbody.rfind("(", pos_first_var);
  size_t pos_rel_delimiter = rbody.rfind(";", pos_body_leftpar);
  size_t pos_body_rel = 0;
  if (pos_rel_delimiter != string::npos)
    {
      pos_body_rel = pos_rel_delimiter + 1;
    }
  size_t body_name_length = pos_body_leftpar - pos_body_rel;
  string body_name = rbody.substr(pos_body_rel, body_name_length);
  
  // Find the corresponding body tuple in the provList
  // Here we assume there is no redundant body tuple

  string body_prov;
  size_t btuple_left_paren = 0;
  size_t pos_body_tuple = 0;
  rn_list_iterator it;
  for (it = provList.begin(); it != provList.end(); it++)
    {
      body_prov = (*it)->ToString();
      btuple_left_paren = body_prov.find("(");
      pos_body_tuple = body_prov.rfind(body_name, btuple_left_paren);
      if (pos_body_tuple != string::npos)
        {
          //The provenance has the desired body tuple as its head
          break;
        }
    }
  if (it == provList.end())
    {
      //No body tuple is found
      return "";
    }


  // Find the position of value in the body tuple corresponding to var
  size_t pos_body_right_delimiter = rbody.find(")", pos_body_leftpar);
  size_t pos_btuple_right_delimiter = body_prov.find(")", btuple_left_paren);
  size_t pos_body_delimiter = pos_body_leftpar;
  size_t pos_val_left_delimiter = btuple_left_paren;

  while (pos_body_delimiter < pos_body_right_delimiter)
    {
      if (pos_body_delimiter == pos_first_var - 1)
        {
          break;
        }
      
      pos_body_delimiter = rbody.find(",", pos_body_delimiter+1);
      pos_val_left_delimiter = body_prov.find(",", pos_val_left_delimiter+1);
    }

  size_t pos_val_right_delimiter = body_prov.find(",", pos_val_left_delimiter+1);
  if (pos_val_right_delimiter > pos_btuple_right_delimiter ||
      pos_val_right_delimiter == string::npos)
    {
      pos_val_right_delimiter = pos_btuple_right_delimiter;
    }
  size_t value_length = pos_val_right_delimiter - pos_val_left_delimiter - 1; 
  string val = body_prov.substr(pos_val_left_delimiter+1, value_length);

  return val;
}

/* Symbolically execute the rule to derive the head tuple in string*/
/* Currently we assume all head attributes take values from body relations*/
string 
DeriveSymbolicHead(string rule, string rhead, string rbody, list<Ptr<Value> >& provList)
{
  cout << endl << "DeriveSymbolicHead" << endl;
  ostringstream htuple_stream;

  // Construct the name of the head tuple
  size_t pos_left_paren = rhead.find("(");
  string head_name = rhead.substr(0, pos_left_paren);
  htuple_stream << head_name << "(";
  
  /* Fetch variables of the head relation one by one.
   For each variable var, find the corresponding variable var'
   in the body relation (or atom).
   Follow var' in the body relation to its concrete value v
   in the body tuple in provList, and assign v to var in the head.*/

  bool end_of_head = false;
  size_t pos_left_delimiter = pos_left_paren;
  size_t pos_right_delimiter = pos_left_paren;
  string var;
  
  do {
    pos_right_delimiter = rhead.find(",", pos_right_delimiter+1);
    if (pos_right_delimiter == string::npos)
      {
        end_of_head = true;
        pos_right_delimiter = rhead.find(")",pos_left_paren);
      }
    size_t var_length = pos_right_delimiter - pos_left_delimiter - 1;
    string var = rhead.substr(pos_left_delimiter+1, var_length);
    cout << endl << "Obtain the value for the variable:" << var << endl; 
    string var_value = FetchVarValue(var, rbody, provList);
    cout << endl << "Result:" << var << ":" << var_value << endl;
    htuple_stream << var_value;
    if (!end_of_head)
      {
        htuple_stream << ",";
      }
    pos_left_delimiter = pos_right_delimiter;
  }while(!end_of_head);

  htuple_stream << ")";
  return htuple_stream.str();
}

Ptr<Value>
FPRuleItm::Eval(Ptr<Tuple> tuple)
{
  list<Ptr<Value> > provList = rn_list (m_provList->Eval (tuple));

  stringstream ss;

  string rule = m_rule->Eval (tuple)->ToString ();
  cout << endl << "rule:" << rule << endl;

  string rhead = m_head->Eval (tuple)->ToString ();
  cout << endl << "rhead:" << rhead << endl;

  string rbody = m_body->Eval (tuple)->ToString ();
  cout << endl << "rbody:" << rbody << endl;

  string headTuple = DeriveSymbolicHead(rule, rhead, rbody, provList);

  ss << headTuple << "<-";

  uint32_t ipaddr = (rn_ipv4 (m_rloc->Eval (tuple))).Get ();
  ipaddr = (ipaddr / 256) % 65536;
  ss << rule << "@n" << ipaddr << "(";

    int index = 0;

    for (rn_list_iterator it = provList.begin (); it != provList.end (); it++)
    {
      if (index++!=0) ss << "*";
      ss << (*it)->ToString ();
    }

    ss << ")";

return StrValue::New (ss.str ());
}

Ptr<FunctionExpr>
FPRuleItm::New (Ptr<Expression> provList, Ptr<Expression> rloc, Ptr<Expression> rule, Ptr<Expression> rhead, Ptr<Expression> rbody, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FPRuleItm> retval = Create<FPRuleItm>();
  retval->m_provList = provList;
  retval->m_rloc = rloc;
  retval->m_rule = rule;
  retval->m_head = rhead;
  retval->m_body = rbody;

  return retval;
  }
