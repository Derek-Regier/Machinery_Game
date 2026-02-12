typdef struct {
    unsigned int x, y;
    int delta_x;
    int delta_y;
    int health;
    int damage;
    unsigned int h,w;
} Player;

void move_player_vertical(Player *player);

void move_player_horizontal(Player *player);

bool update_health(Player *player);

void take_damage(Player *player);

void restore_health(Player *player);
