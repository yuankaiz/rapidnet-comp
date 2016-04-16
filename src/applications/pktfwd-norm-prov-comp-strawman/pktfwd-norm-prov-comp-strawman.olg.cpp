/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(linkhr,infinity,infinity,keys(2)). /*Links between hosts and routers*/
materialize(link,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(device,infinity,infinity,keys(2:int32)). /*Identify the device type of the node*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(4:list)).




/*Switch program*/
/*A hit in the routing table, forward the packet accordingly*/
prov_rs1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, HashList) :-
    device(@Switch, Dvtype),
    packet(@Switch, SrcAdd, DstAdd, Data, HashList),
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
    Dvtype == 1,
    RLOC := Switch,
    R := "rs1",
    RID := f_sha1((R+ RLOC)+ List).

prov_rs1_2 epacketCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, HashList),
    ruleExec(@RLOC, RID, R, List).

prov_rs1_3 ruleExec(@RLOC, RID, R, List) :-
    epacketCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

prov_rs1_4 packet(@Next, SrcAdd, DstAdd, Data, NewHashList) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, HashList),
    Hash := f_append(RID),
    NewHashList := f_concat(HashList, Hash).

/*Host program*/
prov_rh1_1 epacketTemp(@RLOC, Switch, SrcAdd, DstAdd, Data, RID, R, List, HashList) :-
    device(@Host, Dvtype),
    initPacket(@Host, SrcAdd, DstAdd, Data),
    linkhr(@Host, Switch),
    PIDev := f_sha1(((("initPacket"+ Host)+ SrcAdd)+ DstAdd)+ Data),
    HashList := f_append(PIDev),
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
prov_rh2_1 erecvPacketTemp(@RLOC, Host, SrcAdd, DstAdd, Data, RID, R, List) :-
    device(@Host, Dvtype),
    packet(@Host, SrcAdd, DstAdd, Data),
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

prov_rh2_2 ruleExec(@RLOC, RID, R, List) :-
    erecvPacketTemp(@RLOC, Host, SrcAdd, DstAdd, Data, RID, R, List).

prov_rh2_3 erecvPacket(@Host, SrcAdd, DstAdd, Data, RID, RLOC) :-
    erecvPacketTemp(@RLOC, Host, SrcAdd, DstAdd, Data, RID, R, List).

prov_rh2_4 recvPacket(@Host, SrcAdd, DstAdd, Data) :-
    erecvPacket(@Host, SrcAdd, DstAdd, Data, RID, RLOC).

prov_rh2_5 prov(@Host, VID, RID, RLOC) :-
    erecvPacket(@Host, SrcAdd, DstAdd, Data, RID, RLOC),
    VID := f_sha1(((("recvPacket"+ Host)+ SrcAdd)+ DstAdd)+ Data).
