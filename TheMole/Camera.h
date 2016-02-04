#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2pp\SDL2pp.hh>
#include "Vector2.h"

class Camera {
public:

	/**
	 * @brief	Constructor.
	 *
	 * @author	Shane
	 * @date	2/4/2016
	 *
	 * @param	pos				The camera's (initial) position.
	 * @param	vel				The camera's velocity.
	 * @param	playerPos   	The player position.
	 * @param	viewportSize	Size of the viewport.
	 * @param	mapSize			Size of the map.
	 */
	Camera(const SDL2pp::Point& pos, const SDL2pp::Point& viewportSize, const SDL2pp::Point& mapSize)
		: _viewport(pos.x, pos.y, viewportSize.x, viewportSize.y), _mapSize(mapSize.x, mapSize.y)
	{}

	void Update(double elapsedSecs);
	
	const SDL2pp::Rect& GetViewport() const {
		return _viewport;
	}

	/**
	 * @brief	Centres the view on the given position.
	 *
	 * @author	Shane
	 * @date	2/4/2016
	 *
	 * @param 	pos	The position on which to centre the view.
	 */
	void CentreView(const SDL2pp::Point& pos);

	/**
	* @brief	Centres the view on the given position.
	*
	* @author	Shane
	* @date	2/4/2016
	*
	* @param 	pos	The position on which to centre the view.
	*/
	void CentreView(Vector2&& pos);

private:
	/** @brief	The rectangle seen by the camera. */
	SDL2pp::Rect _viewport;
	SDL2pp::Point _mapSize;

};

#endif
