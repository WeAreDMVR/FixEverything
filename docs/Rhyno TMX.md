# Rhyno TMX
## Layers
Every TMX map consists of many layers of tiles and objects. A Rhyno TMX map
follows the following convention of 4-layer groupings to convey track data:

 - **bgx** : the background layer where the majority of the terrain and visuals
   are drawn. Should be rendered behind any object sprites.
 - **fgx** : the foreground layer where visual flourishes and static object
   sprites can be added
 - **metax** : the annotation layer which groups the map into tile types such as
   "platform", "pit", etc. This layer is invisible.
 - **objx** : unlike the other three, an _object layer_ where sprites a defined

**NOTE**: It's especially important that you keep to this naming convention. The
meta layers are loaded _by name_, so if they aren't named "meta1", "meta2", or
"meta3" then they don't get sent to the physics engine!

### metaX Layer
A Level loads the meta layers of a given TMX map by calling the function
**Level::loadLayers()**, which itself calls **Level::createFixtures()**. This
function iterates over every tile in the meta layer, creates a _pSprite_ for
each, and inserts that _pSprite_ into the Level's physic's world
(**Level::_world**) as a rectangular fixture.

Because meta layer tiles are destined to become objects in the physics
simulation there are a number of properties that they must have in order to
behave well with our engine. Here is the list of essential properties for meta
tiles (subject to expansion):

| Name         | Type | Description |
| ------------ | ---- | ----------- |
| CategoryBits | int  | an int representation of a byte corresponding to the object's **FilterClass** (see pSprite.h) |
| BodyType     | int  | an int corresponding to the b2BodyType of the tile: 0 = **static**, 1 = **kinematic**, and 2 = **dynamic** |
| Damage       | int  | the amount of damage this tile does to **destructable** sprites upon collision |
| Destructable  | bool | whether or not an object takes damage when a collision occurs |
| Floating      | bool | whether or not an object is affected by gravity |

If you were creating a standard floor tile on layer 1, for example, you would
want to set the following values:

> CategoryBits : 1
> BodyType: 0
> Damage : 0

A standard pit-box on layer 3 would have the following values:
> CategoryBits: 5
> BodyType: 0
> Damage : 999

More properties will likely be defined for the meta layer to satisfy the needs of AI.

### objX Layer
A Level loads all other sprites using the function **Level::loadObjects()**
which itself calls **Level::addObject()**. Each object defined in the obj layer
is given a sprite which is loaded by calling
**Sprite::createWithSpriteFrameName()**. What's going on here?

This guide probably explains things better than I could:
[Cocos2D Sprites
Guide](http://www.cocos2d-x.org/docs/programmers-guide/sprites/index.html)

In short, we're loading sprites from a sprite sheet. There are a number of tools
online for creating sprite sheets like ShoeBox and Texture Packer. Once you have
a sprite sheet, you need to load it into cocos2d's Sprite Frame Cache:

    `auto spritecache = SpriteFrameCache::getInstance();`
    `spritecache>addSpriteFrameWithFile("sprites.plist");`

Once that's done, you just load the sprite by image name, as usual.

Like meta tiles, objects have several essential properties, listed below:

| Name         | Type | Description |
|--------------|------|-------------|
| CategoryBits | int  |an int representation of a byte corresponding to the object's **FilterClass** (see pSprite.h)|
| BodyType     | int  |an int corresponding to the b2BodyType of the tile: 0 = **static**, 1 = **kinematic**, and 2 = **dynamic**|
| Damage       | int  |the amount of damage this tile does to **destructable** sprites upon collision|
|name          | string | the file name of the sprite's image (must be in the sprite sheet)|
|type          | string | the **className** of the sprite (ex. "Player")|
|x             | int    |default x position|
|y             | int    |default y position|


The Player sprite also must have the following properties:

| Name         | Type | Description |
|--------------|------|-------------|
|Acceleration  | float |the acceleration of the player|
|Deceleration  | float |the deceleration of the player|
| Health       | int  |the max health of the player|
|JumpAcceleration| float | the jump acceleration  of the player|
|MaxSpeed| float |the x speed cap of the player|
|MaxJumpTime| float |the max time the jump button can be held before the player descends|

