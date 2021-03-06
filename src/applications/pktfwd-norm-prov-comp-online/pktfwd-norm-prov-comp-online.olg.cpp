/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(linkhr,infinity,infinity,keys(2)). /*Links between hosts and routers*/
materialize(link,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(device,infinity,infinity,keys(2:int32)). /*Identify the device type of the node*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(4:list)).
materialize(equiHashTable,infinity,infinity,keys(2)). /*Hash table for equivalence detection*/
materialize(provHashTable,infinity,infinity,keys(3:list)). /*Hash table for provenance query*/
materialize(recvAuxPkt, infinity, infinity, keys(2:cid)).

/* 'ID' is a keyword. Parse error on using id for type!
The workaround is to call it cid or chord id. */




/*Switch program*/
/*A hit in the routing table, forward the packet accordingly*/
prov_rs1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    device(@Switch, Dvtype),
    packetProv(@Switch, SrcAdd, DstAdd, Data, Tag),
    flowEntry(@Switch, DstAdd, Next),
    link(@Switch, Next),
    PID1 := f_sha1(("device"+ Switch)+ Dvtype),
    List := f_append(PID1),
    PID3 := f_sha1((("flowEntry"+ Switch)+ DstAdd)+ Next),
    List3 := f_append(PID3),
    List := f_concat(List, List3),
    PID4 := f_sha1(("link"+ Switch)+ Next),
    List4 := f_append(PID4),
    List := f_concat(List, List4),
    Dvtype == 0,
    RLOC := Switch,
    R := "rs1",
    RID := f_sha1((R+ RLOC)+ List).

prov_rs1_2 epacketCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ruleExec(@RLOC, RID, R, List).

prov_rs1_3 ruleExec(@RLOC, RID, R, List) :-
    epacketCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

prov_rs1_4 packetProv(@Next, SrcAdd, DstAdd, Data, NewTag) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    Hash := f_append(RID),
    NewTag := f_concat(Tag, Hash).

/* Rules for normal execution without provenance */
/*
rs1 packetNonProv(@Next, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
        device(@Switch, Dvtype),
        packetNonProv(@Switch, SrcAdd, DstAdd, Data, PIDequi, PIDev),
	flowEntry(@Switch, DstAdd, Next),
	link(@Switch, Next),
        Dvtype == SWITCH.
*/
rs1 packet(@Next, SrcAdd, DstAdd, Data, PIDHash) :-
        device(@Switch, Dvtype),
        packet(@Switch, SrcAdd, DstAdd, Data, PIDHash),
 flowEntry(@Switch, DstAdd, Next),
 link(@Switch, Next),
        Dvtype == 0 .

/*Host program*/
/* Hash on equivalence attributes*/
rh100 initPacketHash(@Host, SrcAdd, DstAdd, Data, PIDequi) :-
    initPacket(@Host, SrcAdd, DstAdd, Data),
    PIDequi := f_sha1("" + Host + DstAdd).

rh101 initPacketCount(@Host, SrcAdd, DstAdd, Data, PIDequi, a_COUNT<*>) :-
    initPacketHash(@Host, SrcAdd, DstAdd, Data, PIDequi),
    equiHashTable(@Host, DstAdd, PIDequi).

rh102 equiHashTable(@Host, DstAdd, PIDequi) :-
    initPacketCount(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDcount),
    PIDcount == 0.

/*
rh103 packetNonProv(@Switch, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
    device(@Host, Dvtype),
    initPacketCount(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDcount),
    linkhr(@Host, Switch),
    Dvtype == HOST,
    PIDev := f_sha1("initPacket" + Host + SrcAdd + DstAdd + Data),
    PIDcount != 0.
*/
rh103 packet(@Switch, SrcAdd, DstAdd, Data, PIDHash) :-
    device(@Host, Dvtype),
    initPacketCount(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDcount),
    linkhr(@Host, Switch),
    Dvtype == 1,
    PIDev := f_sha1("initPacket" + Host + SrcAdd + DstAdd + Data),
    PIDcount != 0,
    PIDequiHash := f_append(PIDequi),
    PIDevHash := f_append(PIDev),
    PIDHash := f_concat(PIDequiHash, PIDevHash).

prov_rh1_1 epacketTemp(@RLOC, Switch, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    device(@Host, Dvtype),
    initPacketCount(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDcount),
    linkhr(@Host, Switch),
    PIDcount == 0,
    PIDev := f_sha1(((("initPacket"+ Host)+ SrcAdd)+ DstAdd)+ Data),
    Equilist := f_append(PIDequi),
           Evlist := f_append(PIDev),
           Tag := f_concat(Equilist, Evlist),
    PID1 := f_sha1(("device"+ Host)+ Dvtype),
    List := f_append(PID1),
    PID2 := f_sha1(("linkhr"+ Host)+ Switch),
    List2 := f_append(PID2),
    List := f_concat(List, List2),
    Dvtype == 1,
    RLOC := Host,
    R := "rh1",
    RID := f_sha1((R+ RLOC)+ List).

/*Receive a packet*/
prov_rh2_1 erecvPacketTemp(@RLOC, Host, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    device(@Host, Dvtype),
    packetProv(@Host, SrcAdd, DstAdd, Data, Tag),
    DstAdd == Host,
    Dvtype == 1,
    PID1 := f_sha1(("device"+ Host)+ Dvtype),
    List := f_append(PID1),
    PID2 := f_sha1(((("packet"+ Host)+ SrcAdd)+ DstAdd)+ Data),
    List2 := f_append(PID2),
    List := f_concat(List, List2),
    RLOC := Host,
    R := "rh2",
    RID := f_sha1((R+ RLOC)+ List).

prov_rh2_2 epacketCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    erecvPacketTemp(@RLOC, Host, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ruleExec(@RLOC, RID, R, List).

prov_rh2_3 ruleExec(@RLOC, RID, R, List) :-
    epacketCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

prov_rh2_4 erecvPacket(@Host, SrcAdd, DstAdd, Data, RID, RLOC, Tag) :-
    erecvPacketTemp(@RLOC, Host, SrcAdd, DstAdd, Data, RID, R, List, Tag).

prov_rh2_5 recvPacketTemp(@Host, SrcAdd, DstAdd, Data, Tag) :-
    erecvPacket(@Host, SrcAdd, DstAdd, Data, RID, RLOC, Tag),
           Hash := f_append(RID),
           NewTag := f_concat(Tag, Hash).

prov_rh2_6 recvPacketDecomp(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev, Hashlist) :-
    recvPacketTemp(@Host, SrcAdd, DstAdd, Data, Tag),
    PIDequi := f_first(Tag),
    Remainlist := f_removeFirst(Tag),
    PIDev := f_first(Remainlist),
    Hashlist := f_removeFirst(Remainlist).

prov_rh2_7 provHashTable(@Host, PIDequi, Hashlist) :-
    recvPacketDecomp(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev, Hashlist).

prov_rh2_8 recvPacket(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
    recvPacketDecomp(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev, Hashlist).

/*
rh2 recvPacket(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
        device(@Host, Dvtype),
	packetNonProv(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev),
	DstAdd == Host,
        Dvtype == HOST.
*/
rh2 recvPacket(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev) :-
        device(@Host, Dvtype),
 packet(@Host, SrcAdd, DstAdd, Data, PIDHash),
 PIDequi := f_first(PIDHash),
 PIDevHash := f_removeFirst(PIDHash),
 PIDev := f_first(PIDevHash),
 DstAdd == Host,
        Dvtype == 1 .

rh3 recvAuxPkt(@Host, PID, PIDequi, PIDev) :-
        recvPacket(@Host, SrcAdd, DstAdd, Data, PIDequi, PIDev),
        PID := f_sha1("recvPacket" + Host + SrcAdd + DstAdd + Data).
