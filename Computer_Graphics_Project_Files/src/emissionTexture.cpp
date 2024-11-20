// emissionTexture.cpp

# include "emissionTexture.h"

void EmissionTexture::instantiateParticleOnSurface(double currentTime)
{
    const std::vector<float>& surfacePoints = e_surface.getDrawingPoints();

    const std::vector<float>& pathPoints = e_path.getDrawingPoints();

    // Logic to select a random point on e_surface
    if (!surfacePoints.empty() && !pathPoints.empty())
    {
        // Calculate the time since the last instantiation
        double elapsedTime = currentTime - lastInstantiationTime;

        // Check if it's time to instantiate
        if (elapsedTime >= 1.0 / e_rate)
        {
            // Generate a random index
            size_t randomIndex = 3 * (std::rand() % (surfacePoints.size() / 3));

            // Get the coordinates of the selected point
            float x = surfacePoints[randomIndex];
            float y = surfacePoints[randomIndex + 1];
            float z = surfacePoints[randomIndex + 2];

            glm::vec3 randomPointOnSurface(x, y, z);

            std::vector<float> particle_dp = e_particle.getDrawingPoints();
            std::vector<float> path_dp = e_path.getDrawingPoints();

            glm::vec3  meanPoint_particle = getMeanPoint(particle_dp);
            glm::vec3 startPoint_path(path_dp[0],path_dp[1],0.0f);

            glm::vec3 positionDiff_particle = randomPointOnSurface - meanPoint_particle;
            glm::vec3 positionDiff_path = randomPointOnSurface - startPoint_path;

            EmissionProperties newEParticle;
            EmissionProperties newEPath;

            // Now, iterate through the drawingSegments and update the particle points
            for (size_t i = 0; i < particle_dp.size(); i += 3)
            {
                particle_dp[i] += positionDiff_particle.x;
                particle_dp[i + 1] += positionDiff_particle.y;
                // No need to modify drawingPoints[i + 2] (Z-coordinate)
            }

            // Now, iterate through the drawingSegments and update the path points
            for (size_t i = 0; i < path_dp.size(); i += 3)
            {
                path_dp[i] += positionDiff_path.x;
                path_dp[i + 1] += positionDiff_path.y;
                // No need to modify drawingPoints[i + 2] (Z-coordinate)
            }

            // Set the updated drawing points for the new particle
            newEParticle.setDrawingPoints(particle_dp);
            // Set the updated drawing points for the new path
            newEPath.setDrawingPoints(path_dp);

            // Add the new particle to ParticleList
            ParticleList.push_back(newEParticle);
            // Add the new particle to PathList
            PathList.push_back(newEPath);
            // Add the new float = 0 to ParticleProgressList
            ParticleProgressList.push_back(0.0f);

            // Update the last instantiation time
            lastInstantiationTime = currentTime;
        }
    }
}

// Update particle positions based on their paths
void EmissionTexture::updateParticles(double currentTime)
{
    // Update each particle
    for (size_t i = 0; i < ParticleList.size(); ++i)
    {
        const std::vector<float>& pathPoints = PathList[i].getDrawingPoints();

        // Check if the particle has a valid path
        if (!pathPoints.empty())
        {
            float framesPerSecond = ImGui::GetIO().Framerate;
            float durationInSeconds = 1.0f/e_speed;
            ParticleProgressList[i] += 1.0f / (framesPerSecond * durationInSeconds);

            // Check if the particle is masked
            bool isMasked = checkMasking(i);

            if(ParticleProgressList[i] >= 1 || isMasked)
            {
                // Remove the particle if it reached the end
                ParticleList.erase(ParticleList.begin() + i);
                // Remove the corresponding path as well
                PathList.erase(PathList.begin() + i);
                // Remove the corresponding progress as well
                ParticleProgressList.erase(ParticleProgressList.begin() + i);
            }
            else
            {
                // Calculate the index in the pathPoints vector
                size_t index = 3 * static_cast<size_t>(ParticleProgressList[i] * pathPoints.size() / 3);

                // Get the coordinates of the point on the path
                float x = pathPoints[index];
                float y = pathPoints[index + 1];
                float z = pathPoints[index + 2];

                glm::vec3 pathPoint(x, y, z);

                // Get the mean position of the particle
                glm::vec3 meanPoint = getMeanPoint(ParticleList[i].getDrawingPoints());

                // Calculate the position difference between the path point and the mean point
                glm::vec3 positionDiff = pathPoint - meanPoint;

                // Update the position of all points in the particle
                std::vector<float> updatedPoints = ParticleList[i].getDrawingPoints();
                for (size_t j = 0; j < updatedPoints.size(); j += 3)
                {
                    updatedPoints[j] += positionDiff.x;
                    updatedPoints[j + 1] += positionDiff.y;
                    // No need to modify updatedPoints[j + 2] (Z-coordinate)
                }

                // Set the updated drawing points for the particle
                ParticleList[i].setDrawingPoints(updatedPoints);

                ++i;
            }
        }
    }
}

// Helper function to check if particle is masked
bool EmissionTexture::checkMasking(size_t i)
{
    std::vector<float> maskPoints = e_mask.getDrawingPoints();
    glm::vec2 meanParticlePos = getMeanPoint(ParticleList[i].getDrawingPoints());

    float threshold = 0.1f;

    // Iterate over mask points and check distance
    for (size_t j = 0; j < maskPoints.size(); j += 3)
    {
        float maskX = maskPoints[j];
        float maskY = maskPoints[j + 1];

        // Calculate Euclidean distance
        float distance = glm::distance(glm::vec2(meanParticlePos.x, meanParticlePos.y), glm::vec2(maskX, maskY));

        // Check if the distance is below the threshold
        if (distance < threshold)
        {
            // Particle is near a mask point
            return true;
        }
    }

    // Particle is not near any mask point
    return false;
}

// Helper function to calculate the mean point of a particle
glm::vec3 EmissionTexture::getMeanPoint(std::vector<float> dp)
{
    float sumX = 0.0f;
    float sumY = 0.0f;

    for (size_t i = 0; i < dp.size(); i += 3)
    {
        sumX += dp[i];
        sumY += dp[i + 1];
        // Z-coordinate is at index i + 2, but not needed for mean calculation
    }

    float meanX = sumX / (dp.size() / 3);
    float meanY = sumY / (dp.size() / 3);

    return glm::vec3(meanX, meanY, 0.0f);
}

void EmissionTexture::draw(ImGuiIO& io, int w, int h, double currentTime)
{
    if(CreationLevel == 0)
    {
        DrawParticles(io, w, h);
    }
    else if(CreationLevel == 1)
    {
        DrawMask(io, w, h);
        ViewParticle();
    }
    else if(CreationLevel == 2)
    {
        DrawSurface(io, w, h);
        ViewParticle();
        ViewMask();
    }
    else if(CreationLevel == 3)
    {
        DrawPath(io, w, h);
        ViewParticle();
        ViewMask();
        ViewSurface();
    }
    else if(CreationLevel == 4)
    {
        // Values for speed and rate will be set in the utils panel

        ViewParticle();
        ViewMask();
        ViewSurface();
        ViewPath();
    }
    if(CreationLevel == 5)
    {
        instantiateParticleOnSurface(currentTime);
        updateParticles(currentTime);
        ViewInstantiatedParticles();
    }
}

void EmissionTexture::viewDrawing(double currentTime)
{
    if(CreationLevel == 5)
    {
        instantiateParticleOnSurface(currentTime);
        updateParticles(currentTime);
        ViewInstantiatedParticles();
    }
}