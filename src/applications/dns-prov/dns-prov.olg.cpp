materialize(url,infinity,infinity,keys(1,2:str,3,4:int32)).
materialize(address_record,infinity,infinity,keys(1,2:str,3)).
materialize(name_server,infinity,infinity,keys(1,2:str,3:str)).
materialize(result,infinity,infinity,keys(1,2:str,3,4:int32)).

/*Provenance table*/
materialize(ruleExec,infinity,infinity,keys(4:list)).
materialize(prov,infinity,infinity,keys(1,2:cid,3:cid,4)).

/*Edb provenanace rules*/

r00 prov(@REQUESTLOC,VID,RID,REQUESTLOC) :-
    url(@REQUESTLOC,URL,HOST,REQUESTID),
    VID := f_sha1("initURL"+REQUESTLOC+URL+HOST+REQUESTID),
    RID := VID.

/*Request Program*/
prov_r1_1 eRequestTemp(@RLOC,X,URL,HOST,REQUESTID,RID,R,List) :-
    url(@X,URL,HOST,REQUESTID),
    PID1 := f_sha1(("url"+X)+URL+HOST+REQUESTID),
    List := f_append(PID1),
    RLOC := X,
    R := "rs1",
    RID := f_sha1((R+RLOC)+List).

prov_r1_2 ruleExec(@RLOC, RID, R, List) :-
    eRequestTemp(@RLOC,X,URL,HOST,REQUESTID,RID,R,List).

prov_r1_3 eRequest(@X,URL,HOST,REQUESTID,RID,RLOC):-
    eRequestTemp(@RLOC,X,URL,HOST,REQUESTID,RID,R,List).

prov_r1_4 request(@X,URL,HOST,REQUESTID) :-
    eRequest(@X,URL,HOST,REQUESTID,RID,RLOC).

prov_r1_5 prov(@X,VID,RID,RLOC):-
    eRequest(@X,URL,HOST,REQUESTID,RID,RLOC),
    VID := f_sha1(("request"+X)+URL+HOST+REQUESTID).


/*Request rule r2*/
prov_r2_1 eRequestTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List) :-
   request(@X,URL,HOST,REQUESTID),
   name_server(@X,DOMAIN,SERVER),
   address_record(@X,SERVER,ADDR),
   f_indexof(URL,DOMAIN)!=-1,
   URL!=DOMAIN,
   PID1 := f_sha1(("request"+X)+URL+HOST+REQUESTID),
   List := f_append(PID1),
   PID2 := f_sha1(("name_server"+X)+DOMAIN+SERVER),
   List2 := f_append(PID2),
   List := f_concat(List,List2),
   PID3 := f_sha1(("address_record"+X)+SERVER+ADDR),
   List3 := f_append(PID3),
   List := f_concat(List,List3),
   RLOC := X,
   R := "r2",
   RID := f_sha1((R+RLOC)+List).

/*Result Program r3*/
prov_r3_1 eResultTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List) :-
   request(@X,URL,HOST,REQUESTID),
   name_server(@X,DOMAIN,SERVER),
   address_record(@X,SERVER,ADDR),
   f_indexof(URL,DOMAIN)!=-1,
   URL==DOMAIN,
   PID1:= f_sha1(("request"+X)+URL+HOST+REQUESTID),
   List := f_append(PID1),
   PID2 := f_sha1(("name_server"+X)+DOMAIN+SERVER),
   List2 := f_append(PID2),
   List := f_concat(List,List2),
   PID3 := f_sha1(("address_record"+X)+SERVER+ADDR),
   List3 := f_append(PID3),
   List := f_concat(List,List3),
   RLOC := X,
   R :="r3",
   RID := f_sha1((R+RLOC)+List).

prov_r3_2 ruleExec(@RLOC,RID,R,List):-
   eResultTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List).

prov_r3_3 eResult(@ADDR,URL,HOST,REQUESTID,RID,RLOC):-
   eResultTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List).

prov_r3_4 result(@ADDR,URL,HOST,REQUESTID) :-
   eResult(@ADDR,URL,HOST,REQUESTID,RID,RLOC).

prov_r3_5 prov(@ADDR,VID,RID,RLOC):-
   eResult(@ADDR,URL,HOST,REQUESTID,RID,RLOC),
   VID := f_sha1(("result"+ADDR)+URL+HOST+REQUESTID).
