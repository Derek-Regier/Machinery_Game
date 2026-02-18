#include "cond.h"

bool player_take_damage(Player *player, int damage) {
    player->health -= damage;
    return player->health <= 0; // Returns true if player is dead
}

void update_health_HUD(Player *player){
    /*TODO: modify HUD bitmap according to player health*/
}
void boss_summon(Boss *boss){
    
    if (summon(boss) && !boss->summoned){
        /*
            Bring more enemies onto screen
        */
    }
}
void player_hits_enemy(Model *model){
    if (!player_is_attacking(&model->player)){
        return;
    }
    
    /* Now check collision with enemies */
    for (int i = 0; i < 1; i++){ // 1 will be replaced with total enemies
        if (model->enemy[i].health > 0) {
            if (player_hitbox_overlaps(&model->player,
                                       model->enemy[i].x,
                                       model->enemy[i].y,
                                       model->enemy[i].w,
                                       model->enemy[i].h)){
                
                int damage = light_attack(&model->player);
                update_enemy_health(&model->enemy[i], -damage);
                /* TODO: sound, animation */
            }
        }
    }
}
bool enemy_hits_player(Model *model){
    /* Check collision with all enemies */
    bool player_died = false;
    for (int i = 0; i < 1; i++){
        Enemy *enemy = &model->enemy[i];
        
        /* Skip if enemy is dead or not attacking */
        if (enemy->health <= 0 || !enemy->is_attacking){
            continue;
        }
        
        /* Check if enemy overlaps player */
        if (enemy_hitbox_overlaps(&model->enemy, 
                                   player->x, player->y, 
                                   player->w, player->h)){
            
            int damage = enemy_attack(enemy);
            bool player_died = player_take_damage(&model->player, damage);
            
            update_health_HUD(&model->player);
            
        }
    }
    return player_died;
}

void player_dies(){
    /* call level end and change game state to restart to menue, 
    this function will be called on conditions like when enemy_hits_player so no implementation for now*/
}

bool level_end(Player *player){
    /* Win: Boss defeated */
    if (model->boss.health <= 0){
        /* TODO: Display "You Win" */
        return true;
    }
    
    /* Lose: Player died */
    if (model->player.health <= 0){
        /* TODO: Restart game */
        return true;
    }
    
    return false;  /* Game continues */
}

