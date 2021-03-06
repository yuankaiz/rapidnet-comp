/****************
*
* Online compression with distributed hash list
*
*****************/

/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(link,infinity,infinity,keys(2)). /*Links between nodes*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(programID,infinity,infinity,keys(2)). /*Allow the user to specify program ID*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(2:cid)).
materialize(provLink, infinity, infinity, keys(2:cid,3:int32,4,5:cid,6:int32)).
materialize(equiHashTable,infinity,infinity,keys(2)). /*Hash table for equivalence detection*/
materialize(provHashTable,infinity,infinity,keys(3:list)). /*Hash table for provenance query*/
materialize(recvPacketQry, infinity, infinity, keys(2:cid))./*Provenance-queryable recv packet*/

/* 'ID' is a keyword. Parse error on using id for type!*/
/*The workaround is to call it cid or chord id. */

/* Tag format:*/
/* ((flag), rloc, rid, rhs, prgid, evhs, eqhs) */


/*Switch program*/
/* Execution of rs1 with provenance enabled*/
prov_rs1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
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
    RID := f_sha1(R + List).

prov_rs1_2 epacketCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ruleExec(@RLOC, RID, R, List).

prov_rs1_3 ruleExec(@RLOC, RID, R, List) :-
    epacketCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

/* Update provLink, which associates the current RID with the RID of the previous rule.*/
prov_rs1_4 provLink(@RLOC, ProgID, RID, CurLhash, Preloc, PreRID, PreLhash) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    Preloc := f_first(Tag),
    TempTag1 := f_removeFirst(Tag),
    PreRID := f_first(TempTag),
    TempTag2 := f_removeFirst(TempTag1),
    PreLhash := f_first(TempTag2),
    TempTag3 := f_removeFirst(TempTag2),
    ProgID := f_first(TempTag3),
    CurLhash := f_sha1(PreLhash + RID).

prov_rs1_5 packetProv(@Next, SrcAdd, DstAdd, Data, NewTag) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    TempNewTag1 := f_removeFirst(Tag),
    TempNewTag2 := f_removeFirst(TempNewTag1),
    PreLhash := f_first(TempNewTag2),
    CurLhash := f_sha1(PreLhash + RID),
    CurLhashlist := f_append(CurLhashlist),
    TempNewTag3 := f_removeFirst(TempNewTag2),
    RIDlist := f_append(RID),
    Loclist := f_append(RLOC),
    TempNewTag4 := f_concat(CurLhashlist, TempNewTag3),
    TempNewTag5 := f_concat(RIDlist, TempNewTag4),
    NewTag := f_concat(Loclist, TempNewTag5).

/* Execution for original rs1*/
rs1 packet(@Next, SrcAdd, DstAdd, Data, PIDHash) :-
        packet(@Node, SrcAdd, DstAdd, Data, PIDHash),
 flowEntry(@Node, DstAdd, Next),
 link(@Node, Next).

/*Host program*/
/* prov_ri1 to prov_ri3 hashes on equivalence-class identifier,*/
/* and check whether the hash value "PIDequi" exists in the hash set "equiHashTable"*/
prov_ri1 initPacketHash(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    programID(@Node, DstAdd, ProgID),
    PIDequi := f_sha1("" + Node + DstAdd).

prov_ri2 initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, a_COUNT<*>) :-
    initPacketHash(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID),
    equiHashTable(@Node, DstAdd, PIDequi).

prov_ri3 equiHashTable(@Node, DstAdd, PIDequi) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, PIDcount),
    PIDcount == 0.

/* Execution of original rh1*/
rh1 packet(@Next, SrcAdd, DstAdd, Data, PIDHash) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, PIDcount),
    link(@Node, Next),
    PIDev := f_sha1("initPacket" + Node + SrcAdd + DstAdd + Data),
    PIDcount != 0,
    PIDequiHash := f_append(PIDequi),
    PIDevHash := f_append(PIDev),
    ProgIDlist := f_append(ProgID),
    PIDHash1 := f_concat(PIDequiHash, PIDevHash),
    PIDHash := f_concat(PIDHash1, ProgIDlist).

