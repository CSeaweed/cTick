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
  mingw_gettimeofday(&tv, NULL);
  return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}

void setClock(struct Clock *clock, int frequency)
{
  clock->frequency = 1000/frequency;
  clock->lastTick = 0;
  clock->nextTick = 0;
}

int checkDelay(struct Clock *clock)
{
  if (unixMillis() > clock->nextTick)
  {
    clock->nextTick = (unixMillis() - clock->nextTick) % clock->frequency; 
    clock->lastTick = clock->nextTick;
    return 1;
  }
  return 0;
}

void tickClock(struct Clock *clock)
{
  if (clock->lastTick == 0)
  {
    clock->lastTick = unixMillis();
  }

  clock->nextTick = clock->lastTick + clock->frequency;
  clock->lastTick = clock->nextTick;

  checkDelay(clock);

  int tickDelay = clock->nextTick - unixMillis();
  if (tickDelay < clock->frequency)
  {
    printf("TICK SHORT BY %dms\n", clock->frequency - tickDelay);
  }

  while (unixMillis() < clock->nextTick)
  {
    // Epic spinlock
    // This part does not matter
    // The point here is to make a clock that prevents sliding,
    // not to make a perfect clock 
    continue;
  }
}

int main()
{
  struct Clock *clock = malloc(sizeof(struct Clock));
  setClock(clock, 10);
  for (int i = 0; i < 50; i++)
  {
    tickClock(clock);
  }
  free(clock);
  return 0;
}
