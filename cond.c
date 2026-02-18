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
void enemy_hits_player(Model *model,int enemy_num);
bool level_end(Player *player);
