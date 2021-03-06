#pragma once
#include "AIActor.h"

class BombAIActor : public AIActor
{
public:

    /**
    * @brief	Creates a new AI actor with the given parameters.
    *
    * @author	Shane
    * @date	2/8/2016
    *
    * @param	position	    The AI's start position.
    * @param [in,out]	manager	A reference to the game manager.
    * @param	spd			   	The AI's starting speed.
    * @param [in,out]	sprites	A named list of sprite sheets that can be used to draw the AI.
    * @param	startSprite	   	The default sprite sheet to draw.
    * @param	startXDirection	The actor's default facing direction along the x axis.
    * @param	startYDirection	The actor's default facing direction along the y axis.
    */
    BombAIActor(Vector2 position,
        GameManager & manager,
        Vector2 spd,
        std::unordered_map<std::string,
        std::shared_ptr<SpriteSheet >> &sprites,
        const std::string&& startSprite,
        std::unordered_map<std::string, std::pair<std::string, bool>> &sounds,
        std::shared_ptr<SDL2pp::Texture> mindControlIndicator,
        Vector2 spawn,
        SpriteSheet::XAxisDirection startXDirection = SpriteSheet::XAxisDirection::RIGHT, SpriteSheet::YAxisDirection startYDirection = SpriteSheet::YAxisDirection::UP)
        : AIActor(manager, position, spd, sprites, std::move(startSprite), sounds, mindControlIndicator, spawn, startXDirection, startYDirection),
        _isBlowingUp{false}
    {
    }

    /**
     * @brief Creates an AIActor from a serialised string.
     * @param serialised The string specifying the actor's properties.
     * 
     * Format
     * Same as AIActor format (see AIActor.h)
     */
    BombAIActor(std::string & serialised)
        : AIActor{ serialised }, _isBlowingUp{ false } {}

    virtual Type GetType() const override { return Type::bombenemy; }

    bool IsBlowingUp() const;
    void BlowUp();

    virtual void Update(double deltaTime);

    virtual BombAIActor* Clone() override;

    virtual bool IsCloneable() const override { return true; }

    /**
     * Check collision against another SimpleAgent
     *
     * @return true if collided, false if not
     **/
    bool CollisionCheck(Actor & otherAI);

    void Reset(Vector2 position);
private:
    bool _isBlowingUp;
};