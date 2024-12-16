#include "tk_math.h"
#include <time.h> /* time */
#include <stdlib.h> /* rand() */

int tkmt_clamp(int value_to_clamp, int min, int max)
{
    if (value_to_clamp < min) 
        return  min;
    if (value_to_clamp > max)
        return max;
    
    return value_to_clamp;
}

float tkmt_clampf(float value_to_clamp, float min, float max)
{
    if (value_to_clamp < min) 
        return  min;
    if (value_to_clamp > max)
        return max;
    
    return value_to_clamp;
}

void tkmt_srand(void)
{
    srand((unsigned int)time(NULL));
}

int tkmt_rand(int min, int max){
    return min + (int)(rand() * (max - min + 1) / (1 + RAND_MAX));
}

float tkmt_randf(float min, float max)
{
    return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}
