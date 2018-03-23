from collections import defaultdict
import os

cumu = {}
# cumulative storage for provenance tables
storage = {}
# iid : provenance table size for iid

for i in range(1, 13):
    cumusize = os.stat('prov/storage/image' + str(i) + '/1').st_size
    cumu[i] = cumusize

for i in range(1, 13):
    storage[i] = cumu[i] - (cumu[i-1] if i > 1 else 0)
    print 'image', i, 'provenance tables size:', storage[i], 'bytes'

class Record(object):
    def __init__(self, textline):
        self.isImage = True if 'image' in textline else False
        self.isResult = not self.isImage
        self.timing = int(textline.split()[-1])
        self.iid = int(textline[textline.find(':')+1:textline.find(')')])

proviData = {}
provrData = defaultdict(list)
# iid: [records of iid]
provoutput = {}
# iid: avg time took for each image

mliData = {}
mlrData = defaultdict(list)
# iid: [records of iid]
mloutput = {}
# iid: avg time took for each image

def timing_analysis(timing_file, iData, rData, output):
    with open(timing_file, 'r') as f:
        lines = [line.rstrip('\n') for line in f]

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
        output[i] = total*1.0/n

timing_analysis("prov/timing", proviData, provrData, provoutput)
timing_analysis("ml/timing", mliData, mlrData, mloutput)

print '--------------------------------'
print 'image processing latency (in microseconds)'
for i, o in provoutput.iteritems():
    ptime = o
    mtime = mloutput[i]
    num = len(mlrData[i])
    increase = (ptime - mtime) * 1.0 / mtime
    print 'image' , i, 'has', num, 'results', 'ml-only:' , "{0:.1f}".format(mtime) , ', prov:' , "{0:.1f}".format(ptime) ,
    print ', overhead:' , "{0:.2f}".format(increase*100) , '%'
