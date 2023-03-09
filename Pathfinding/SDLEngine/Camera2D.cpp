#include "Camera2D.h"

namespace SDLEngine
{

	Camera2D::Camera2D() :
		_screenWidth(500),
		_screenHeight(500),
		_needsMatrixUpdate(true),
		_scale(1),
		_position(0, 0),
		_orthoMatrix(1.0f),
		_cameraMatrix(1.0f)
	{
	}

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		_orthoMatrix = glm::ortho(0.0f, static_cast<float>(_screenWidth), 0.0f, static_cast<float>(_screenHeight));
	}

	void Camera2D::update()
	{
		if (_needsMatrixUpdate)
		{
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) const
	{
		// center
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);

		// scale
		screenCoords /= _scale;

		// translate
		screenCoords += _position;

		return screenCoords;
	}

	bool Camera2D::isBoxInView(const glm::vec2& pos, const glm::vec2& dim)
	{
		const glm::vec2 SCALED_SCREEN_DIM = glm::vec2(_screenWidth, _screenHeight) / _scale;
		const glm::vec2 MIN_DIST = (dim + SCALED_SCREEN_DIM) / 2.0f;

		glm::vec2 centerPos = pos + dim / 2.0f;
		auto distVec = centerPos - _position;

		float xDepth = MIN_DIST.x - std::abs(distVec.x);
		float yDepth = MIN_DIST.y - std::abs(distVec.y);

		if (xDepth > 0 && yDepth > 0)
			return true;

		return false;
	}

	void Camera2D::setScale(const float newScale)
	{
		_scale = newScale;
		_needsMatrixUpdate = true;
	}

	void Camera2D::setPosition(const glm::vec2& newPosition)
	{
		_position = newPosition;
		_needsMatrixUpdate = true;
	}
}
