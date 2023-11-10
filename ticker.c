#include <stdio.h>
#include <stdlib.h>
#include <time.h>


struct Clock
{
    int frequency;
    long long int lastTick;
    long long int nextTick;
};

long long unixMillis() 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}

void setClock(struct Clock *clock, int frequency)
{
    clock->frequency = 1000/frequency;
    clock->lastTick = 0;
    clock->nextTick = 0;
}

void tickClock(struct Clock *clock)
{
    if (clock->lastTick == 0)
    {
        clock->lastTick = unixMillis();
    }
    clock->nextTick = clock->lastTick + clock->frequency;
    clock->lastTick = clock->nextTick;
    while (unixMillis() < clock->nextTick)
    {
        // Epic spinlock
        continue;
    }
}
