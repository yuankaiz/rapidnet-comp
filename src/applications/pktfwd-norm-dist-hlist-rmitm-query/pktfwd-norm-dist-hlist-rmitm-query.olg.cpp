/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(link,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Links between routers and other devices*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(2:cid,5,6:cid)).
materialize(prov, infinity, infinity, keys(2:cid,3,4:cid)).

/* Tables for provenance query*/
materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(rQList,infinity,infinity,keys(1,2:cid)).
materialize(pResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(childWait,infinity,infinity,keys(1,2:cid,3:cid)).


/* Forward a packet*/
prov_rs1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist) :-
    packet(@Node, SrcAdd, DstAdd, Data, PreLoc, PreRID),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PID2 := f_sha1(((("packet"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List := f_append(PID2),
    PID3 := f_sha1((("flowEntry"+ Node)+ DstAdd)+ Next),
    List3 := f_append(PID3),
    List := f_concat(List, List3),
    PID4 := f_sha1(("link"+ Node)+ Next),
    List4 := f_append(PID4),
    List := f_concat(List, List4),
    RLOC := Node,
    R := "rs1",
    RID := f_sha1((R+ RLOC)+ List),
    PreLoclist := f_append(PreLoc),
    PreRIDlist := f_append(PreRID),
    PreInfolist := f_concat(PreLoclist, PreRIDlist).

prov_rs1_2 ruleExec(@RLOC, RID, R, List, PreLoc, PreRID) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist),
    PreLoc := f_first(PreInfolist),
    Remainlist := f_removeFirst(PreInfolist),
    PreRID := f_first(Remainlist).

prov_rs1_3 packet(@Next, SrcAdd, DstAdd, Data, RLOC, RID) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist).

/* Packet initialization*/
prov_rh1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    link(@Node, Next),
    PID2 := f_sha1(((("initPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List := f_append(PID2),
    PID3 := f_sha1(("linkhr"+ Node)+ Next),
    List3 := f_append(PID3),
    List := f_concat(List, List3),
    PreLoc := f_sha1("NULL"),
    PreLoclist := f_append(PreLoc),
    PreRID := f_sha1("NULL"),
    PreRIDlist := f_append(PreRID),
    RLOC := Node,
    R := "rh1",
    RID := f_sha1((R+ RLOC)+ List),
    PreInfolist := f_concat(PreLoclist, PreRIDlist).

/*Receive a packet*/
prov_rh2_1 erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist) :-
    packet(@Node, SrcAdd, DstAdd, Data, PreLoc, PreRID),
    DstAdd == Node,
    PID2 := f_sha1(((("packet"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List := f_append(PID2),
    RLOC := Node,
    R := "rh2",
    RID := f_sha1((R+ RLOC)+ List),
    PreLoclist := f_append(PreLoc),
    PreRIDlist := f_append(PreRID),
    PreInfolist := f_concat(PreLoclist, PreRIDlist).

prov_rh2_2 ruleExec(@RLOC, RID, R, List, PreLoc, PreRID) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist),
    PreLoc := f_first(PreInfolist),
    Remainlist := f_removeFirst(PreInfolist),
    PreRID := f_first(Remainlist).

prov_rh2_3 recvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist).

prov_rh2_5 prov(@Node, VID, RLOC, RID) :-
    recvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC),
    VID := f_sha1(((("recvPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data).

/* Query program */


/* EDB vertex */
edb1 baseReturn(@Ret,QID,VID,Prov) :- baseQuery(@X,QID,VID,Ret), Prov:=f_pEDB(VID,X).

/* root vertex */
rtdb1 ruleQuery(@RLoc,NQID,RID,X) :- provQuery(@X,QID,VID,Ret),
       prov(@X,VID,RLoc,RID), RID!=VID,
 NQID:=f_sha1(""+QID+RID).
rtdb2 pResultTmp(@X,QID,Ret,VID,Buf) :-
       provQuery(@X,QID,VID,Ret), Buf:=f_empty().

rtdb3 pResultTmp(@X,QID,Ret,VID,Buf) :- rReturn(@X,NQID,RID,Prov),
       pResultTmp(@X,QID,Ret,VID,Buf1), NQID==f_sha1(""+QID+RID),
       Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).

rtdb4 pReturn(@Ret,QID,VID,Prov) :- pResultTmp(@X,QID,Ret,VID,Buf), Prov:=f_pIDB(Buf,X).

/* Rule Vertex */
rv1 rQList(@X,QID,List) :- ruleQuery(@X,QID,RID,Ret),
      ruleExec(@X,RID,R,List, PreLoc, PreRID).

rv2 ruleQuery(@PreLoc,NQID,PreRID,Ret) :- ruleQuery(@X,QID,RID,Ret),
 ruleExec(@X,RID,R,List, PreLoc, PreRID),
 PreRID != f_sha1("NULL"),
 NQID:=f_sha1(""+QID+VID).

rv3 childWait(@X,QID,RID,Ret,Itm) :- ruleQuery(@X,QID,RID,Ret),
 ruleExec(@X,RID,R,List, PreLoc, PreRID),
 PreRID == f_sha1("NULL"),
 NQID:=f_sha1(""+QID+VID),
 Itm := 0.

rv4 childWait(@X,QID,RID,Ret,Itm) :- ruleQuery(@X,QID,RID,Ret),
 ruleExec(@X,RID,R,List, PreLoc, PreRID),
 PreRID != f_sha1("NULL"),
 NQID:=f_sha1(""+QID+VID),
 Itm := 1.

rv5 rResultTmp(@X,QID,Ret,RID,Buf) :-
      ruleQuery(@X,QID,RID,Ret), Buf:=f_empty().

rv6 rIterate(@X,QID,N) :- ruleQuery(@X,QID,RID,Ret), N:=1.
rv7 rIterate(@X,QID,N) :- rIterate(@X,QID,N1),
      rQList(@X,QID,List), N1<f_size(List), N:=N1+1.

rv8 baseQuery(@X,NQID,VID) :- rIterate(@X,QID,N),
      rQList(@X,QID,List),
      VID:=f_item(List,N), NQID:=f_sha1(""+QID+VID).

rv9 rResultTmp(@X,QID,Ret,RID,Buf) :- baseReturn(@X,NQID,VID,Prov),
      rResultTmp(@X,QID,Ret,RID,Buf1), NQID==f_sha1(""+QID+VID),
      Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).

rv10 rResultTmp(@X,QID,Ret,RID,Buf):- rReturn(@X,QID,RID,Prov),
 rResultTmp(@X,QID,Ret,RID,Buf1),
 NQID==f_sha1(""+QID+RID),
 Buf2:=f_append(Prov),
 Buf:=f_concat(Buf2, Buf1).

rv11 eRReturn(@X,QID) :- rResultTmp(@X,QID,Ret,RID,Buf),
 childWait(@X,QID,RID,Ret,Itm), Itm == 1,
      rQList(@X,QID,List), f_size(Buf)==f_size(List)+1.

rv12 eRReturn(@X,QID) :- rResultTmp(@X,QID,Ret,RID,Buf),
 childWait(@X,QID,RID,Ret,Itm), Itm == 0,
      rQList(@X,QID,List), f_size(Buf)==f_size(List).

rv13 rReturn(@Ret,QID,RID,Prov) :- eRReturn(@X,QID),
      rResultTmp(@X,QID,Ret,RID,Buf),
      ruleExec(@X,RID,R,List,PreLoc,PreRID), Prov:=f_pRULE(Buf,X,R).
