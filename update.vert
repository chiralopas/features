#version 330 core

/* INPUT: current state of the particle */
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inVelocity;

/* OUTPUT: new state after physics update */
out vec2 outPosition;
out vec2 outVelocity;

uniform float uDeltaTime;

void main()
{
    /* move particle forward by velocity */
    vec2 newPosition = inPosition + inVelocity * uDeltaTime;
    vec2 newVelocity = inVelocity;

    /* boundary check: if out of [-1, 1] range, reset to center */
    if (newPosition.x > 1.0 || newPosition.x < -1.0 ||
        newPosition.y > 1.0 || newPosition.y < -1.0)
    {
        newPosition = vec2(0.0, 0.0);
        // keep newVelocity unchanged
    }

    /* write to transform feedback buffers */
    outPosition = newPosition;
    outVelocity = newVelocity;
}
