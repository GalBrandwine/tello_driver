#pragma once
namespace tello_protocol
{
    struct Vec3
    {
        float x, y, z;
    };
    struct Vec4
    {
        float x, y, z, w;
    };

    struct PoseVelData
    {
        Vec3 pose,vel;
    };
    
} // namespace tello_protocol
