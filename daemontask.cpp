#include "daemontask.h"

extern string currentdoing;
extern float totalwork;
extern float workdone;

DaemonTask::DaemonTask()
{

}
DaemonTask::~DaemonTask()
{

}

void DaemonTask::run(){
    DaemonWorker worker;
    GenusCollection collection = worker.LoadTargetGenus(config.sourceGenus,config.targetGenus);
    SimpleGenusCollection x = worker.LoadSimpleSourceGenus(config.sourceGenus);
    CommonFragments = worker.GetCommonFragmentFromGenus(collection,x,config.fragmentCoverage);
    genusPrimerPairRegular = worker.MakePair(collection,CommonFragments,config.productLengthBottom,config.productLengthTop);
    genusPrimerPairRegular = worker.FilterFragment(genusPrimerPairRegular);
    emit signal_complete_proccess("OK");

}


//进度条
//进程每1秒执行一次本函数
void process(){
    int pct;
    //文本框 = currentdoing;
    if(totalwork != 0){
        pct = workdone / totalwork;
        //显示进度pct
    }
    else{
        //显示进度缺省或正在计算中
    }
}
