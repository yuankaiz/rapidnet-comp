/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(linkhr,infinity,infinity,keys(2)). /*Links between hosts and routers*/
materialize(link,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(device,infinity,infinity,keys(2:int32)). /*Identify the device type of the node*/




/*Switch program*/
/*A hit in the routing table, forward the packet accordingly*/
rs1 packet(@Next, SrcAdd, DstAdd, Data) :-
        device(@Switch, Dvtype),
        packet(@Switch, SrcAdd, DstAdd, Data),
 flowEntry(@Switch, DstAdd, Next),
 link(@Switch, Next),
        Dvtype == 0 .

/*Host program*/
rh1 packet(@Switch, SrcAdd, DstAdd, Data) :-
        device(@Host, Dvtype),
 initPacket(@Host, SrcAdd, DstAdd, Data),
 linkhr(@Host, Switch),
        Dvtype == 1 .

/*Receive a packet*/
rh2 recvPacket(@Host, SrcAdd, DstAdd, Data) :-
        device(@Host, Dvtype),
 packet(@Host, SrcAdd, DstAdd, Data),
 DstAdd == Host,
        Dvtype == 1 .
