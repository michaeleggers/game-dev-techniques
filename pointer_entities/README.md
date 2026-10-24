## Why entities storing pointers to other entities can (will) cause problems.

No. It is not because a `nullptr` dereference (although still can happen).
The [`EntityManager`](./entity.h) has control over the
memory and it stays valid throughout the lifetime of the game.
This demo shows how gameplay bugs can arise when entities store
references to each other by using pointers.

The problem becomes obvious when an entity stores a reference to
another entity, for example, entity A wants to follow entity B.
In this game two such follow-behaviors are possible:

1. The player (red) gets too close to an enemy (yellow): The enemy
   starts to hunt the player.
2. The turrets (black) fire bullets (pink) at any moving
   entity that gets within their firing range. The bullets
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
