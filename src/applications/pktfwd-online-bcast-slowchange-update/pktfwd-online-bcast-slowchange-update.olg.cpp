/****************
*
* Online compression with distributed hash list,
* with auxiliary tables maintaining rule and tuple information
*
*****************/

/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(link,infinity,infinity,keys(2)). /*Links between nodes*/
materialize(flowEntry,infinity,infinity,keys(2,3)). /*Routing information between nodes*/
materialize(programID,infinity,infinity,keys(2)). /*Allow the user to specify program ID*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(2:cid)).
materialize(provLink, infinity, infinity, keys(2:cid,3:int32,4,5:cid,6:int32,7:cid,8:cid)).
materialize(equiHashTable,infinity,infinity,keys(2:cid,3:list)). /*Hash table for equivalence detection*/
materialize(provHashTable,infinity,infinity,keys(2:cid,3:str,4,5:cid,6:int32)). /*Hash table for provenance query*/
materialize(provRef, infinity, infinity, keys(2:cid))./*Reference to the provenance tree*/

/* Tables for handling update of slow-changing tables*/
materialize(updateRoute, infinity, infinity, keys(2,3)).
materialize(trigRecord, infinity, infinity, keys(2:cid)).


/* 'ID' is a keyword. Parse error on using id for type!
The workaround is to call it cid or chord id. */


/* Packet forwarding*/
/* Execution of rs1 with provenance enabled*/
prov_rs1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, NewTag) :-
    packetProv(@Node, SrcAdd, DstAdd, Data, Tag),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PID3 := f_sha1((("flowEntry"+ Node)+ DstAdd)+ Next),
    List := f_append(PID3),
    PID4 := f_sha1(("link"+ Node)+ Next),
    List4 := f_append(PID4),
    List := f_concat(List, List4),
    RLOC := Node,
    R := "rs1",
    RID := f_sha1((R+ RLOC)+ List),
    ProvID := f_first(Tag),
    NewProvID := f_sha1(""+ProvID+RID),
    NewProvID := f_append(NewProvID),
    NewTag1 := f_removeFirst(Tag),
    NewTag := f_concat(NewProvID, NewTag1).

prov_rs1_2 epacketCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ruleExec(@RLOC, RID, R, List).

prov_rs1_3 ruleExec(@RLOC, RID, R, List) :-
    epacketCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

/* Update provLink, which associates the current RID with the RID of the previous rule.*/
prov_rs1_4 provLink(@RLOC, RID, CurCount, Preloc, PreRID, PreCount, PIDequi, ProvID) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ProvID := f_first(Tag),
    TempTag0 := f_removeFirst(Tag),
    PreCount := f_first(TempTag0),
    CurCount := PreCount + 1,
    TempTag1 := f_removeFirst(TempTag0),
    Preloc := f_first(TempTag1),
    TempTag2 := f_removeFirst(TempTag1),
    PreRID := f_first(TempTag2),
    TempTag3 := f_removeFirst(TempTag2),
    PIDequi := f_first(TempTag3).

prov_rs1_5 packetProv(@Next, SrcAdd, DstAdd, Data, NewTag) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ProvID := f_first(Tag),
    ProvIDList := f_append(ProvID),
    TempNewTag0 := f_removeFirst(Tag),
    Count := f_first(TempNewTag0),
    NewCount := Count + 1,
    NewCountlist := f_append(NewCount),
    TempNewTag1 := f_removeFirst(TempNewTag0),
    TempNewTag2 := f_removeFirst(TempNewTag1),
    TempNewTag3 := f_removeFirst(TempNewTag2),
    RIDlist := f_append(RID),
    Loclist := f_append(RLOC),
    TempNewTag4 := f_concat(RIDlist, TempNewTag3),
    TempNewTag5 := f_concat(Loclist, TempNewTag4),
    TempNewTag6 := f_concat(NewCountlist, TempNewTag5),
    NewTag := f_concat(ProvIDList, TempNewTag6).

