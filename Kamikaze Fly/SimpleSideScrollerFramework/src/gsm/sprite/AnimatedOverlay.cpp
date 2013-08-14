#include "stdafx.h"
#include "src\gsm\physics\PhysicalProperties.h"
#include "src\gsm\sprite\AnimatedOverlay.h"
#include "src\gsm\sprite\AnimatedSpriteType.h"

void AnimatedOverlay::update()
{
}

/*
	AnimatedOverlay - Default constructor, just sets everything to 0.
*/
AnimatedOverlay::AnimatedOverlay()  
{
	spriteType = 0;
	frameIndex = 0;
	animationCounter = 0;
	objectToFollow = NULL;
	remove = false;
}

/*
	Destructor - Nothing to clean up. We don't want to clean up the
	sprite type because it is a shared variable.
*/
AnimatedOverlay::~AnimatedOverlay() 
{

}

/*
	changeFrame - This method allows for the changing of an image in an 
	animation sequence for a given animation state.
*/
void AnimatedOverlay::changeFrame()
{
	// RESET THE COUNTER
	animationCounter = 0;

	// REMEMBER, WE COUNT BY 2s
	frameIndex += 2;

	// GO BACK TO FIRST INDEX IF NECESSARY
	if (frameIndex == spriteType->getSequenceSize(currentState))
		frameIndex = 0;
}

/*
	getCurrentImageID - This method does the work of retrieving
	the image id that corresponds to this sprite's current state
	and frame index.
*/
unsigned int AnimatedOverlay::getCurrentImageID()
{
	return spriteType->getAnimationFrameID(currentState, frameIndex);
}

/*
	setCurrentState - This method sets this sprite to the newState
	state and starts its animtion sequence from the beginning.
*/
void AnimatedOverlay::setCurrentState(wstring newState) 
{
	string cs(currentState.begin(), currentState.end());
	string ns(newState.begin(), newState.end());
	if (strcmp(cs.c_str(), ns.c_str()) != 0)
	{
		// SET THE ANIMATINO STATE
		currentState = newState;

		// AND RESET OUR COUNTERS
		animationCounter = 0;
		frameIndex = 0;
	}
}

/*
	updateSprite - To be called every frame of animation. This
    method advances the animation counter appropriately per
	the animation speed. It also updates the sprite location
	per the current velocity.
*/
void AnimatedOverlay::updateSprite()
{
	if(objectToFollow && objectToFollow->getIsDying())
	{
		markForRemoval();
	}

	unsigned int duration = spriteType->getDuration(currentState, frameIndex);
	animationCounter++;

	// WE ONLY CHANGE THE ANIMATION FRAME INDEX WHEN THE
	// ANIMATION COUNTER HAS REACHED THE DURATION
	if (animationCounter >= duration)
		changeFrame();
}