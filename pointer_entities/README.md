## Why entities storing pointers to other entities can (will) cause problems.

No. It is not because a `nullptr` dereference (although still can happen).
The [`EntityManager`](./entity.h) has control over the
memory and it stays valid throughout the lifetime of the game.
This demo shows how gameplay bugs can arise when entities store
references to each other by using pointers.


## The game rules

The demo game consists of the following rules:
- Player entity (red) has 100 health-point (green bar),
  controllable via keyboard (W,A,S,D).
- Enemies (yellow): Start to hunt the player once the
  player gets too close to them. If the player touches
  an enemy the player looses one healthpoint.
  If an enemy dies, a new one spawns at a random location.
- Turrets (black, one at each corner, one in the middle):
  Once the player or an enemy gets too close to them
  they start to fire homing projectiles. The projectiles
  lock on to the closest enemy and starts to follow
  them until they either reach them (hit entity looses healthpoints),
  or the projectile's time to live is <= 0.0.
- Pickups (green): At regular intervals green pickup items
  spawn in the world. If the player reaches them before
  they despawn the player's speed increases by 5%.
- There is no winning condition for the player. When the
  player dies, the simulation carries on.

## The problem

The problem becomes obvious when an entity stores a reference to
another entity, for example, entity A wants to follow entity B.
In this game two such follow-behaviors are possible:

1. The player (red) gets too close to an enemy (yellow): The enemy
   starts to hunt the player.
2. The turrets (black) fire bullets (pink) at any moving
   entity (enemy or player) that gets within their firing range. The bullets
   are homing missiles that lock on to a target.

The `EntityManager` just holds a fixed-sized array of `MAX_ENTITIES`.
Let's say at timepoint t the array looks like this:
| Slot | Entity-Type |
| ----------- | ----------- |
| 0 | Player |
| 1 | Enemy | 
| 2 | Enemy | 
| 3 | Turret | 
| 4 | Turret | 
| 5 | Turret | 
| 6 | Turret | 
| 7 | Turret | 
| 8 | free | 
| 9 | free | 
...
| MAX_ENTITIES-1 | free |




## References:

This demo is based on the article by Andre Weissflog: 
[Handles are the better pointers](https://floooh.github.io/2018/06/17/handles-vs-pointers.html).