/* Execution for original rs1*/
rs1 packet(@Next, SrcAdd, DstAdd, Data, PIDHash) :-
        packet(@Node, SrcAdd, DstAdd, Data, PIDHash),
 flowEntry(@Node, DstAdd, Next),
 link(@Node, Next).

/* Packet initialization*/
/* prov_ri1 to prov_ri3 hashes on equivalence-class identifier,*/
/* and check whether the hash value "PIDequi" exists in the hash set "equiHashTable"*/
prov_ri1 initPacketHash(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    programID(@Node, DstAdd, ProgID),
    PIDequi := f_sha1("" + Node + DstAdd).

prov_ri2 initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, a_COUNT<*>) :-
    initPacketHash(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID),
    equiHashTable(@Node, PIDequi, EquiKeys, Flag).

prov_ri3 initPacketProv(@Node, Node, SrcAdd, DstAdd, Data, PIDequi, ProgID) :-
    initPacketHash(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID),
    equiHashTable(@Node, PIDequi, EquiKeys, Flag),
    Flag == 0.

prov_ri4 initPacketNoProv(@Node, Node, SrcAdd, DstAdd, Data, PIDequi, ProgID) :-
    initPacketHash(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID),
    equiHashTable(@Node, PIDequi, EquiKeys, Flag),
    Flag == 1.

prov_ri5 equiHashTable(@Node, PIDequi, EquiKeys, NewFlag) :-
    initPacketProv(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID),
    equiHashTable(@Node, PIDequi, EquiKeys, Flag),
    Flag == 0,
    NewFlag := 1.

prov_ri6 equiHashTable(@Node, PIDequi, EquiKeys, Flag) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, PIDcount),
    SrcList := f_append(SrcAdd),
    DstList := f_append(DstAdd),
    EquiKeys := f_concat(SrcList, DstList),
    Flag := 1,
    PIDcount == 0.

/* Execution of original rh1*/
rh1 packet(@Next, SrcAdd, DstAdd, Data, PIDHash) :-
    initPacketNoProv(@Node, Node, SrcAdd, DstAdd, Data, PIDequi, ProgID),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PIDev := f_sha1("initPacket" + Node + SrcAdd + DstAdd + Data),
    PIDequiHash := f_append(PIDequi),
    PIDevHash := f_append(PIDev),
    ProgIDlist := f_append(ProgID),
    PIDHash1 := f_concat(PIDequiHash, PIDevHash),
    PIDHash := f_concat(PIDHash1, ProgIDlist).

