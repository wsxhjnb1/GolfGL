#include <precomp.h>

#include "Camera.h"

void Camera::Init() { ms_Camera = std::move(std::unique_ptr<Camera>()); }

void Camera::LookUp() { cameraPos += cameraSpeed * cameraFront; }

void Camera::LookDown() { cameraPos -= cameraSpeed * cameraFront; }

void Camera::LookRight() { cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; }

void Camera::LookLeft() { cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; }

glm::mat4 Camera::LookAt() { return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); }

void Camera::SetCameraSpeed(float dt) { cameraSpeed = MovementSpeed * dt; }

glm::vec3 Camera::GetCameraPos() { return cameraPos; }

float Camera::GetCameraZoom() { return Zoom; }

glm::vec3 Camera::GetCameraFront() { return cameraFront; }

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    m_BoundAngleInRange(Pitch, -constrainPitchBound, constrainPitchBound);

    // update Front, Right and Up Vectors using the updated Euler angles
    m_UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= static_cast<float>(yoffset);
    m_BoundAngleInRange(Zoom, zoomLowerBound, zoomUpperBound);
}

void Camera::m_UpdateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x     = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y     = sin(glm::radians(Pitch));
    front.z     = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    cameraFront = glm::normalize(front);
    // also re-calculate the Right and Up vector
    // LookRight();
    // LookUp();
}
