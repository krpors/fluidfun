# How to use fluidfun! #

## What is this thing? ##
Fluidfun is a fun environment in which you experiment with various types of particles (elements) interacting. Some of the elements have fluid-like properties, some have solid or gas-like properties. Along with the standard elements, you can also create your own by editing the configuration file.


## Key guide ##
As of yet there is no graphical interface for doing most of the interactions. For now, the entire program is key-driven. Here is a list of the hotkeys you will need to control the environment:
  * Directional keys: Move cursor (holding shift key increases speed)
  * Page up / Page down keys: Change selected element
  * Return key: Emit selected element
  * Space key: Draw wall
  * "h" key: Draw hot block
  * "c" key: Draw cold block
  * "w", "a", "s", "d" keys: Draw directional blocks
  * "v" key: Draw void block (eats particles)
  * "z" key: Draw particle emitter (emits selected element)
  * "x" key: Erases drawn elements (but not particles)
I know it's a lot to remember, but a GUI is on the way! If you'd like to help with the GUI, please feel free to contribute.

## How to create new elements ##
Playing with the pre-made elements is fun, but wouldn't it be funner if you could make your own? Well, you can! The configuration file is located in the data folder (./data/config.txt) All of the pre-made elements are scripted out in this file. Try studying them to get a feel for how the configuration works. Making small changes to the pre-made elements is a good way to learn what's going on.

Here is a quick reference to the command functions:
  * newElement(""): Creates a new element by name (all the commands following apply to this)
  * enableEmit(): Particle emits energy (set emission with "setEmit")
  * enableBias(): Enable bias for particle energy (the bias is set with "setBias")
  * enableSharing(): Allow particle to share energy with its neighbors
  * enableMorphing(): Allow particle to morph between high and low energy states
  * enableClumping(): Enable clumping behavior
  * setRGB0(R, G, B): Set particle color for low energy state
  * setRGB1(R, G, B): Set particle color for high energy state
  * setGravity(l, h): Set gravity for low and high energy states (negative is antigravity)
  * setSize(l, h): Set particle size for low and high energy states
  * setFriction(l, h): Set friction for low and high energy states (inverse friction)
  * setEmit(e): Set energy for particle to emit (it's neighbors will absorb it)
  * setBias(b): Set bias energy level (particle will settle at this energy level)
  * setColdState(""): Set element to transition into when energy level hits 0.0
  * setHotState(""): Set element to transition into when energy level hits 1.0
  * addReaction(A, B, X): When the particle hits the element type A it transitions into the element type B with a 1 in X chance of transition.
You're better off learning from the pre-made script :)