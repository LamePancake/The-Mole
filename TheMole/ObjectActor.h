#pragma once
#include "Actor.h"
class PlayerActor;
class GameScreen;

class ObjectActor : public Actor
{
public:

	enum ObjectType
	{
		flag,
		pancake,
		projectileThrower,
		dialogTrigger,
		tutorialSign,
	};

	/**
	 * Constructor that initializes _position to the input parameter.
	 *
	 * @param	position		starting position of the actor.
	 * @param [in,out]	manager A reference to the game manager.
	 * @param	spd				The actor's starting velocity.
	 * @param	id				The type of object (pancake or flag) that this object represents. Should probably be broken into sub-classes.
	 * @param	sprites			A map of names to sprite sheets.
	 * @param	startSprite		The name of the sprite to show by default.
	 * @param	startXDirection	The direction in the x axis which the actor will face at the start.
	 * @param	startYDirection	The direction in the y axis which the actor will face at the start.
	 */
	ObjectActor(Vector2 position,
                GameManager & manager,
                Vector2 spd,
                int id,
                std::unordered_map<std::string, std::shared_ptr<SpriteSheet>>& sprites,
                const std::string&& startSprite,
                std::unordered_map<std::string, std::pair<std::string, bool>> & sounds,
		        SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT,
                SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP,
                std::string dialog = " ",
                int numericIdentifier = -1);

    // Format: Same as Actor (see Actor.h), followed by
    // id (int)
    // dialog (string, may be empty)
    // numericIdentifier (int)
    ObjectActor(std::string serialised);

	/** Destructor. */
	~ObjectActor();

	/**
	* Draws.
	*
	* @param [in,out]	level	The level.
	*
	* 	Draws this object.
	*/
	virtual void Draw(Camera& camera);

	// All the state changing stuff happens in here.
	virtual void Update(double elapsedSecs) override;

	// Resets the actor
	virtual void Reset(Vector2 pos);

    virtual ObjectActor* Clone();

    virtual bool IsCloneable() const { return true; }

	virtual Type GetType() const override { return Type::object; }
	
	/**
	* Collision check.
	*
	* @param [in,out]	otherAI	The other an i.
	*
	* @return	true if it succeeds, false if it fails.
	*/
	bool CollisionCheck(Actor & otherAI);

	/**
	 * Gets the identifier.
	 *
	 * @return	The identifier.
	 */
	int GetID();

	/**
	 * Sets an identifier.
	 *
	 * @param	id	The identifier.
	 */
	void SetID(int id);

	void SetDialog(std::string d);

	std::string GetDialog();

	void SetNumericID(int id);

	int GetNumericID();

private:

	int _id;

	bool _collided;

	void FlagUpdate(double elapsedSecs);

	void PancakeUpdate(double elapsedSecs);

	void ProjectileThrowerUpdate(double elapseSecs);

	void DialogUpdate(double elapsedSecs);

	int _numericIdentifier;

	std::string _dialog;
	double _timer;

	SDL2pp::Texture* _dialogTexture;
	SDL2pp::Font* _font;
};

