#include "wavedata.h"
#include <limits>

WaveData::WaveData(std::string filepath) {
    // For now, this code generates summaries of size sampleCount/(2^n)
    // where n is the summary number. It does this until that expression
    // is less than or equal to 1, at which point it stops. It might make
    // sense to instead do sampleCount/(5 * 2^n), or some other constant
    // to reduce the memory usage. This (the constant) should be tweaked
    // to give the lowest possible memory footprint while avoiding any
    // noticible inaccuracies or artifacts.

    AudioFile<double> audioFile = AudioFile<double>();

    audioFile.load(filepath);

    this->summaries = QList<WaveSummary>();

    int reductionFactor = 5;

    summaries.push_back(WaveSummary(&audioFile, static_cast<unsigned int>(reductionFactor)));

    int currentIndex = 0;
    while (true) {
        summaries.push_back(WaveSummary(&summaries[currentIndex], reductionFactor));
        currentIndex++;
        if (summaries[currentIndex].m_packets[0].length() <= 2) {
            break;
        }
    }
}





WaveSummary::WaveSummary(AudioFile<double>* audioFile, unsigned int samplesPerPacket) {
    m_packets = QList<QList<Packet>>();

    auto channelCount = audioFile->getNumChannels();

    for (int channel = 0; channel < channelCount; channel++) {
        QList<Packet> channelPackets = QList<Packet>();
        double min = std::numeric_limits<double>::max();
        double max = -min;

        auto samplesPerChannel = audioFile->getNumSamplesPerChannel();

        for (unsigned int sampleNum = 0; sampleNum < samplesPerChannel; sampleNum++) {
            double sample = audioFile->samples[static_cast<unsigned long long>(channel)][sampleNum];
            if (sample > max) {
                max = sample;
            }

            if (sample < min) {
                min = sample;
            }

            if ((sampleNum + 1) % samplesPerPacket == 0) {
                Packet packet;
                packet.max = max;
                packet.min = min;
                channelPackets.push_back(packet);

                min = std::numeric_limits<double>::max();
                max = -min;
            }
        }

        if (max != std::numeric_limits<double>::max()) {
            Packet packet;
            packet.max = max;
            packet.min = min;
            channelPackets.push_back(packet);
        }

        m_packets.push_back(channelPackets);
    }
}

WaveSummary::WaveSummary(WaveSummary* summary, int packetsPerPacket) {
    m_packets = QList<QList<Packet>>();

    for (int channel = 0; channel < summary->m_packets.size(); channel++) {
        QList<Packet> channelPackets = QList<Packet>();
        double min = std::numeric_limits<double>::max();
        double max = -min;

        for (int packetNum = 0; packetNum < summary->m_packets[0].size(); packetNum++) {
            double packetMin = summary->m_packets[channel][packetNum].min;
            double packetMax = summary->m_packets[channel][packetNum].max;
            if (packetMax > max) {
                max = packetMax;
            }

            if (packetMin < min) {
                min = packetMin;
            }

            if ((packetNum + 1) % packetsPerPacket == 0) {
                Packet packet;
                packet.max = max;
                packet.min = min;
                channelPackets.push_back(packet);

                min = std::numeric_limits<double>::max();
                max = -min;
            }
        }

        // Insert remaining incomplete packet if it exists
        if (max != std::numeric_limits<double>::max()) {
            Packet packet;
            packet.max = max;
            packet.min = min;
            channelPackets.push_back(packet);
        }

        m_packets.push_back(channelPackets);
    }
}
