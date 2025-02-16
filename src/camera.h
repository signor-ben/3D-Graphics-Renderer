#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    SPRINT,
    JUMP
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.01f;
const float ZOOM = 80.0f;
const float GRAVITY = -9.8f;
const float JUMP_STRENGTH = 4.5f;

// Camera class that processes input and calculates matrices for OpenGL
class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Physics properties
    float velocityY = 0.0f; // Vertical velocity
    bool isJumping = false; // Jumping state

    // Constructor
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns view matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // Processes keyboard input
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;

        glm::vec3 moveDirection = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));

        switch (direction)
        {
        case FORWARD:
            Position += moveDirection * velocity;
            break;
        case BACKWARD:
            Position -= moveDirection * velocity;
            break;
        case LEFT:
            Position -= Right * velocity;
            break;
        case RIGHT:
            Position += Right * velocity;
            break;
        case SPRINT:
            Position += moveDirection * velocity * 1.1f;
            break;
        case JUMP:
            if (!isJumping) // Only jump if on the ground
            {
                isJumping = true;
                velocityY = JUMP_STRENGTH; // Jump impulse
            }
            break;
        }
    }

    // Apply physics updates (gravity & jumping)
    void UpdatePhysics(float deltaTime)
    {
        if (isJumping)
        {
            velocityY += GRAVITY * deltaTime; // Apply gravity
            Position.y += velocityY * deltaTime; // Update vertical position

            // Collision with ground (reset state)
            if (Position.y <= 0.0f)
            {
                Position.y = 0.0f;
                velocityY = 0.0f;
                isJumping = false;
            }
        }
    }

    // Processes mouse movement
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch)
        {
            if (Pitch > 89.0f) Pitch = 89.0f;
            if (Pitch < -89.0f) Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    // Processes scroll wheel input
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= yoffset;
        if (Zoom < 1.0f) Zoom = 1.0f;
        if (Zoom > 80.0f) Zoom = 80.0f;
    }

private:
    // Updates the camera vectors
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

#endif
