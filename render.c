#include "render.h"
#include "raster.h"
#include "bitmaps.h"

void render(const Model *model, void *base){

  render_player($model->player, base);
  render_enemy($model->enemy, base);
  
}

void render_player(const Player *player, UINT32 *base){
  pbm32(base, player->y, player->x, player, 32); 
}

void render_enemy(const Enemy *enemy, UINT32 *base){
  pbm32(base, enemy->y, enemy->x, enemy, 32);
}

void render_boss(const Boss *boss, UINT32 *base){
  int r, w;
   for (r = 0; r < 128; r++){
        for (w = 0; w < 4; w++){
            pbm32(base, boss->y + r, boss->x + (w * 32), &boss[r][w], 1);}
        } 
}

void render_item(const Item *item, UINT16 *base){
  pbm16(base, item->y, item->x, hp_pot, 16);
}

void render_health_bar(const *health_bar, UINT32 *base){
  pbm32(base, 4, 4, health_bar, 32);
}


