#include "Camera.h"

using namespace DirectX;

Camera::Camera() {
   mPosition = XMFLOAT3(0.0f, 0.0f, -1.0f);
   mTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);
   XMStoreFloat3(&mUp, 
	XMLoadFloat3(&mPosition) + XMLoadFloat3(new XMFLOAT3(0, 1, 0)));

   this->initViewMatrix();

   mAngle = 0.0f;
   mClientWidth = 0.0f;
   mClientHeight = 0.0f;
   mNearest = 0.0f;
   mFarthest = 0.0f;

   XMStoreFloat4x4(&mView, XMMatrixIdentity());
   XMStoreFloat4x4(&mProj, XMMatrixIdentity());
   XMStoreFloat4x4(&mOrtho, XMMatrixIdentity());
}

Camera::Camera(const Camera& other) {
	*this = other;
}

Camera& Camera::operator=(const Camera& other) {
    mPosition = other.mPosition;
    mTarget = other.mTarget;
    mUp = other.mUp;
    mAngle = other.mAngle;
    mClientWidth = other.mClientWidth;
    mClientHeight = other.mClientHeight;
    mNearest = other.mNearest;
    mFarthest = other.mFarthest;
    mView = other.mView;
    mProj = other.mProj;
    mOrtho = other.mOrtho;
    return *this;
}

Camera::~Camera() {

}

void Camera::initViewMatrix() {
    auto rUp = Up();
    XMStoreFloat4x4(&mView, XMMatrixLookAtLH(XMLoadFloat3(&mPosition), 
        XMLoadFloat3(&mTarget), XMLoadFloat3(&rUp)));
}

void Camera::InitProjMatrix(const float angle, const float client_width, 
    const float client_height, const float near_plane, const float far_plane) {
    mAngle = angle;
    mClientWidth = client_width;
    mClientHeight = client_height;
    mNearest = near_plane;
    mFarthest = far_plane;

    XMStoreFloat4x4(&mProj, XMMatrixPerspectiveLH(angle, client_width / client_height, near_plane, far_plane));
}

void Camera::InitOrthoMatrix(const float clientWidth, 
    const float clientHeight, const float fartherZ)
{
    XMStoreFloat4x4(&mOrtho, XMMatrixOrthographicLH(clientWidth, clientHeight, 0.0f, fartherZ));
}

void Camera::OnResize(uint32_t new_width, uint32_t new_height)
{
    mClientWidth = new_width; mClientHeight = new_height;
    InitProjMatrix(mAngle, static_cast<float>(new_width), static_cast<float>(new_height), mNearest, mFarthest);
    InitOrthoMatrix(static_cast<float>(new_width), static_cast<float>(new_height), mFarthest);
}

void Camera::Rotate(DirectX::XMFLOAT3 axis, float degrees)
{
    if (XMVector3Equal(XMLoadFloat3(&axis), XMVectorZero()) || degrees == 0.0f) return;

    // rotate vectors 
    XMFLOAT3 look_at_target;
    XMStoreFloat3(&look_at_target,
        XMLoadFloat3(&mTarget) - XMLoadFloat3(&mPosition));
    XMFLOAT3 look_at_up;
    XMStoreFloat3(&look_at_up,
        XMLoadFloat3(&mUp) - XMLoadFloat3(&mPosition));
    XMStoreFloat3(
        &look_at_target, XMVector3Transform(XMLoadFloat3(&look_at_target),
            XMMatrixRotationAxis(XMLoadFloat3(&axis), XMConvertToRadians(degrees)))
    );
    XMStoreFloat3(
        &look_at_up, XMVector3Transform(XMLoadFloat3(&look_at_up),
            XMMatrixRotationAxis(XMLoadFloat3(&axis),
                XMConvertToRadians(degrees)))
    );

    // restore vectors's end points mTarget and mUp from new rotated vectors 
    XMStoreFloat3(
        &mTarget, XMLoadFloat3(&mPosition) + XMLoadFloat3(&look_at_target)
    );
    XMStoreFloat3(
        &mUp, XMLoadFloat3(&mPosition) + XMLoadFloat3(&look_at_up)
    );
    this->initViewMatrix();
}

