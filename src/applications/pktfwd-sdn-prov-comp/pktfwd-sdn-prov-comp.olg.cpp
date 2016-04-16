/* initPacket is the event tuple */
/* Note: we remove RLoc from RID in this version*/

/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(linkhr,infinity,infinity,keys(2)). /*Links between hosts and routers*/
materialize(link,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(flowEntry,infinity,infinity,keys(2,4:int32)). /*Flow tables of switches*/
materialize(maxPriority,infinity,infinity,keys(2:int32)). /*Determine the maximum priority of the flow table*/
materialize(device,infinity,infinity,keys(2:int32)). /*Identify the device type of the node*/
materialize(hashTable,infinity,infinity,keys(2:int32)). /*Hash table for compression*/

/* Provenance tables*/
materialize(ruleExec,infinity,infinity,keys(4:list)). /*Record the rule execution*/
materialize(recvAuxPkt,infinity,infinity,keys(2,3:str)). /*Record the hash list*/




/* Rewritten provenance-enabled rules*/
/*Switch program*/
/*Query the controller when receiving unknown packets */
rs10 eMatchingPacketTemp(@RLoc, SwcSrcList, DstAdd, Data, TopPriority, RID, R, List, HashList) :-
    device(@Switch, Dvtype),
    packet(@Switch, SrcAdd, DstAdd, Data, HashList),
    maxPriority(@Switch, TopPriority),
    Dvtype == 0,
    RLoc := Switch,
    R := "rs1",
    PID1 := f_sha1("device" + Switch + Dvtype),
    PID2 := f_sha1("maxPriority" + Switch + TopPriority),
    List := f_append(PID1),
    List2 := f_append(PID2),
    List := f_concat(List, List2),
    RID := f_sha1(R + List),
    SwitchList := f_append(Switch),
    SrcAddList := f_append(SrcAdd),
    SwcSrcList := f_concat(SwitchList, SrcAddList).

rs11 eMatchingPacket(@Switch, SrcAdd, DstAdd, Data, TopPriority, RID, RLoc, HashList) :-
    eMatchingPacketTemp(@RLoc, SwcSrcList, DstAdd, Data, TopPriority, RID, R, List, HashList),
    Switch := f_first(SwcSrcList),
    SrcList := f_removeFirst(SwcSrcList),
    SrcAdd := f_first(SrcList).

/* TODO: the performance of using aggregation for hash table is not expected to be good */
rs12 eMatchingPacketCount(@RLoc, RID, R, List, a_COUNT<*>) :-
    eMatchingPacketTemp(@RLoc, SwcSrcList, DstAdd, Data, TopPriority, RID, R, List, HashList),
    ruleExec(@RLoc, RID, R, List).

/* If the RID does not exist, store the provenance node*/
rs13 ruleExec(@RLoc, RID, R, List) :-
    eMatchingPacketCount(@RLoc, RID, R, List, Rcount),
    Rcount == 0.

/* Add the RID to the HashList*/
rs15 matchingPacket(@Switch, SrcAdd, DstAdd, Data, TopPriority, NewHashList) :-
 eMatchingPacket(@Switch, SrcAdd, DstAdd, Data, TopPriority, RID, RLoc, HashList),
 Hash := f_append(RID),
 NewHashList := f_concat(HashList, Hash).

/*Recursively matching entries in a routing table*/
rs20 eMatchingPacketTemp(@RLoc, SwcSrcList, DstAdd, Data, NextPriority, RID, R, List, HashList) :-
    device(@Switch, Dvtype),
    matchingPacket(@Switch, SrcAdd, DstAdd, Data, Priority, HashList),
    flowEntry(@Switch, DstEntry, Next, Priority),
    Priority > 0,
    DstAdd != DstEntry,
    NextPriority := Priority - 1,
    Dvtype == 0,
    RLoc := Switch,
    R := "rs2",
    PID1 := f_sha1("device" + Switch + Dvtype),
    PID2 := f_sha1("flowEntry" + Switch + DstEntry + Next + Priority),
    List := f_append(PID1),
    List2 := f_append(PID2),
    List := f_concat(List, List2),
    RID := f_sha1(R + List),
    SwitchList := f_append(Switch),
    SrcAddList := f_append(SrcAdd),
    SwcSrcList := f_concat(SwitchList, SrcAddList).

/* rs21 - rs24 are identical to rs11 - rs14*/


/*A hit in the routing table, forward the packet accordingly*/
rs30 ePacketTemp(@RLoc, Next, SrcAdd, DstAdd, Data, RID, R, List, HashList) :-
        device(@Switch, Dvtype),
        matchingPacket(@Switch, SrcAdd, DstAdd, Data, Priority, HashList),
 flowEntry(@Switch, DstEntry, Next, Priority),
 link(@Switch, Next),
 Priority > 0,
 DstAdd == DstEntry,
        Dvtype == 0,
 RLoc := Switch,
 R := "rs3",
 PID1 := f_sha1("device" + Switch + Dvtype),
 PID2 := f_sha1("flowEntry" + Switch + DstEntry + Next + Priority),
 PID3 := f_sha1("link" + Switch + Next),
 List := f_append(PID1),
 List2 := f_append(PID2),
 List := f_concat(List, List2),
 List3 := f_append(PID3),
 List := f_concat(List, List3),
 RID := f_sha1(R + List).

rs31 ePacket(@Next, SrcAdd, DstAdd, Data, RID, RLoc, HashList) :-
 ePacketTemp(@RLoc, Next, SrcAdd, DstAdd, Data, RID, R, List, HashList).

/*TODO: optimization - join on RID is enough*/
rs32 ePacketCount(@RLoc, RID, R, List, a_COUNT<*>) :-
 ePacketTemp(@RLoc, Next, SrcAdd, DstAdd, Data, RID, R, List, HashList),
 ruleExec(@RLoc, RID, R, List).

rs33 ruleExec(@RLoc, RID, R, List) :-
 ePacketCount(@RLoc, RID, R, List, Rcount),
 Rcount == 0.

rs34 packet(@Next, SrcAdd, DstAdd, Data, NewHashList) :-
 ePacket(@Next, SrcAdd, DstAdd, Data, RID, RLoc, HashList),
 Hash := f_append(RID),
 NewHashList := f_concat(HashList, Hash).


/*No hit in the routing table, follow the default routing*/
rs40 ePacketTemp(@RLoc, Next, SrcAdd, DstAdd, Data, RID, R, List, HashList) :-
        device(@Switch, Dvtype),
        matchingPacket(@Switch, SrcAdd, DstAdd, Data, Priority, HashList),
 flowEntry(@Switch, DstAdd, Next, Priority),
 link(@Switch, Next),
 Priority == 0,
        Dvtype == 0,
 RLoc := Switch,
 R := "rs4",
 PID1 := f_sha1("device" + Switch + Dvtype),
 PID2 := f_sha1("flowEntry" + Switch + DstAdd + Next + Priority),
 PID3 := f_sha1("link" + Switch + Next),
 List := f_append(PID1),
 List2 := f_append(PID2),
 List := f_concat(List, List2),
 List3 := f_append(PID3),
 List := f_concat(List, List3),
 RID := f_sha1(R + List).

/* rs41 - rs44 are identical to rs31 - rs34*/

/*Host program*/
/* TODO: Separate the generation of PIDev and HashList, because RID could be conpressed*/
rh10 ePacketTemp(@RLoc, Switch, SrcAdd, DstAdd, Data, RID, R, List, HashList) :-
        device(@Host, Dvtype),
        initPacket(@Host, SrcAdd, DstAdd, Data),
 linkhr(@Host, Switch),
        Dvtype == 1,
 RLoc := Host,
 R := "rh1",
 PIDev := f_sha1("initPacket" + Host + SrcAdd + DstAdd + Data),
 PID1 := f_sha1("device" + Host + Dvtype),
 PID2 := f_sha1("linkhr" + Host + Switch),
 List := f_append(PID1),
 List2 := f_append(PID2),
 List := f_concat(List, List2),
 RID := f_sha1(R + List),
 HashList := f_append(PIDev).

/* rh11 - rh14 are identical to rs21 - rs24*/

/*Receive a packet*/
rh20 eRecvPacketTemp(@RLoc, Host, SrcAdd, DstAdd, Data, RID, R, List, HashList) :-
 device(@Host, Dvtype),
 packet(@Host, SrcAdd, DstAdd, Data, HashList),
 DstAdd == Host,
        Dvtype == 1,
 RLoc := Host,
 R := "rh2",
 PID1 := f_sha1("device" + Host + Dvtype),
 List := f_append(PID1),
 RID := f_sha1(R + List).

rh21 eRecvPacket(@Host, SrcAdd, DstAdd, Data, RID, RLoc, HashList) :-
    eRecvPacketTemp(@RLoc, Host, SrcAdd, DstAdd, Data, RID, R, List, HashList).

rh22 eRecvPacketCount(@RLoc, RID, R, List, a_COUNT<*>) :-
 eRecvPacketTemp(@RLoc, Host, SrcAdd, DstAdd, Data, RID, R, List, HashList),
 ruleExec(@RLoc, RID, R, List).

rs23 ruleExec(@RLoc, RID, R, List) :-
 eRecvPacketCount(@RLoc, RID, R, List, Rcount),
 Rcount == 0.

rh24 recvPacket(@Host, SrcAdd, DstAdd, Data, NewHashList) :-
 eRecvPacket(@Host, SrcAdd, DstAdd, Data, RID, RLoc, HashList),
 Hash := f_append(RID),
 NewHashList := f_concat(HashList, Hash).

rh25 recvAuxPkt(@Host, SrcAdd, Data, NewHashList) :-
        recvPacket(@Host, SrcAdd, DstAdd, Data, NewHashList).
