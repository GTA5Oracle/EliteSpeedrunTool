#pragma once

#include "MemoryUtil.h"
#include <QObject>

template <typename T>
class BaseDataFetcher : public QObject {
public:
    explicit BaseDataFetcher(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    virtual T fetchData() = 0;

    T getLocalData(int index)
    {
        T buffer;
        MemoryUtil::read(MemoryUtil::missionPtr + (0x8 * index), &buffer, sizeof(T));
        return buffer;
    }

    T getGlobalData(int index)
    {
        long long buffer;
        MemoryUtil::read(MemoryUtil::globalPtr + (0x8 * (index >> 0x12 & 0x3F)), &buffer, 8);
        T data;
        MemoryUtil::read(buffer + (8 * (index & 0x3FFFF)), &data, sizeof(T));
        return data;
    }
};
