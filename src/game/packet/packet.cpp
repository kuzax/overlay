#include "packet.h"
#include <globals.h>
#include <game/manager/manager.h>
#include <thread>
#include <chrono>
#include <iostream>  
#include <data/elements.h>

void c_packets::record(unsigned char packetId, NetBitStreamInterface* bitStream, NetPacketPriority priority, NetPacketReliability reliability, ePacketOrdering ordering)
{
    if (!manager || !manager->is_loaded())
    {
        return;
    }
    if (!global || !global->m_pNet || !bitStream)
    {
        return;
    }

    if (element->info.bypass_screenshot)
    {
        m_blockedIds.insert(94);
    }

    if (IsBlocked(packetId) || m_blockedIds.count(packetId))
    {
        return;
    }

    size_t size = bitStream->GetNumberOfBytesUsed();
    const char* data = reinterpret_cast<const char*>(bitStream->GetData());
    if (!data || size == 0)
    {
        return;
    }
    m_packets.emplace_back(packetId, data, size, priority, reliability, ordering);
}

void c_packets::resend(size_t index, int times)
{
    if (!manager || !manager->is_loaded())
    {
        std::cerr << "Hata: Manager geçersiz veya yüklenmemiþ!" << std::endl;
        return;
    }

    if (!global || !global->m_pNet || index >= m_packets.size() || times <= 0)
    {
        std::cerr << "Hata: Geçersiz parametreler!" << std::endl;
        return;
    }

    const auto& packet = m_packets[index];

    for (int i = 0; i < times; ++i)
    {
        NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
        if (pBitStream)
        {
            pBitStream->Write(packet.data.data(), packet.dataSize);
            global->m_pNet->SendPacket(packet.packetId, pBitStream, packet.priority, packet.reliability, packet.ordering);
            global->m_pNet->DeallocateNetBitStream(pBitStream);
        }
        else
        {
            std::cerr << "Hata: BitStream tahsis edilemedi!" << std::endl;
        }
    }
}

void c_packets::block(unsigned char packetId, bool blocked)
{
    m_blockedPackets[packetId] = blocked;
}

std::vector<s_packets> c_packets::getPacketsInTimeRange(const std::chrono::system_clock::time_point& eventTime, std::chrono::milliseconds range)
{
    std::vector<s_packets> matchingPackets;
    auto rangeStart = eventTime - range;
    auto rangeEnd = eventTime + range;

    for (const auto& packet : m_packets)
    {
        if (packet.timePoint >= rangeStart && packet.timePoint <= rangeEnd)
        {
            matchingPackets.push_back(packet);
        }
    }

    return matchingPackets;
}

void c_packets::resendByEvent(const std::string& eventName, const std::chrono::system_clock::time_point& eventTime)
{
    auto matchingPackets = getPacketsInTimeRange(eventTime);

    for (const auto& packet : matchingPackets)
    {
        NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
        if (pBitStream)
        {
            pBitStream->Write(packet.data.data(), packet.dataSize);
            global->m_pNet->SendPacket(packet.packetId, pBitStream, packet.priority, packet.reliability, packet.ordering);
            global->m_pNet->DeallocateNetBitStream(pBitStream);
        }
        else
        {
            std::cerr << "Hata: BitStream tahsis edilemedi!" << std::endl;
        }
    }
}

void c_packets::send_random_packets_for_duration(int duration_seconds)
{
    auto start_time = std::chrono::steady_clock::now();
    auto end_time = start_time + std::chrono::seconds(duration_seconds);

    std::uniform_int_distribution<> packetIdDist(1, 255);
    std::uniform_int_distribution<> priorityDist(0, 2);
    std::uniform_int_distribution<> reliabilityDist(0, 2);
    std::uniform_int_distribution<> orderingDist(0, 1);

    while (std::chrono::steady_clock::now() < end_time)
    {
        for (int i = 0; i < 60; ++i)
        {
            unsigned char packetId = static_cast<unsigned char>(packetIdDist(gen));
            NetPacketPriority priority = static_cast<NetPacketPriority>(priorityDist(gen));
            NetPacketReliability reliability = static_cast<NetPacketReliability>(reliabilityDist(gen));
            ePacketOrdering ordering = static_cast<ePacketOrdering>(orderingDist(gen));

            NetBitStreamInterface* pBitStream = global->m_pNet->AllocateNetBitStream();
            if (pBitStream)
            {
                const char* exampleData = "random_packet_data";
                pBitStream->Write(exampleData, strlen(exampleData));
                global->m_pNet->SendPacket(packetId, pBitStream, priority, reliability, ordering);
                global->m_pNet->DeallocateNetBitStream(pBitStream);
            }
            else
            {
                std::cerr << "Hata: BitStream tahsis edilemedi!" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}