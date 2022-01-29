#ifndef GENERICHWCHANMANAGER_H
#define GENERICHWCHANMANAGER_H


class GenericHwChanManager
{
public:
    GenericHwChanManager();
    long getOffset(int i);
    void setOffset(int _chan, long _offset);
    void setGain(int i, double gain);

private:
    long m_hwChan[2];
    double m_hwChanGain[2];
};

#endif // GENERICHWCHANMANAGER_H