/* Execution of rh1 with provenance enabled*/
prov_rh1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, PIDcount),
    link(@Node, Next),
    PIDcount == 0,
    PID := f_sha1(("link"+ Node)+ Next),
    List := f_append(PID),
    RLOC := Node,
    RID := f_sha1(R + List),
    R := "rh1",
    Equilist := f_append(PIDequi),
    PIDev := f_sha1(((("initPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data),
           Evlist := f_append(PIDev),
    ProgIDlist := f_append(ProgID),
    InitLhashlist := f_append("NULL"),
    InitProvNode := RLOC,
    InitProvNodelist := f_append(InitProvNode),
    InitProvHash := f_sha1("NULL"),
    InitProvHashlist := f_append(InitProvHash),
    TempTag1 := f_concat(InitProvNodelist, InitProvHashlist),
    TempTag2 := f_concat(TempTag1, InitLhashlist),
    TempTag3 := f_concat(TempTag2, ProgIDlist),
           TempTag4 := f_concat(TempTag3, Evlist),
    Tag := f_concat(TempTag4, Equilist).

/* Receive a packet with provenance maintenance enabled*/
prov_rh2_1 erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    packetProv(@Node, SrcAdd, DstAdd, Data, Tag),
    DstAdd == Node,
    PID2 := f_sha1(((("packet"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List := f_append(PID2),
    RLOC := Node,
    R := "rh2",
    RID := f_sha1(R + List).

prov_rh2_2 epacketCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ruleExec(@RLOC, RID, R, List).

prov_rh2_3 ruleExec(@RLOC, RID, R, List) :-
    epacketCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

prov_rh2_4 provLink(@RLOC, ProgID, RID, CurLhash, Preloc, PreRID, PreLhash) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    Preloc := f_first(Tag),
    TempTag1 := f_removeFirst(Tag),
    PreRID := f_first(TempTag),
    TempTag2 := f_removeFirst(TempTag1),
    PreLhash := f_first(TempTag2),
    TempTag3 := f_removeFirst(TempTag2),
    ProgID := f_first(TempTag3),
    CurLhash := f_sha1(PreLhash + RID).

prov_rh2_5 recvPacketProv(@Node, SrcAdd, DstAdd, Data, NewTag) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    TempNewTag1 := f_removeFirst(Tag),
    TempNewTag2 := f_removeFirst(TempNewTag1),
    PreLhash := f_first(TempNewTag2),
    CurLhash := f_sha1(PreLhash + RID),
    CurLhashlist := f_append(CurLhashlist),
    TempNewTag3 := f_removeFirst(TempNewTag2),
    RIDlist := f_append(RID),
    Loclist := f_append(RLOC),
    TempNewTag4 := f_concat(CurLhashlist, TempNewTag3),
    TempNewTag5 := f_concat(RIDlist, TempNewTag4),
    NewTag := f_concat(Loclist, TempNewTag5).

prov_rh2_6 recvPacket(@Node, SrcAdd, DstAdd, Data) :-
    recvPacketProv(@Node, SrcAdd, DstAdd, Data, NewTag).

/* Handle provenance information for the output recv tuple*/
/* Create a tuple in provHashTable. The tuple connects the output tuple to a provenance tree*/
ro1 provHashTable(@Node, ProvID, ProgID, PreLoc, PreRID, PreLhash) :-
    recvPacketProv(@Node, SrcAdd, DstAdd, Data, Tag),
    PreLoc := f_first(Tag),
    RemainTag1 := f_removeFirst(Tag),
    PreRID := f_first(RemainTag1),
    RemainTag2 := f_removeFirst(RemainTag1),
    PreLhash := f_first(RemainTag2),
    RemainTag3 := f_removeFirst(RemainTag2),
    ProgID := f_first(RemainTag3),
    RemainTag4 := f_removeFirst(RemainTag3),
    EvHash := f_first(RemainTag4),
    RemainTag5 := f_removeFirst(RemainTag4),
    EqHash := f_first(RemainTag5),
    ProvID := f_sha1(ProgID, PIDequi).

ro2 recvPacketQry(@Node, PID, ProvID, EvHash) :-
    recvPacketProv(@Node, SrcAdd, DstAdd, Data, Tag),
           PID := f_sha1("recvPacket" + Node + SrcAdd + DstAdd + Data),
    PreLoc := f_first(Tag),
    RemainTag1 := f_removeFirst(Tag),
    PreRID := f_first(RemainTag1),
    RemainTag2 := f_removeFirst(RemainTag1),
    PreLhash := f_first(RemainTag2),
    RemainTag3 := f_removeFirst(RemainTag2),
    ProgID := f_first(RemainTag3),
    RemainTag4 := f_removeFirst(RemainTag3),
    EvHash := f_first(RemainTag4),
    RemainTag5 := f_removeFirst(RemainTag4),
    EqHash := f_first(RemainTag5),
    ProvID := f_sha1(ProgID, PIDequi).

/* Execution of original rh2*/
rh2 recvPacket(@Node, SrcAdd, DstAdd, Data) :-
 packet(@Node, SrcAdd, DstAdd, Data, PIDHash),
 DstAdd == Node.

rho3 recvPacketQry(@Node, PID, ProvID, PIDev) :-
 packet(@Node, SrcAdd, DstAdd, Data, PIDHash),
 DstAdd == Node,
        PID := f_sha1("recvPacket" + Node + SrcAdd + DstAdd + Data),
 PIDequi := f_first(PIDHash),
 PIDHash1 := f_removeFirst(PIDHash),
 PIDev := f_first(PIDHash1),
 PIDHash2 := f_removeFirst(PIDHash1),
 ProgID := f_first(PIDHash2),
 ProvID := f_sha1(ProgID, PIDequi).
