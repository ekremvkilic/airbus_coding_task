#ifndef AIRBUS_CODING_TASK_COMMON_TIME_H
#define AIRBUS_CODING_TASK_COMMON_TIME_H

typedef struct
{
    int tm_year;
    int tm_mon;
    int tm_mday;
    int tm_hour;
    int tm_min;
    int tm_sec;
} Timestamp;

bool IsEarlier(const Timestamp* const left, const Timestamp* const right)
{
    return !((left->tm_year > right->tm_year)
          || (left->tm_mon  > right->tm_mon)
          || (left->tm_mday > right->tm_mday)
          || (left->tm_hour > right->tm_hour)
          || (left->tm_min  > right->tm_min)
          || (left->tm_sec  > right->tm_sec));
}

void AdvanceTime(Timestamp* const the_time, const int seconds)
{
    the_time->tm_sec += seconds;
    if ((the_time->tm_sec / 60) > 0)
    {
        the_time->tm_sec %= 60;

        the_time->tm_min += 1;
        if ((the_time->tm_min / 60) > 0)
        {
            the_time->tm_min %= 60;

            the_time->tm_hour += 1;
            if ((the_time->tm_hour / 24) > 0)
            {
                the_time->tm_hour %= 24;

                the_time->tm_mday += 1;
                if ((the_time->tm_mday / 32) > 0)
                {
                    the_time->tm_mday %= 31;

                    the_time->tm_mon += 1;
                    if ((the_time->tm_mon / 13) > 0)
                    {
                        the_time->tm_mon %= 12;

                        the_time->tm_year += 1;
                    }
                }
            }
        }
    }
}

#endif // AIRBUS_CODING_TASK_COMMON_TIME_H
