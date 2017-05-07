# Physics Info

## Box 2D Objects

Every thing in our game that needs to have physics calculated for it must exist
in the Box2D world stored in Level->\_world. This primarily includes anything
that is affected by gravity, forces, or collisions. That would be players,
ground, obstacles, power-ups, etc. All of these objects are represented in the
Box2D world as small shapes called fixtures. Most of the time, these are
rectangles, but they could be many other shapes. These fixtures represent the
bodies of the objects. So our main player will move around and collide with
things as if it were a rectangle and not whatever the sprite looks like. Thus,
we aim to have our physics and graphics line up closely so that collisions
feel right.

## Category and Mask Bits

Because we are going to be simulating three tracks, we need a way to
distinguish which objects should and should not collide. There are two related
tools that help us here, category and mask bits. Category bits are a definition
of what an object is. Take for example our game. Think of two tiles in the same layer. There is never a time that something should collide with one but not the
other. So each layer is its own category. Now consider the player. At any given
time, the player may or may not be interacting with any given layer. Thus the
players have to exist in a separate category.

Now that we have separated our fixtures into categories, we can specify what
should interact with what. Objects on layer 1 should collide with all other
objects on layer 1. So we set the mask bit to match the layer 1 bits. Also,
there are times when the player would collide with layer 1, so lets include
the player bits. We do the same for the other two layers. Now, to switch
layers, all we need to do is adjust the player's mask.

This is implemented as a simple bit operation. So let's set up some constants

layer1Bits = 0x0001; (0001 in binary)
layer2Bits = 0x0002; (0010 in binary)
layer3Bits = 0x0004; (0100 in binary)
playerBits = 0x0008; (1000 in binary)

Now, to tell if two things collide, we just do a bit comparison:

layer1Block.categoryBits = layer1Bits;
layer1Block.maskBits = layer1Bits | playerBits;

player.categoryBits = playerBits;
player.maskBits = layer1Bits;

if (layer1Block.categoryBits & player.maskBits
    && player.categoryBits & layer1Block.maskBits) { 
  collide
} else {
  don't collide
}

Since they both must agree, we can just adjust the players mask to switch
layers.
