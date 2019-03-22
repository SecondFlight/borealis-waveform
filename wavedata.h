#ifndef WAVEDATA_H
#define WAVEDATA_H

#include <string>
#include "AudioFile/AudioFile.h"

#include <QList>

struct Packet {
    float max;
    float min;
};

class WaveSummary {
public:
    // Each QList<Packet> is one channel.
    QList<QList<Packet>> m_packets;
    WaveSummary(WaveSummary* summary, int packetsPerPacket);
    WaveSummary(AudioFile<float>* audioFile, unsigned int samplesPerPacket);
private:
};


class WaveData {
public:
    // List of summaries, ordered from most to least specific.
    QList<WaveSummary> summaries;
    WaveData(std::string filepath);
private:
};

#endif // WAVEDATA_H
