// emissionTexture.h

#ifndef _EMISSIONTEXTURE_H
#define _EMISSIONTEXTURE_H

#include "utils.h"
#include "sketching.h"
#include "emissionProperties.h"

class EmissionTexture
{
private:
    int id;
    int CreationLevel;
    EmissionProperties e_particle;
    EmissionProperties e_mask;
    EmissionProperties e_surface;
    EmissionProperties e_path;

    float e_rate;
    float e_speed;

    double lastInstantiationTime;
    double lastUpdateParticlesTime;

    std::vector<EmissionProperties> ParticleList;
    std::vector<EmissionProperties> PathList;

    std::vector<float> ParticleProgressList;

public:
    // Constructor with member initializer list
    EmissionTexture(EmissionProperties particle, EmissionProperties mask, EmissionProperties surface, EmissionProperties path, float rate, float speed)
            : e_particle(particle), e_mask(mask), e_surface(surface), e_path(path), e_rate(rate), e_speed(speed), lastInstantiationTime(0.0)
    {
        CreationLevel = 0;
    }

    void instantiateParticleOnSurface(double currentTime);
    void updateParticles(double currentTime);
    glm::vec3 getMeanPoint(std::vector<float> dp);
    bool checkMasking(size_t i);

    void draw(ImGuiIO& io, int w, int h, double currentTime);
    void viewDrawing(double currentTime);

    void DrawMask(ImGuiIO& io, int width, int height){e_mask.draw(io, width, height);}
    void DrawSurface(ImGuiIO& io, int width, int height){e_surface.draw(io, width, height);}
    void DrawPath(ImGuiIO& io, int width, int height){e_path.draw(io, width, height);}
    void DrawParticles(ImGuiIO& io, int width, int height){e_particle.draw(io, width, height);}

    void ViewMask(){e_mask.viewDrawing();}
    void ViewSurface(){e_surface.viewDrawing();}
    void ViewPath(){e_path.viewDrawing();}
    void ViewParticle(){e_particle.viewDrawing();}
    void ViewInstantiatedParticles(){for (EmissionProperties particle : ParticleList){particle.viewDrawing();}}

    void deleteVBO(){e_particle.deleteVBO(); e_mask.deleteVBO(); e_surface.deleteVBO(); e_path.deleteVBO();}

    // setters
    void setId(int _id){id = _id;}
    void setRate(int rate) { e_rate = rate;}
    void setSpeed(float speed) { e_speed = speed;}
    void setCreationLevel(int c_l){CreationLevel = c_l;}

    // getters
    int getId(){return id;}
    int getRate() const { return e_rate;}
    int getCreationLevel(){return CreationLevel;}
    float getSpeed() const { return e_speed;}
};
#endif