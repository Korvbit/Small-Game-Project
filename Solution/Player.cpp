#include "Player.h"
#include <iostream> // TODO: Remove after testing

Player::Player(float height, float fov, float near, float far, glm::vec3 walkingVector)
	:playerCamera(glm::vec3(0, height, 0), fov, (float)SCREENWIDTH / (float)SCREENHEIGHT, near, far, walkingVector)
{
	this->playerSpeed = 0;
	this->walkingVector = walkingVector;
	//this->maze = maze;
}

Player::~Player()
{
}

float Player::GetPlayerSpeed()
{
	return this->playerSpeed;
}

float Player::GetPlayerHeight()
{
	return this->playerHeight;
}

glm::vec3 Player::GetWalkingVector()
{
	return this->walkingVector;
}

Camera * Player::GetCamera()
{
	return &this->playerCamera;
}

void Player::SetPlayerHeight(float height)
{
	this->playerHeight = height;
}

void Player::SetPlayerSpeed(float speed)
{
	this->playerSpeed = speed;
}

void Player::MoveForward(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * this->walkingVector * elapsedTime;
	playerCamera.SetCameraPosition(newPos);
}

void Player::MoveBackward(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * this->walkingVector * elapsedTime;
	playerCamera.SetCameraPosition(newPos);
}

void Player::MoveRight(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * playerCamera.GetRotateAround() * elapsedTime;
	playerCamera.SetCameraPosition(newPos);
}

void Player::MoveLeft(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * playerCamera.GetRotateAround() * elapsedTime;
	playerCamera.SetCameraPosition(newPos);
}

void Player::MoveUp(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.GetCameraPosition() + this->playerSpeed * playerCamera.GetUpVector() * elapsedTime;
	playerCamera.SetCameraPosition(newPos);
}

void Player::MoveDown(float elapsedTime)
{
	glm::vec3 newPos = playerCamera.GetCameraPosition() - this->playerSpeed * playerCamera.GetUpVector() * elapsedTime;
	playerCamera.SetCameraPosition(newPos);
}

void Player::UpdateMouse(const glm::vec2& newMousePosition, float elapsedTime)
{
	// Get mouse delta vector, how much the mouse has moved
	this->playerCamera.SetMouseDelta(newMousePosition - this->playerCamera.GetOldMousePosition());

	// if the mouseDelta is to far away, the camera will jump to unpredicted areas.
	if (glm::length(this->playerCamera.GetMouseDelta()) < 50.0f)
	{
		//Update the horizontal view
		this->playerCamera.SetForwardVector(glm::mat3(glm::rotate(-this->playerCamera.GetMouseDelta().x * this->playerCamera.GetRotationalSpeed() * elapsedTime, this->playerCamera.GetUpVector())) * this->playerCamera.GetForwardVector());
		this->walkingVector = glm::mat3(glm::rotate(-this->playerCamera.GetMouseDelta().x * this->playerCamera.GetRotationalSpeed() * elapsedTime, this->playerCamera.GetUpVector())) * this->walkingVector;

		//Update the vertical view limited to 45 degrees
		playerCamera.SetRotateAround(glm::cross(this->playerCamera.GetForwardVector(), this->playerCamera.GetUpVector()));
		//if (glm::dot(glm::normalize(playerCamera.GetForwardVector()), glm::normalize(this->walkingVector)) > 0.45f)
		//{
			playerCamera.SetForwardVector(glm::mat3(glm::rotate(-this->playerCamera.GetMouseDelta().y * this->playerCamera.GetRotationalSpeed() * elapsedTime, this->playerCamera.GetRotateAround())) * this->playerCamera.GetForwardVector());
		//}
		//else
		//{
		//	playerCamera.SetForwardVector(glm::mat3(glm::rotate(playerCamera.GetMouseDelta().y * playerCamera.GetRotationalSpeed() * elapsedTime, playerCamera.GetRotateAround())) * playerCamera.GetForwardVector());
		//}
	}

	playerCamera.SetOldMousePosition(newMousePosition);
}

void Player::Update(double dt)
{
	// Set player position to the cameras position
	transform.SetPos(this->playerCamera.GetCameraPosition());

	// Test variables
	double x = 0.0;
	double y = 0.0;

	// Move player
	x = 3.0;
	y = 0.0;

	/*
	// Check collision
	if (maze->IsWallAtWorld(x, y))
	{
		// Don't move
		cout << "V�GGGGG" << endl; // TODO: Remove after testing
	}
	*/

}