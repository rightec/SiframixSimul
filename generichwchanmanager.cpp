#include "generichwchanmanager.h"

GenericHwChanManager::GenericHwChanManager()
{

}

long GenericHwChanManager::getOffset(int i)
//short getOffset(int i)
{
    if (i < 0 || i >= 2)
    {
        return 0;
    }
    return m_hwChan[i];
};

void GenericHwChanManager::setOffset(int _chan, long _offset)
{
    m_hwChan[_chan] = _offset;
}

void GenericHwChanManager::setGain(int i, double gain)
{
    if (i < 0 || i >= 2)
    {
        return;
    }
    m_hwChanGain[i] = gain;
};
