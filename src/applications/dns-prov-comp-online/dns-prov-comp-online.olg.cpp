materialize(url,infinity,infinity,keys(1,2:str,3,4:int32)).
materialize(address_record,infinity,infinity,keys(1,2:str,3)).
materialize(name_server,infinity,infinity,keys(1,2:str,3:str)).
materialize(results,infinity,infinity,keys(1,2:str,3,4:int32)).


/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(4:list)).
materialize(equiHashTable,infinity,infinity,keys(2:str)). /*Hash table for equivalence detection*/
materialize(provHashTable,infinity,infinity,keys(3:list)). /*Hash table for provenance query*/
materialize(resultsAux, infinity, infinity, keys(2:cid)).


/*Program for initialization of rule table*/
prov_r1_1 eRequestTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List,Tag) :-
  requestProv(@X,URL,HOST,REQUESTID,Tag),
    name_server(@X,DOMAIN,SERVER),
    address_record(@X,SERVER,ADDR),
    f_indexof(URL,DOMAIN)!=-1,
    URL!=DOMAIN,
    List := f_empty(),
    PID2 := f_sha1(("name_server"+X)+DOMAIN+SERVER),
    List2 := f_append(PID2),
    List := f_concat(List,List2),
    PID3 := f_sha1(("address_record"+X)+SERVER+ADDR),
    List3 := f_append(PID3),
    List := f_concat(List,List3),
    RLOC := X,
    R := "r2",
    RID := f_sha1((R+RLOC)+List).

prov_r1_2 eRequestCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    eRequestTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List,Tag),
    ruleExec(@RLOC, RID, R, List).

prov_r1_3 ruleExec(@RLOC, RID, R, List) :-
    eRequestCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

prov_r1_4 requestProv(@ADDR,URL,HOST,REQUESTID,NewTag) :-
    eRequestTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List,Tag),
    Hash := f_append(RID),
    NewTag := f_concat(Tag, Hash).

r1 request(@ADDR,URL,HOST,REQUESTID,PIDHash) :-
       request(@X,URL,HOST,REQUESTID,PIDHash),
       name_server(@X,DOMAIN,SERVER),
          address_record(@X,SERVER,ADDR),f_indexof(URL,DOMAIN)!=-1,
       URL!=DOMAIN.

/*Hash on equivalence attributes*/
r100 urlHash(@X,URL,HOST,REQUESTID,PIDequi) :-
   url(@X,URL,HOST,REQUESTID), PIDequi := f_sha1(""+X+URL).

r101 urlCount(@X,URL,HOST,REQUESTID,PIDequi,a_COUNT<*>) :-
   urlHash(@X,URL,HOST,REQUESTID,PIDequi),
   equiHashTable(@X,URL,PIDequi).

r102 equiHashTable(@X,URL,PIDequi) :-
   urlCount(@X,URL,HOST,REQUESTID,PIDequi,PIDCount),
   PIDCount == 0.

r103 request(@X,URL,HOST,REQUESTID,PIDHash) :-
     urlCount(@X,URL,HOST,REQUESTID,PIDequi,PIDCount),
  PIDCount != 0,
  PIDev := f_sha1("url"+X+URL+HOST+REQUESTID),
  PIDequiHash := f_append(PIDequi),
      PIDevHash := f_append(PIDev),
      PIDHash := f_concat(PIDequiHash, PIDevHash).

prov_r2_1 eRequestTemp(@RLOC,X,URL,HOST,REQUESTID,RID,R,List,Tag) :-
  urlCount(@X,URL,HOST,REQUESTID,PIDequi,PIDCount),
  PIDCount == 0,
  PIDev := f_sha1("url"+X+URL+HOST+REQUESTID),
  Equilist := f_append(PIDequi),
            Evlist := f_append(PIDev),
            Tag := f_concat(Equilist, Evlist),
  List := f_empty(),
    RLOC := X,
    R := "r1",
    RID := f_sha1((R+RLOC)+List).


prov_r3_1 eresultsTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List,Tag) :-
  requestProv(@X,URL,HOST,REQUESTID,Tag),
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

prov_r3_2 eRequestCount(@RLOC, RID, R, List, a_COUNT<*>) :-
  eresultsTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List,Tag),
  ruleExec(@RLOC, RID, R, List).

prov_r3_3 ruleExec(@RLOC, RID, R, List) :-
     eRequestCount(@RLOC, RID, R, List, Rcount),
     Rcount == 0.

prov_r3_4 eresults(@ADDR, URL,HOST,REQUESTID, RID, RLOC, Tag) :-
  eresultsTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List,Tag).

prov_r3_5 resultsTemp(@ADDR,URL,HOST,REQUESTID,Tag) :-
    eresults(@ADDR,URL,HOST,REQUESTID,RID, RLOC, Tag),
           Hash := f_append(RID),
           NewTag := f_concat(Tag, Hash).

prov_r3_6 resultsDecomp(@ADDR,URL,HOST,REQUESTID,PIDequi, PIDev, Hashlist) :-
    resultsTemp(@ADDR,URL,HOST,REQUESTID,Tag),
    PIDequi := f_first(Tag),
    Remainlist := f_removeFirst(Tag),
    PIDev := f_first(Remainlist),
    Hashlist := f_removeFirst(Remainlist).

prov_r3_7 provHashTable(@ADDR, PIDequi, Hashlist) :-
   resultsDecomp(@ADDR,URL,HOST,REQUESTID,PIDequi, PIDev, Hashlist).

prov_r3_8 results(@ADDR, URL, HOST,REQUESTID,PIDequi, PIDev) :-
   resultsDecomp(@ADDR, URL, HOST,REQUESTID,PIDequi, PIDev, Hashlist).

r3 results(@ADDR, URL, HOST,REQUESTID,PIDequi, PIDev) :-
 request(@X,URL,HOST,REQUESTID,PIDHash),
 name_server(@X,DOMAIN,SERVER),
 address_record(@X,SERVER,ADDR),
 f_indexof(URL,DOMAIN)!=-1,
 URL==DOMAIN,
 PIDequi := f_first(PIDHash),
 PIDevHash := f_removeFirst(PIDHash),
 PIDev := f_first(PIDevHash).

r4 resultsAux(@ADDR, PID, PIDequi, PIDev) :-
        results(@ADDR, URL,HOST, REQUESTID,PIDequi, PIDev),
        PID := f_sha1("results" + ADDR + URL + HOST+REQUESTID).
