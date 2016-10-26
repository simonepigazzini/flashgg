import FWCore.ParameterSet.Config as cms

import sys
from math import sqrt

def getEtaR9(cat):
    cat = cat.lower()
    if 'eb' in cat:
        etaMin,etaMax = 0.,1.5
    elif 'ee' in cat:
        etaMin,etaMax = 1.5,6. # before preselection there are photon candidated above 3 in some cases (should be removed by diphoton producer?)
    else:
        etaMin = cat.split("abseta_",1)[1].split('_')[0]
        etaMax = cat.split("abseta_",1)[1].split('_')[1].split('-',1)[0]

    if etaMax == 2.5: etaMax = 6. # before preselection there are photon candidated above 3 in some cases (should be removed by diphoton producer?)
    if etaMax == 1.4442: etaMax = 1.5
    if etaMin == 1.566: etaMin = 1.5

    r9Min = -999.
    r9Max = 999.
    if 'highr9' in cat or 'gold' in cat:
        r9Min = 0.94
    elif 'lowr9'  in cat or 'bad' in cat:
        r9Max = 0.94

    return (etaMin,etaMax),(r9Min,r9Max)

def getGain(cat):
    if( len(cat.split("gainEle_",1)) == 1 ): return (0,13) #if len is 1 => no gain cat
    gainFlag = int(cat.split("gainEle_",1)[1].split('-',1)[0])
    return (gainFlag,gainFlag) #min=max 

def getEt(cat):
    if( len(cat.split("Et_",1)) == 1 ): return (0,50000) #if len is 1 => no Et cat
    EtMin = cat.split("Et_",1)[1].split('_')[0]
    EtMax = cat.split("Et_",1)[1].split('_')[1].split('-',1)[0]
    return (EtMin,EtMax)
    
def readEcalElfRunDep(line):
    toks = filter(lambda x: x!='', line.replace('\t',' ').split(' ') )
    if( len(toks) == 0 ): return None
    #toks[0] is the category name
    eta,r9 = getEtaR9(toks[0])
    gain   = getGain(toks[0])
    et     = getEt(toks[0])
    run = map(int, toks[2:4])
    run[1] += 1 # ECALELF boundaries are [], while flashggg ones [)
    
    lowB,upB = zip(run,eta,r9,et,gain) #

    val = float( toks[4] )-1.
    errs = map(float,toks[5:8]) 
    stat = 0.5*(errs[0]+errs[1])
    syst = errs[2]
    err = sqrt(stat**2 + syst**2)

    ret =  cms.PSet( lowBounds=cms.vdouble(lowB), upBounds=cms.vdouble(upB), values=cms.vdouble(val), uncertainties=cms.vdouble(err) ) 
    ## print line, ret
    return ret


def getRunDependentScaleBins(fname):
    
    import os
    search_path = os.environ['CMSSW_SEARCH_PATH']
    
    if os.path.exists(fname):
        iname = fname
    else:
        matches = filter(lambda x: os.path.exists(x), map(lambda x: os.path.join(x,fname),  search_path.split(':')))
        if len(matches) == 0:
            raise Exception('Could not find ECALELF dat file %s in path' % fname)
        iname = matches[0]
        
    with open(iname,'r') as fin:
        shifts = filter(lambda x: x!=None, map(readEcalElfRunDep, fin.read().split("\n") ))
        
        scaleBins = cms.PSet(
            variables = cms.vstring("global.run","abs(superCluster.eta)","r9","et","gain"),#gain?
            bins = cms.VPSet(shifts)
            )

        return scaleBins
    
    raise Exception('Could not open ECALELF dat file %s ' % fname)

