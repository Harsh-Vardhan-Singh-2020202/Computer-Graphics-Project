// animating.cpp

#include "sketching.h"

void Animating::PlayAnimation(bool loopInfo)
{
    isPlaying = true;
    isLooping = loopInfo;
}

void Animating::PauseAnimation()
{
    isPlaying = false;
}

void Animating::SetInitialAnimParameters(bool loop, float dur, float xd, float yd, float ang, float xp, float yp)
{
        if(!decided)
        {
            isLooping = loop;
            animationDuration = dur;
            to_dx = xd;
            to_dy = yd;
            to_angle = ang;
            px = xp;
            py = yp;
            decided = true;
        }
}