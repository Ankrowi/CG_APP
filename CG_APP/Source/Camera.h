#pragma once

#include "d3d11.h"
#include "DirectXMath.h"

class Camera
{
public:
    Camera(); 
    Camera(const Camera& camera); 
    Camera& operator=(const Camera& camera);
    ~Camera(void);

private:
    void initViewMatrix();
public:
    void InitProjMatrix(const float angle, const float client_width, 
        const float client_height, const float nearest, const float farthest);
    void InitOrthoMatrix(const float client_width, const float client_height, 
        const float far_plane);
    void OnResize(uint32_t new_width, uint32_t new_height);
    void Rotate(DirectX::XMFLOAT3 axis, float degrees);
    void Position(DirectX::XMFLOAT3& new_position);
    void Move(DirectX::XMFLOAT3 direction);
    const DirectX::XMFLOAT3& Position() const;
    void Target(DirectX::XMFLOAT3 new_target);
    const DirectX::XMFLOAT3& Target() const;
    const DirectX::XMFLOAT3 Up();
    const DirectX::XMFLOAT3 LookAtTarget();
    const DirectX::XMFLOAT4X4 View();
    const float& Angle() const;
    void Angle(float angle);
    void NearestPlane(float nearest);
    void FarthestPlane(float farthest);
    const DirectX::XMFLOAT4X4 Proj();
    const DirectX::XMFLOAT4X4 Ortho();

private:
    // Camera's coordinates 
    DirectX::XMFLOAT3 mPosition;
    // View target's coordinates
    DirectX::XMFLOAT3 mTarget;
    // Camera's up vector end coordinates 
    DirectX::XMFLOAT3 mUp;

    float mAngle; // Angle of view frustum 
    float mClientWidth; // Window's width 
    float mClientHeight;    // Window's height 
    float mNearest; // Nearest view frustum plane 
    float mFarthest; // Farthest view frustum plane 
    DirectX::XMFLOAT4X4 mView; // View matrix 
    DirectX::XMFLOAT4X4  mProj; // Projection matrix 
    DirectX::XMFLOAT4X4  mOrtho; // Ortho matrix for drawing without tranformation 
};

