#!/usr/bin/python                                                                                                                                                                                                                 

#nRUN = 245192
#nRUN = "MinBias"
## change run number

import os
currentDir = os.getcwd()
print 'currentDir = %s' %(currentDir)

##settings
nRUN = "268955"
print 'nRUN %s  >>>> DID YOU UPDATE?' %(nRUN)

subDir = currentDir+"/JOB_%s" %(nRUN)
print 'subDir = %s  >>>> DID YOU CREATE THIS?' %(subDir)

outFolder = "/store/user/amartell/Commissioning2016_EStiming/xTiming_%s" %(nRUN)
print 'outFolder = %s' %(outFolder)

##prepare list with eos path of .root
## example ll ~/eos/cms//store/user/amartell/Commissioning2016_EStiming/ES_run268955/ | awk '{print "root://eoscms//eos/cms/store/user/amartell/Commissioning2016_EStiming/ES_run268955/"$9""}' > listOfNtuplesFiles_268955.txt
inFileList = currentDir+"/listOfNtuplesFiles_%s.txt" %(nRUN)
print 'inFileList = %s' %(inFileList)

outLancia = open('%s/lancia_%s.sh' %(currentDir, nRUN),'w');

num = 0
plusNum = 5
while num < 60:   ## little bigger that number of files
#for num in range(0,2000):  #to iterate between 10 to 20  
    outScript = open('%s/bjob_%d.sh' %(subDir, num),'w');
    outScript.write('#!/bin/bash \n');
    outScript.write('cd %s \n' %(subDir));
    outScript.write('export SCRAM_ARCH=slc6_amd64_gcc493 \n');
    outScript.write('eval `scramv1 ru -sh` \n');
    #outScript.write('cd - \n');
    outScript.write('cmsMkdir %s \n' %(outFolder));
    outScript.write('pwd \n ')
    outScript.write('./../xTimingSensor_Add_temp  %d %d %s \n' %(num, num+plusNum, inFileList) );
    outScript.write('cmsStage coll_timing_%d_%d.root %s/' %(num, num+plusNum, outFolder));
    outScript.write( '\n ' );
    os.system('chmod 777 %s/bjob_%d.sh' %(subDir, num));
    outLancia.write(' bsub -cwd %s -q cmscaf1nd %s/bjob_%d.sh \n' %(subDir, subDir, num));
    num += plusNum


