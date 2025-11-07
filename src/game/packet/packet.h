#pragma once
#include <includes.h>
#include <sdk.h>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <iomanip>
#include <sstream>
#include <iostream> 
struct s_packets {
    unsigned char packetId;
    std::vector<char> data;
    NetPacketPriority priority;
    NetPacketReliability reliability;
    ePacketOrdering ordering;
    bool blocked;
    std::string timestamp;
    std::chrono::system_clock::time_point timePoint;
    size_t dataSize;

    s_packets(unsigned char id, const char* packetData, size_t size,
        NetPacketPriority prio, NetPacketReliability rel, ePacketOrdering ord)
        : packetId(id)
        , data(packetData, packetData + size)
        , priority(prio)
        , reliability(rel)
        , ordering(ord)
        , blocked(false)
        , dataSize(size)
    {
        timePoint = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(timePoint);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        timestamp = ss.str();
    }

    std::string getFormattedTime() const {
        auto time = std::chrono::system_clock::to_time_t(timePoint);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
};

class c_packets {
private:
    std::vector<s_packets> m_packets;
    std::unordered_map<unsigned char, bool> m_blockedPackets;
    std::unordered_set<unsigned char> m_blockedIds = {32,52,85,31};
    std::random_device rd;
    std::mt19937 gen;

public:
    c_packets() : gen(rd()) {}

    void record(unsigned char packetId, NetBitStreamInterface* bitStream, NetPacketPriority priority, NetPacketReliability reliability, ePacketOrdering ordering);
    void resend(size_t index, int times);
    void block(unsigned char packetId, bool blocked);
    void resendByEvent(const std::string& eventName, const std::chrono::system_clock::time_point& eventTime);

    bool IsBlocked(unsigned char packetId) const {
        auto it = m_blockedPackets.find(packetId);
        return it != m_blockedPackets.end() && it->second;
    }

    const std::vector<s_packets>& GetPackets() const { return m_packets; }

    void clearPacketsOnly() { m_packets.clear(); }
    void clearAll() {
        m_packets.clear();
        m_blockedPackets.clear();
    }

    void send_random_packets_for_duration(int duration_seconds);
    std::vector<s_packets> getPacketsInTimeRange(const std::chrono::system_clock::time_point& eventTime,
        std::chrono::milliseconds range = std::chrono::milliseconds(100));
};

inline c_packets* packet = new c_packets();