/* Execution of rh1 with provenance enabled*/
/* Tag = (layered hash, count, preNode, preRID, equiKey, eventHash, progID)*/
prov_rh1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, PIDcount),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PIDcount == 0,
    PID1 := f_sha1(("link"+ Node)+ Next),
    PID2 := f_sha1("flowEntry"+Node+DstAdd+Next),
    PID1List := f_append(PID1),
    PID2List := f_append(PID2),
    List := f_concat(PID1List, PID2List),
    RLOC := Node,
    R := "rh1",
    RID := f_sha1((R+ RLOC)+ List),
    Equilist := f_append(PIDequi),
    PIDev := f_sha1(((("initPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data),
           Evlist := f_append(PIDev),
    ProgIDlist := f_append(ProgID),
    Count := 0,
    Countlist := f_append(Count),
    InitProvNode := Node,
    InitProvNodelist := f_append(InitProvNode),
    InitProvHash := f_sha1("NULL"),
    InitProvHashlist := f_append(InitProvHash),
    ProvID := f_sha1(""+InitProvHash+RID),
    ProvIDList := f_append(ProvID),
    TempTag0 := f_concat(ProvIDList, Countlist),
    TempTag1 := f_concat(TempTag0, InitProvNodelist),
    TempTag2 := f_concat(TempTag1, InitProvHashlist),
    TempTag3 := f_concat(TempTag2, Equilist),
           TempTag4 := f_concat(TempTag3, Evlist),
    Tag := f_concat(TempTag4, ProgIDlist).

prov_rh1_2 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    initPacketProv(@Node, Node, SrcAdd, DstAdd, Data, PIDequi, ProgID),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PID1 := f_sha1(("link"+ Node)+ Next),
    PID2 := f_sha1("flowEntry"+Node+DstAdd+Next),
    PID1List := f_append(PID1),
    PID2List := f_append(PID2),
    List := f_concat(PID1List, PID2List),
    RLOC := Node,
    R := "rh1",
    RID := f_sha1((R+ RLOC)+ List),
    Equilist := f_append(PIDequi),
    PIDev := f_sha1(((("initPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data),
           Evlist := f_append(PIDev),
    ProgIDlist := f_append(ProgID),
    Count := 0,
    Countlist := f_append(Count),
    InitProvNode := Node,
    InitProvNodelist := f_append(InitProvNode),
    InitProvHash := f_sha1("NULL"),
    InitProvHashlist := f_append(InitProvHash),
    ProvID := f_sha1(""+InitProvHash+RID),
    ProvIDList := f_append(ProvID),
    TempTag0 := f_concat(ProvIDList, Countlist),
    TempTag1 := f_concat(TempTag0, InitProvNodelist),
    TempTag2 := f_concat(TempTag1, InitProvHashlist),
    TempTag3 := f_concat(TempTag2, Equilist),
           TempTag4 := f_concat(TempTag3, Evlist),
    Tag := f_concat(TempTag4, ProgIDlist).


/* Packet receipt*/
/* Receive a packet with provenance maintenance enabled*/
prov_rh2_1 erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, NewTag) :-
    packetProv(@Node, SrcAdd, DstAdd, Data, Tag),
    DstAdd == Node,
    List := f_empty(),
    RLOC := Node,
    R := "rh2",
    RID := f_sha1((R+ RLOC)+ List),
    ProvID := f_first(Tag),
    NewProvID := f_sha1(""+ProvID+RID),
    NewProvID := f_append(NewProvID),
    NewTag1 := f_removeFirst(Tag),
    NewTag := f_concat(NewProvID, NewTag1).

prov_rh2_2 epacketCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ruleExec(@RLOC, RID, R, List).

prov_rh2_3 ruleExec(@RLOC, RID, R, List) :-
    epacketCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

prov_rh2_4 provLink(@RLOC, RID, CurCount, Preloc, PreRID, PreCount, PIDequi, ProvID) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ProvID := f_first(Tag),
    TempTag0 := f_removeFirst(Tag),
    PreCount := f_first(TempTag0),
    CurCount := PreCount + 1,
    TempTag1 := f_removeFirst(TempTag0),
    Preloc := f_first(TempTag1),
    TempTag2 := f_removeFirst(TempTag1),
    PreRID := f_first(TempTag2),
    TempTag3 := f_removeFirst(TempTag2),
    PIDequi := f_first(TempTag3).

prov_rh2_5 recvPacketProv(@Node, SrcAdd, DstAdd, Data, NewTag) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ProvID := f_first(Tag),
    ProvIDList := f_append(ProvID),
    TempNewTag0 := f_removeFirst(Tag),
    Count := f_first(TempNewTag0),
    NewCount := Count + 1,
    NewCountlist := f_append(NewCount),
    TempNewTag1 := f_removeFirst(TempNewTag0),
    TempNewTag2 := f_removeFirst(TempNewTag1),
    TempNewTag3 := f_removeFirst(TempNewTag2),
    RIDlist := f_append(RID),
    Loclist := f_append(RLOC),
    TempNewTag4 := f_concat(RIDlist, TempNewTag3),
    TempNewTag5 := f_concat(Loclist, TempNewTag4),
    TempNewTag6 := f_concat(NewCountlist, TempNewTag5),
    NewTag := f_concat(ProvIDList, TempNewTag6).


/* Handle provenance information for the output recv tuple*/
/* Create a tuple in provHashTable. The tuple connects the output tuple to a provenance tree*/
ro1 provHashTable(@Node, PIDequi, ProgID, Loc, RID, Count) :-
    recvPacketProv(@Node, SrcAdd, DstAdd, Data, Tag),
    ProvID := f_first(Tag),
    RemainTag0 := f_removeFirst(Tag),
    Count := f_first(RemainTag0),
    RemainTag1 := f_removeFirst(RemainTag0),
    Loc := f_first(RemainTag1),
    RemainTag2 := f_removeFirst(RemainTag1),
    RID := f_first(RemainTag2),
    RemainTag3 := f_removeFirst(RemainTag2),
    PIDequi := f_first(RemainTag3),
    RemainTag4 := f_removeFirst(RemainTag3),
    PIDev := f_first(RemainTag4),
    RemainTag5 := f_removeFirst(RemainTag4),
    ProgID := f_first(RemainTag5).

ro2 provRef(@Node, PID, PIDequi, PIDev) :-
    recvPacketProv(@Node, SrcAdd, DstAdd, Data, Tag),
           PID := f_sha1("recvPacket" + Node + SrcAdd + DstAdd + Data),
    ProvID := f_first(Tag),
    RemainTag0 := f_removeFirst(Tag),
    Count := f_first(RemainTag0),
    RemainTag1 := f_removeFirst(RemainTag0),
    Loc := f_first(RemainTag1),
    RemainTag2 := f_removeFirst(RemainTag1),
    RID := f_first(RemainTag2),
    RemainTag3 := f_removeFirst(RemainTag2),
    PIDequi := f_first(RemainTag3),
    RemainTag4 := f_removeFirst(RemainTag3),
    PIDev := f_first(RemainTag4).


ro3 recvPacket(@Node, SrcAdd, DstAdd, Data) :-
 recvPacketProv(@Node, SrcAdd, DstAdd, Data, Tag).

/* Execution of original rh2*/
rh2 recvPacketNP(@Node, SrcAdd, DstAdd, Data, PIDHash) :-
 packet(@Node, SrcAdd, DstAdd, Data, PIDHash),
 DstAdd == Node.

rho3 provRef(@Node, PID, PIDequi, PIDev) :-
 recvPacketNP(@Node, SrcAdd, DstAdd, Data, PIDHash),
        PID := f_sha1("recvPacket" + Node + SrcAdd + DstAdd + Data),
 PIDequi := f_first(PIDHash),
 PIDHash1 := f_removeFirst(PIDHash),
 PIDev := f_first(PIDHash1).

rho4 recvPacket(@Node, SrcAdd, DstAdd, Data) :-
 recvPacketNP(@Node, SrcAdd, DstAdd, Data, PIDHash).


/* Handle update of slow-changing tables*/
/* The current slow-changing table is the flowEntry table*/

/* Insert the new flowEntry*/
ru1 flowEntry(@Node, DstAdd, Next) :-
 updateRoute(@Node, DstAdd, Next).

/* Broadcast the update to all the nodes*/

/* Initiate the ResetTrigger */
ru2 resetTrigger(@Node, SeqID) :-
 updateRoute(@Node, DstAdd, Next),
 Time := f_now(),
 SeqID := f_sha1(""+Time).

/* Count the number of trigRecord that has SeqID*/
ru3 trigRecordCount(@Node, SeqID, a_COUNT<*>) :-
 resetTrigger(@Node, SeqID),
 trigRecord(@Node, SeqID).

/* Record the sequence ID if there is no one existing*/
ru4 trigRecord(@Node, SeqID) :-
 trigRecordCount(@Node, SeqID, SeqCount),
 SeqCount == 0.

/* Keep broadcasting the ResetTrigger if it is the first one ever seen*/
ru5 resetTrigger(@Next, SeqID) :-
 trigRecordCount(@Node, SeqID, SeqCount),
 link(@Node, Next),
 SeqCount == 0.

/* Refresh the local equiHashTable*/
ru6 equiHashTable(@Node, PIDequi, EquiKeys, NewFlag) :-
 resetTrigger(@Node, SeqID),
 equiHashTable(@Node, PIDequi, EquiKeys, Flag),
 NewFlag := 0.
