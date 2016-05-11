materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(pQList,infinity,infinity,keys(1,2:cid)).
materialize(rQList,infinity,infinity,keys(1,2:cid)).
materialize(pResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).


/* Querying */

/* EDB vertex */
edb1 pReturn(@Ret,QID,VID,Prov) :- provQuery(@X,QID,VID,Ret),
       prov(@X,VID,RID,RLoc), RID==VID, Prov:=f_pEDB(VID,X).

/* IDB vertex */
idb1 pQList(@X,QID,a_LIST<RID>) :- provQuery(@X,QID,VID,Ret),
       prov(@X,VID,RID,RLoc), RID!=VID.
idb2 pResultTmp(@X,QID,Ret,VID,Buf) :-
       provQuery(@X,QID,VID,Ret), Buf:=f_empty().

idb3 pIterate(@X,QID,N) :- provQuery(@X,QID,VID,Ret), N:=1.
idb4 pIterate(@X,QID,N) :- pIterate(@X,QID,N1),
       pQList(@X,QID,List), N1<f_size(List), N:=N1+1.

idb5 eRuleQuery(@X,NQID,RID) :- pIterate(@X,QID,N),
       pQList(@X,QID,List), N<=f_size(List),
       RID:=f_item(List,N), NQID:=f_sha1(""+QID+RID).
idb6 ruleQuery(@RLoc,QID,RID,X) :- eRuleQuery(@X,QID,RID),
       prov(@X,VID,RID,RLoc).

idb7 pResultTmp(@X,QID,Ret,VID,Buf) :- rReturn(@X,NQID,RID,Prov),
       pResultTmp(@X,QID,Ret,VID,Buf1), NQID==f_sha1(""+QID+RID),
       Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).

idb8 ePReturn(@X,QID) :- pResultTmp(@X,QID,Ret,VID,Buf),
       pQList(@X,QID,List),
       f_size(Buf)==f_size(List), f_size(Buf)!=0.

idb9 pReturn(@Ret,QID,VID,Prov) :- ePReturn(@X,QID),
       pResultTmp(@X,QID,Ret,VID,Buf), Prov:=f_pIDB(Buf,X).

/* Rule Vertex */
rv1 rQList(@X,QID,List) :- ruleQuery(@X,QID,RID,Ret),
      ruleExec(@X,RID,R,List).
rv2 rResultTmp(@X,QID,Ret,RID,Buf) :-
      ruleQuery(@X,QID,RID,Ret), Buf:=f_empty().

rv3 rIterate(@X,QID,N) :- ruleQuery(@X,QID,RID,Ret), N:=1.
rv4 rIterate(@X,QID,N) :- rIterate(@X,QID,N1),
      rQList(@X,QID,List), N1<f_size(List), N:=N1+1.

rv5 eProvQuery(@X,NQID,VID) :- rIterate(@X,QID,N),
      rQList(@X,QID,List),
      VID:=f_item(List,N), NQID:=f_sha1(""+QID+VID).
rv6 provQuery(@X,QID,VID,X) :- eProvQuery(@X,QID,VID).

rv7 rResultTmp(@X,QID,Ret,RID,Buf) :- pReturn(@X,NQID,VID,Prov),
      rResultTmp(@X,QID,Ret,RID,Buf1), NQID==f_sha1(""+QID+VID),
      Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).

rv8 eRReturn(@X,QID) :- rResultTmp(@X,QID,Ret,RID,Buf),
      rQList(@X,QID,List), f_size(Buf)==f_size(List).
rv9 rReturn(@Ret,QID,RID,Prov) :- eRReturn(@X,QID),
      rResultTmp(@X,QID,Ret,RID,Buf),
      ruleExec(@X,RID,R,List), Prov:=f_pRULE(Buf,X,R).
