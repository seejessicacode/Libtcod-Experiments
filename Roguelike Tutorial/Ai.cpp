#include "main.h"
#include <math.h>

static const int TRACKING_TURNS = 3;

void PlayerAi::update(Actor *owner)
{
    if(owner->destructible && owner->destructible->isDead())
    {
        return;
    }

    int dx = 0, dy = 0;
    switch(engine.lastKey.vk)
    {
        case TCODK_UP : dy = -1; break;
        case TCODK_DOWN : dy = 1; break;
        case TCODK_LEFT : dx = -1; break;
        case TCODK_RIGHT : dx = 1; break;
        default: break;
    }

    if(dx != 0 || dy != 0)
    {
        engine.gameStatus = Engine::NEW_TURN;
        if(moveOrAttack(owner, owner->x + dx, owner->y + dy))
        {
            engine.map->computeFov();
        }
    }
}

bool PlayerAi::moveOrAttack(Actor *owner, int targetx, int targety)
{
    if(engine.map->isWall(targetx, targety))
        return false;

    for(Actor **iterator=engine.actors.begin(); iterator != engine.actors.end(); iterator++)
    {
        Actor *actor = *iterator;
        if(actor->destructible && !actor->destructible->isDead() && actor->x == targetx && actor->y == targety)
        {
            owner->attacker->attack(owner, actor);
            return false;
        }
    }

    for(Actor **iterator=engine.actors.begin(); iterator != engine.actors.end(); iterator++)
    {
        Actor *actor = *iterator;
        if(actor->destructible && actor->destructible->isDead() && actor->x == targetx && actor->y == targety)
        {
            engine.gui->message(TCODColor::lightGrey, "There's a %s here", actor->name);
        }
    }

    owner->x = targetx;
    owner->y = targety;
    return true;
}

void MonsterAi::update(Actor *owner)
{
    if(owner->destructible && owner->destructible->isDead())
    {
        return;
    }

    if(engine.map->isInFov(owner->x, owner->y))
    {
        moveCount = TRACKING_TURNS;
    }
    else
    {
        moveCount--;
    }

    if(moveCount > 0)
    {
        moveOrAttack(owner, engine.player->x, engine.player->y);
    }
}

void MonsterAi::moveOrAttack(Actor *owner, int targetx, int targety)
{
    // distance = square root of ((x2 - x1)^2 + (y2 - y1)^2)
    int dx = targetx - owner->x;
    int dy = targety - owner->y;
    int stepdx = (dx > 0 ? 1 : -1);
    int stepdy = (dy > 0 ? 1 : -1);
    float distance=sqrtf(dx * dx + dy * dy);

    // Is in melee range?
    if ( distance >= 2 )
    {
        // the possible values for dx,dy are -1,0 and 1
        dx = (int)(round(dx/distance));
        dy = (int)(round(dy/distance));

        if(engine.map->canWalk(owner->x + dx, owner->y + dy))
        {
            owner->x += dx;
            owner->y += dy;
        }
        else if(engine.map->canWalk(owner->x + stepdx, owner->y))
        {
            owner->x += stepdx;
        }
        else if(engine.map->canWalk(owner->x, owner->y + stepdy))
        {
            owner->y += stepdy;
        }
    }
    else if(owner->attacker)
    {
        owner->attacker->attack(owner, engine.player);
    }
}