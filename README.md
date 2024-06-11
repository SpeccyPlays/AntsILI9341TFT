This project was based on the automous agents sections of the Nature of Code
https://natureofcode.com/autonomous-agents/

I wanted the added challenge of implementing this on an Arduino uno clone with limited stack and heap space.

The 'ants' act in a similar way to boids except they don't swarm together, and wander around aimlessly.

They are state machines with differing behavours :

A wanering mode - ants move around the screen, try to avoid each other, and the aim was to give them a more natual movement

A food mode - touching the screen will add a 'food' circle. If an ant walks within the circle, it will head back to the base.

A predator mode - one ant will be randomly picked as a predator and the others will try to avoid it.

A follow mode - one ant will be randomly picked as a leader and the others will follow them.

There's an auto food mode which will add food to the screen after a certain time period (only one food item will show at a time)