void Camera::Position(DirectX::XMFLOAT3& new_position)
{
    XMFLOAT3 move_vector;
    XMStoreFloat3(&move_vector,
        XMLoadFloat3(&new_position) - XMLoadFloat3(&mPosition));
    XMFLOAT3 target = mTarget; 
    this->Move(move_vector);
    this->Target(target);
}

void Camera::Move(DirectX::XMFLOAT3 direction) {
    XMStoreFloat3(
        &mPosition, XMVector3Transform(XMLoadFloat3(&mPosition), 
            XMMatrixTranslation(direction.x, direction.y, direction.z))
    );
    XMStoreFloat3(
        &mTarget, XMVector3Transform(XMLoadFloat3(&mTarget), 
            XMMatrixTranslation(direction.x, direction.y, direction.z))
    );
    XMStoreFloat3(
        &mUp, XMVector3Transform(XMLoadFloat3(&mUp), 
            XMMatrixTranslation(direction.x, direction.y, direction.z))
    );

    this->initViewMatrix();
}

const DirectX::XMFLOAT3& Camera::Position() const
{
    return mPosition;
}

void Camera::Target(DirectX::XMFLOAT3 new_target)
{
    if (XMVector3Equal(XMLoadFloat3(&new_target), XMLoadFloat3(&mPosition)) || 
        XMVector3Equal(XMLoadFloat3(&new_target), XMLoadFloat3(&mTarget))) return;


    XMFLOAT3 old_look_at_target;
    XMStoreFloat3(&old_look_at_target,
        XMLoadFloat3(&mTarget) - XMLoadFloat3(&mPosition));
    XMFLOAT3 new_look_at_target;
    XMStoreFloat3(&new_look_at_target,
        XMLoadFloat3(&new_target) - XMLoadFloat3(&mPosition));

    float angle = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenNormals(
        XMVector3Normalize(XMLoadFloat3(&old_look_at_target)),
        XMVector3Normalize(XMLoadFloat3(&new_look_at_target)))));

    if (angle != 0.0f && angle != 360.0f && angle != 180.0f)
    {
        XMVECTOR axis = XMVector3Cross(XMLoadFloat3(&old_look_at_target), XMLoadFloat3(&new_look_at_target));
        XMFLOAT3 axf;
        XMStoreFloat3(&axf, axis);
        Rotate(axf, angle);
    }

    mTarget = new_target;
    this->initViewMatrix();
}

const DirectX::XMFLOAT3& Camera::Target() const
{
    return mTarget;
}

const DirectX::XMFLOAT3 Camera::Up()
{
    XMFLOAT3 res;
    XMStoreFloat3(
        &res,
        XMLoadFloat3(&mUp) - XMLoadFloat3(&mPosition)
    );
    return res;
}

const DirectX::XMFLOAT3 Camera::LookAtTarget()
{
    XMFLOAT3 res;
    XMStoreFloat3(
        &res,
        XMLoadFloat3(&mTarget) - XMLoadFloat3(&mPosition)
    );
    return res;
}

const DirectX::XMFLOAT4X4 Camera::View()
{
    XMFLOAT4X4 res;
    XMStoreFloat4x4(
        &res,
        XMLoadFloat4x4(&mView)
    );
    return res;
}

const float& Camera::Angle() const
{
    return mAngle;
}

void Camera::Angle(float angle) {
    mAngle = angle;
    InitProjMatrix(mAngle, mClientWidth, mClientHeight, mNearest, mFarthest);
}

void Camera::NearestPlane(float nearest)
{
    mNearest = nearest; 
    OnResize(mClientWidth, mClientHeight);
}

void Camera::FarthestPlane(float farthest)
{
    mFarthest = farthest;
    OnResize(mClientWidth, mClientHeight);
}

const DirectX::XMFLOAT4X4 Camera::Proj()
{
    XMFLOAT4X4 res;
    XMStoreFloat4x4(
        &res,
        XMLoadFloat4x4(&mProj)
    );
    return res;
}

const DirectX::XMFLOAT4X4 Camera::Ortho()
{
    XMFLOAT4X4 res;
    XMStoreFloat4x4(
        &res,
        XMMatrixTranspose(XMLoadFloat4x4(&mOrtho))
    );
    return res;
}

