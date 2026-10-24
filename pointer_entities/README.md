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

## References:

This demo is based on the article by Andre Weissflog: 
[Handles are the better pointers](https://floooh.github.io/2018/06/17/handles-vs-pointers.html).
