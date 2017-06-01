# Rhynos

## Fixing all of the problems with cocos in DMVR

From 1.2 GB to 75MB!

To run on Linux:
tools/bin/cocos run -s Rhynos -p linux

Replace linux with mac for compile & run on a Mac computer

# Design Document for We Are DMVR

## Project Name

Department of Mammal Vehicles, Rhino edition

## Group Members

Lee Ehudin, Steven Hernandez, Patrick Dougherty, Michelle Ross, Long Pham

## Gameplay Overview (controls, graphics, scoring, etc.)

The game we have designed is a 2D side-scrolling racing game between a rhino
and an AI (or other human-controlled animals) with a twist.  On any one
scene/level, there are 3 different parallel tracks that the player can switch
at the touch of a button to overcome obstacles that would be impossible to
cross on just one path. The levels will incorporate different terrains and
obstacles.

The first object of the game is to be the first to the finish line.
Alternative game play modes include racing against time and getting high scores
from collecting items.

### Controls

For controls, we will map x-axis movement to the left and right arrow keys and
jump to Space, but we’d like them to be configurable (stretch goal).  Two
additional buttons (probably Q and E) will be used to cycle between tracks.
Major Architectural Components (graphics engine, AI agents, networking, ...)
The major components we will be implementing and integrating into our game
include a bit graphic engine, a game state manager for the game world, at least
one AI agent, networking across computers, a basic 2D physics engine largely
concerned with jumping and acceleration, controls for the user, and reading
from file formats to parse and render tracks.  All components will be
implemented in C++.

### Graphics Engine

The graphics engine will be responsible for generating graphics so that the
player can view the game world.  The input to the graphics engine will come
from the game state manager.  This input will be the state of the game,
including the track and the locations of the player(s) and AI(s).  The output
of the graphics engine will be rendered graphics that the player can see.  We
plan on using Oxygine to implement our graphics engine.

### Game State Manager

The game state manager will be responsible for keeping track of the state of
the game world at every update cycle.  It is also responsible for initializing
the state of the game world from the track supplied by the file parser.  It
will accept input from the physics engine, and the file parser.  It will output
the current state of the game at every update cycle to the graphics engine, AI
agent, and physics engine.  It will also output the actions of the current
player to the networking component if the game is in multiplayer mode.

### AI Agent

The AI Agent is responsible for planning out and executing the action of each
AI racer in the game.  The initial implementation of the AI will be supplied a
path for each AI racer based on the current track, and the AI racers will
attempt to follow the paths as closely as possible, at a set speed for each
portion of the path.  At each update cycle, the game state manager will send
the state of the game to the AI agent.  The AI agent will then process the next
moves for each of the AI racers based on the supplied AI path and any obstacles
in that path.  It will send these next moves to the physics engine so that it
updates the state of the AI racers.

### Networking Manager

The networking manager is used to communicate with other players while playing
in multiplayer mode.  The networked game has a client-server architecture.
Players can opt to create a client or server in addition to the single-player AI
game.  Once two clients have connected to the same server, the networking game
begins, and clients see the updates from all other clients in their game through
input reflection from the server.

The protocol we use has only four messages:
  * CONNECTION_ESTABLISHED is the initial message the server sends to a client
    to let it know that it has successfully connected to the server and to
    assign it a unique id.
  * GAME_START is a message that is sent to all clients once two players have
    connected to the server. It contains a list of the id of every player
    currently connected, so the client and server have a shared way of
    communicating about actions from each player.
  * When the game has started and the client presses a key, it sends over the
    key code of that key press to the server.
  * KEY_PRESSED is a message that is send to every client when the server
    receives a notification from a client that it has pressed a key. This
    message contains the key code of the key that was pressed and the id of the
    player that pressed it.

We used asio as our networking library and cereal as our serialization library.
Both our server and clients communicate over TCP using synchronous (blocking)
function calls in multiple threads.

### Physics Engine

We used Box2D to implement our world. The main mechanic, layer swapping, was
achieved using an interesting feature of Box2D. Every object in Box2D has
category bits and mask bits. Category bits are used to define what an object
is. For example, you might have a category called "Players" and assign them to
category 1 (Binary: 0001). Thus anything that lives on the "Player" layer will
be given that category bit. Then, you have some other category like "Layer 1,"
(Binary: 0010). Objects that exclusively reside in Layer 1 (e.g. terrain,
obstacles) will have category 0010.

In addition to category bits, each object also has mask bits. These mask bits
are used to indicate what the object will collide with. Thus to achieve layer
swapping, we just have to adjust the player's mask bits. If we want the player
to collide with layer 1, then  we adjust it's mask bits to include layer 1's
category bits (0010). So if the player has mask bits 0010 (the player wants to
collide with layer 1) and the objects in layer 1 have mask bits 0001 (the layer
wants to collide with the player), then we will have a collision. For a
collision to occur, both objects have to agree to collide. Thus, we can set the
layers to always collide with the player, and then manually set on the player
which layers it can collide with.

### User Controls

This component is responsible for capturing user input and transforming it into
an update for the player that is sent to the physics engine.

### File Parser

This component is responsible for reading in files in a track format and
loading this initial track into the game state manager.  This component allows
users to specify their own game tracks.

## Interactions Between Components

The main state manager will make some data available to the various engines. It
will also track overall world data and miscellaneous items like score and time.

The graphics engine will read from the state manager to render the scene. It
will need access to almost all of the state.

The physics engine will handle user input and the continuous interaction
between the player and the world. It will need to read and modify the state.

The AI can have two forms. The first basic one will be able to just read from a
set of instructions and produce the necessary input to the physics engine. The
more advanced form will be able to react to the actual track, with some form of
decision making for handling obstacles and other events.

The network will handle transferring data between multiple computers, primarily
passing user inputs and state changes.

## Milestones

Step 1) Implement the Physics engine with basic gravity, velocity, jump,
acceleration, and braking features.  (Goal for Checkpoint 1)

Step 2) Create a world state manager with classes for tracks, user, locations,
paths etc.

Step 3) Implement functions to transform world states into graphic
visualizations. (Goal for Checkpoint 2)

Step 4) Finetune user input to let the user control the main character.

Step 5) Design and implement a simple AI agent to race against a player (Goal
for Checkpoint 3).

Step 6) Allow a host player to run the application and invite remote players to
join race (networking) and finish a more advanced AI.

Step 7) Work on stretch goals. (Goal for Final Presentation) Group Member Roles
For the first half of our implementation, we will need to cover the Physics,
Graphics, Data, and Assets roles. For the second half, we will be focusing on
Sound, AI, Networking, and Code Style.

# Individual Work

Michelle: graphics, assets, sound

Steven: physics, data

Lee: graphics, networking

Long: AI

Patrick: physics, AI

# Other

Stretch Goals
Items/powerups
Other animals
Smart respawn
Fancy animations to describe the world
Up to 4 people at once (instead of just 2)
Smart AI using all of our tests as training data
Story mode ish
Smoother and better physics (especially acceleration)
Better networking
Rhino drifting
Make your own tracks
Save scores
Gamecube controllers
User configurable control scheme
Different difficulty levels
2.5D graphics
Race other players and AI at the same time
Coding Style (naming conventions, source code organization, documentation conventions, etc.)

Git will be used for version control.
clang-format will be used for code formatting.
Doxygen can be used to generate HTML documentation for the project.
We will be following the coding conventions outlined in the Google C++ Style
Guide
