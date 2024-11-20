// animating.h

#ifndef _ANIMATING_H_
#define _ANIMATING_H_

#include "utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Animating
{
private:
    int anim_id;

    bool decided;

    glm::mat3 transformationMatrix;

    float animationTime = 0.0f;
    float animationDuration = 1.0f;
    bool isPlaying = false;
    bool isLooping = false;

public:

    Animating()
    {
        transformationMatrix = glm::mat3(1.0f);
        decided = false;
    }

    float to_dx = 0.0f, to_dy = 0.0f, to_angle = 0.0f;
    float px = 0.0f, py = 0.0f;

    void SetInitialAnimParameters(bool loop, float dur, float xd, float yd, float ang, float xp, float yp);
    void PlayAnimation(bool loopInfo);
    void PauseAnimation();

    // setters
    void setId(int _id){anim_id = _id;}
    void setIsPlaying(bool _ip){isPlaying = _ip;}
    void setAnimationTime(float _at){animationTime = _at;}

    // getters
    int getId(){return anim_id;}
    bool getIsPlaying(){return  isPlaying;}
    bool getIsLooping(){return  isLooping;}
    bool getDecided(){return  decided;}
    float getAnimationDuration(){return  animationDuration;}
    float getAnimationTime(){return  animationTime;}
    glm::mat3 getAnimTransformationMatrix(){return transformationMatrix;}

};
#endif