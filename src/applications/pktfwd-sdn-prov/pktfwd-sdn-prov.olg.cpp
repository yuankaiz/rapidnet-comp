/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(linkhr,infinity,infinity,keys(2)). /*Links between hosts and routers*/
materialize(link,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(flowEntry,infinity,infinity,keys(2,4:int32)). /*Flow tables of switches*/
materialize(maxPriority,infinity,infinity,keys(2:int32)). /*Determine the maximum priority of the flow table*/
materialize(device,infinity,infinity,keys(2:int32)). /*Identify the device type of the node*/

/* Provenance tables*/
//materialize(ruleExec,infinity,infinity,keys(2:str)). /*Record the rule execution*/
//materialize(prov,infinity,infinity,keys(2:str,3:str)). /*Record the tuple generation*/




/* Edb provenance rules*/
r00 prov(@Host, VID, RID, Host) :-
    initPacket(@Host, SrcAdd, DstAdd, Data),
    VID := f_sha1("initPacket" + Host + SrcAdd + DstAdd + Data),
    RID := VID.

r01 prov(@Device, VID, RID, Device) :-
    device(@Device, Dvtype),
    VID := f_sha1("device" + Device + Dvtype),
    RID := VID.

r02 prov(@Switch, VID, RID, Switch) :-
    maxPriority(@Switch, TopPriority),
    VID := f_sha1("maxPriority" + Switch + TopPriority),
    RID := VID.

r03 prov(@Switch, VID, RID, Switch) :-
    flowEntry(@Switch, DstEntry, Next, Priority),
    VID := f_sha1("Switch" + DstEntry + Next + Priority),
    RID := VID.

/* Rewritten provenance-enabled rules*/
/*Switch program*/
/*Query the controller when receiving unknown packets */
rs10 eMatchingPacketTemp(@RLoc, Switch, SrcAdd, DstAdd, Data, TopPriority, RID, R, List) :-
    device(@Switch, Dvtype),
    packet(@Switch, SrcAdd, DstAdd, Data),
    maxPriority(@Switch, TopPriority),
    Dvtype == 0,
    RLoc := Switch,
    R := "rs1",
    PID1 := f_sha1("device" + Switch + Dvtype),
    PID2 := f_sha1("packet" + Switch + SrcAdd + DstAdd + Data),
    PID3 := f_sha1("maxPriority" + Switch + TopPriority),
    List := f_append(PID1),
    List2 := f_append(PID2),
    List := f_concat(List, List2),
    List3 := f_append(PID3),
    List := f_concat(List, List3),
    RID := f_sha1(R + RLoc + List).

rs11 eMatchingPacket(@Switch, SrcAdd, DstAdd, Data, TopPriority, RID, RLoc) :-
    eMatchingPacketTemp(@RLoc, Switch, SrcAdd, DstAdd, Data, TopPriority, RID, R, List).

rs12 ruleExec(@RLoc, RID, R, List) :-
    eMatchingPacketTemp(@RLoc, Switch, SrcAdd, DstAdd, Data, TopPriority, RID, R, List).

rs13 matchingPacket(@Switch, SrcAdd, DstAdd, Data, TopPriority) :-
    eMatchingPacket(@Switch, SrcAdd, DstAdd, Data, TopPriority, RID, RLoc).

rs14 prov(@Switch, VID, RID, RLoc) :-
    eMatchingPacket(@Switch, SrcAdd, DstAdd, Data, TopPriority, RID, RLoc),
    VID := f_sha1("matchingPacket" + Switch + SrcAdd + DstAdd + Data + TopPriority).

/*Recursively matching entries in a routing table*/
rs20 eMatchingPacketTemp(@RLoc, Switch, SrcAdd, DstAdd, Data, NextPriority, RID, R, List) :-
    device(@Switch, Dvtype),
    matchingPacket(@Switch, SrcAdd, DstAdd, Data, Priority),
    flowEntry(@Switch, DstEntry, Next, Priority),
    Priority > 0,
    DstAdd != DstEntry,
    NextPriority := Priority - 1,
    Dvtype == 0,
    RLoc := Switch,
    R := "rs2",
    PID1 := f_sha1("device" + Switch + Dvtype),
    PID2 := f_sha1("matchingPacket" + Switch + SrcAdd + DstAdd + Data + Priority),
    PID3 := f_sha1("flowEntry" + Switch + DstEntry + Next + Priority),
    List := f_append(PID1),
    List2 := f_append(PID2),
    List := f_concat(List, List2),
    List3 := f_append(PID3),
    List := f_concat(List, List3),
    RID := f_sha1(R + RLoc + List).

/* rs21 - rs24 are identical to rs11 - rs14*/


/*A hit in the routing table, forward the packet accordingly*/
rs30 ePacketTemp(@RLoc, Next, SrcAdd, DstAdd, Data, RID, R, List) :-
        device(@Switch, Dvtype),
        matchingPacket(@Switch, SrcAdd, DstAdd, Data, Priority),
 flowEntry(@Switch, DstEntry, Next, Priority),
 link(@Switch, Next),
 Priority > 0,
 DstAdd == DstEntry,
        Dvtype == 0,
 RLoc := Switch,
 R := "rs3",
 PID1 := f_sha1("device" + Switch + Dvtype),
 PID2 := f_sha1("matchingPacket" + Switch + SrcAdd + DstAdd + Data + Priority),
 PID3 := f_sha1("flowEntry" + Switch + DstEntry + Next + Priority),
 PID4 := f_sha1("link" + Switch + Next),
 List := f_append(PID1),
 List2 := f_append(PID2),
 List := f_concat(List, List2),
 List3 := f_append(PID3),
 List := f_concat(List, List3),
 List4 := f_append(PID4),
 List := f_concat(List, List4),
 RID := f_sha1(R + RLoc + List).

rs31 ePacket(@Next, SrcAdd, DstAdd, Data, RID, RLoc) :-
 ePacketTemp(@RLoc, Next, SrcAdd, DstAdd, Data, RID, R, List).

fs32 ruleExec(@RLoc, RID, R, List) :-
 ePacketTemp(@RLoc, Next, SrcAdd, DstAdd, Data, RID, R, List).

rs33 packet(@Next, SrcAdd, DstAdd, Data) :-
 ePacket(@Next, SrcAdd, DstAdd, Data, RID, RLoc).

rs34 prov(@Next, VID, RID, RLoc) :-
 ePacket(@Next, SrcAdd, DstAdd, Data, RID, RLoc),
 VID := f_sha1("packet" + Next + SrcAdd + DstAdd + Data).

/*No hit in the routing table, follow the default routing*/
rs40 ePacketTemp(@RLoc, Next, SrcAdd, DstAdd, Data, RID, R, List) :-
        device(@Switch, Dvtype),
        matchingPacket(@Switch, SrcAdd, DstAdd, Data, Priority),
 flowEntry(@Switch, DstAdd, Next, Priority),
 link(@Switch, Next),
 Priority == 0,
        Dvtype == 0,
 RLoc := Switch,
 R := "rs4",
 PID1 := f_sha1("device" + Switch + Dvtype),
 PID2 := f_sha1("matchingPacket" + Switch + SrcAdd + DstAdd + Data + Priority),
 PID3 := f_sha1("flowEntry" + Switch + DstAdd + Next + Priority),
 PID4 := f_sha1("link" + Switch + Next),
 List := f_append(PID1),
 List2 := f_append(PID2),
 List := f_concat(List, List2),
 List3 := f_append(PID3),
 List := f_concat(List, List3),
 List4 := f_append(PID4),
 List := f_concat(List, List4),
 RID := f_sha1(R + RLoc + List).

/* rs41 - rs44 are identical to rs31 - rs34*/

/*Host program*/
rh10 ePacketTemp(@RLoc, Switch, SrcAdd, DstAdd, Data, RID, R, List) :-
        device(@Host, Dvtype),
        initPacket(@Host, SrcAdd, DstAdd, Data),
 linkhr(@Host, Switch),
        Dvtype == 1,
 RLoc := Host,
 R := "rh1",
 PID1 := f_sha1("device" + Host + Dvtype),
 PID2 := f_sha1("initPacket" + Host + SrcAdd + DstAdd + Data),
 PID3 := f_sha1("linkhr" + Host + Switch),
 List := f_append(PID1),
 List2 := f_append(PID2),
 List := f_concat(List, List2),
 List3 := f_append(PID3),
 List := f_concat(List, List3),
 RID := f_sha1(R + RLoc + List).

/* rh11 - rh14 are identical to rs21 - rs24*/

/*Receive a packet*/
rh20 eRecvPacketTemp(@RLoc, Host, SrcAdd, DstAdd, Data, RID, R, List) :-
 device(@Host, Dvtype),
 packet(@Host, SrcAdd, DstAdd, Data),
        Dvtype == 1,
 RLoc := Host,
 R := "rh2",
 PID1 := f_sha1("device" + Host + Dvtype),
 PID2 := f_sha1("packet" + Host + SrcAdd + DstAdd + Data),
 List := f_append(PID1),
 List2 := f_append(PID2),
 List := f_concat(List, List2),
 RID := f_sha1(R + RLoc + List).

rh21 eRecvPacket(@Host, SrcAdd, DstAdd, Data, RID, RLoc) :-
    eRecvPacketTemp(@RLoc, Host, SrcAdd, DstAdd, Data, RID, R, List).

rh22 ruleExec(@RLoc, RID, R, List) :-
    eRecvPacketTemp(@RLoc, Host, SrcAdd, DstAdd, Data, RID, R, List).

rh23 recvPacket(@Host, SrcAdd, DstAdd, Data) :-
    eRecvPacket(@Host, SrcAdd, DstAdd, Data, RID, RLoc).

rh24 prov(@Host, VID, RID, RLoc) :-
    eRecvPacket(@Host, SrcAdd, DstAdd, Data, RID, RLoc),
    VID := f_sha1("recvPacket" + Host + SrcAdd + DstAdd + Data).
