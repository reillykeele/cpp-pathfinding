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

		/**
		 * \brief 2D axis-aligned bounding box check if the box is in the
		 * camera's view for camera culling.
		 * \param pos Position of the bounding box in the world.
		 * \param dim Dimensions of the bounding box (width, height). 
		 * \return True if the box is in view of the camera, otherwise false.
		 */
		bool isBoxInView(const glm::vec2& pos, const glm::vec2& dim);

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