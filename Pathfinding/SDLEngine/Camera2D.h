#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

namespace SDLEngine
{

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);

		void update();

		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords) const;

		float getScale() const { return _scale; }
		void setScale(const float newScale);

		glm::vec2 getPosition() const { return _position; }
		void setPosition(const glm::vec2& newPosition);

		glm::mat4 getCameraMatrix() const { return _cameraMatrix;  }

	private:
		int _screenWidth;
		int _screenHeight;

		bool _needsMatrixUpdate;

		float _scale;

		glm::vec2 _position;
		glm::mat4 _orthoMatrix;
		glm::mat4 _cameraMatrix;
	};

}