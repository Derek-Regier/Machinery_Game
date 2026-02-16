typedef struct {
    unsigned int x, y;
    int delta_x;
    int delta_y;
    int health;
    int damage;
    bool is_attacking;
    unsigned int attack_cooldown;
    unsigned int w, h;
} Player;

void move_player_vertical(Player *player);

void move_player_horizontal(Player *player);

bool update_health(Player *player, int x);

void update_damage(Player *player, int x);

int light_attack(const Player *player);

bool player_is_attacking(const Player *player);

void player_set_attacking(Player *player, bool attacking, const int cooldown);

bool player_hitbox_overlaps(const Player *player, int x, int y, int width, int height);
