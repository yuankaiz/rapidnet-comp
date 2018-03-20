from collections import defaultdict
import os
storageinfo = os.stat('1')
print "Provenance tables size: " + str(storageinfo.st_size) + " bytes"

class Record(object):
    def __init__(self, textline):
        self.isImage = True if 'image' in textline else False
        self.isResult = not self.isImage
        self.timing = int(textline.split()[-1])
        self.iid = int(textline[textline.find(':')+1:textline.find(')')])

with open('timing', 'r') as f:
    lines = [line.rstrip('\n') for line in f]

iData = {}
rData = defaultdict(list)
# iid: [records of iid]
output = {}
# iid: avg time took for each image

for l in lines:
    record = Record(l)
    if record.isImage:
        iData[record.iid] = record
    else:
        rData[record.iid].append(record)

for i, r in iData.iteritems():
    n = total = 0
    for res in rData[i]:
        n += 1
        total += res.timing - r.timing
    output[i] = total/n

for i, o in output.iteritems():
    print 'image ' + str(i) + ' has ' + str(len(rData[i]))  + ' results, took average ' + str(o) + ' microseconds'
