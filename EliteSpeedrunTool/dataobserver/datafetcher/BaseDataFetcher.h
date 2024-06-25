#pragma once

#include "MemoryUtil.h"
#include <QDebug>

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
        memoryUtil->read(memoryUtil->missionPtr + (0x8 * index), &buffer, sizeof(T));
        return buffer;
    }

    T getGlobalData(int index)
    {
        long long buffer;
        memoryUtil->read(memoryUtil->globalPtr + (0x8 * (index >> 0x12 & 0x3F)), &buffer, 8);
        T data;
        memoryUtil->read(buffer + (8 * (index & 0x3FFFF)), &data, sizeof(T));
        return data;
    }
};
