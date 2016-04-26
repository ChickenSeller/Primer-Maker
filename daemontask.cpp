#include "daemontask.h"


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
