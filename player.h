typedef struct {
    unsigned int x, y;
    int delta_x;
    int delta_y;
    int health;
    int damage;
    unsigned int h,w;
} Player;

void move_player_vertical(Player *player);

void move_player_horizontal(Player *player);

bool update_health(Player *player, int x);

void update_damage(Player *player, int x);

int light_attack(Player *player);

