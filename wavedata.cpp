#include "wavedata.h"
#include <limits>
#include <QDebug>

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

Packet WaveData::getValue(double pos, double left, double right, int pixelWidth) {
    // temp
    if (summaries.length() < 5) {
        Packet result;
        result.min = 0;
        result.max = 0;
        return result;
    }

    double width = right - left;
    double sizeMult = 1/width;

    WaveSummary* summary = nullptr;

    for (int i = summaries.length() - 1; i >= 0; i--) {
        if (summaries[i].m_packets[0].length() > pixelWidth * sizeMult) {
            summary = &summaries[i - 1 == -1 ? i : i - 1];
            break;
        }
    }

    if (summary == nullptr)
        summary = &summaries[0];

    int packetWidth = static_cast<int>(summary->m_packets[0].length() * width + 0.5) / pixelWidth;

//    if (packetWidth == 0)
        packetWidth++;

    auto lFloat = summary->m_packets[0].length() * (pos * width + left);
    auto rFloat = summary->m_packets[0].length() * (pos * width + left) + packetWidth;

    int indLeft = lFloat;
    int indRight = rFloat;

    double remainderLeft = lFloat - indLeft;
    double remainderRight = rFloat - indRight;

    if (indLeft >= summary->m_packets[0].length() - 1) {
        indLeft = summary->m_packets[0].length() - 2;
        remainderLeft = 0;
    }
    if (indRight >= summary->m_packets[0].length()) {
        indRight = summary->m_packets[0].length() - 1;
        remainderRight = 0;
    }

    Packet result;

    double max = -std::numeric_limits<double>::infinity();
    double min = std::numeric_limits<double>::infinity();

    for (int i = indLeft; i <= indRight; i++) {
        auto packetMax = summary->m_packets[0][i].max;
        auto packetMin = summary->m_packets[0][i].min;

        if (packetMax > max)
            max = packetMax;
        if (packetMin < min)
            min = packetMin;
    }

    result.max = max;
    result.min = min;

    // prevent line from being too thin
    auto distance = max - min;
    if (distance < 0.02) {
        auto addAmt = ((0.02 - distance) * 0.5);
        result.max += addAmt;
        result.min -= addAmt;
    }

    return result;
}

/*
    auto positionInList = summary->m_packets[0].length() * (pos * width + left);
    int ind = static_cast<int>(positionInList - 0.0001);

    if (ind == summary->m_packets[0].length() - 1)
        return summary->m_packets[0][ind];

    double weight = positionInList - ind;
    Packet result;

    auto maxLeft = summary->m_packets[0][ind].max;
    auto maxRight = summary->m_packets[0][ind + 1].max;

    auto minLeft = summary->m_packets[0][ind].min;
    auto minRight = summary->m_packets[0][ind + 1].min;

    result.max = maxLeft * (1 - weight) + maxRight * weight;
    result.min = minLeft * (1 - weight) + minRight * weight;
    return result;
    */





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
