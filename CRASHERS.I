# 1 "crashers.c" 1
# 1 "./crashers.h" 1
# 1 "C:/c68/include/stdio.h" 1
# 1 "C:/c68/include/compiler.h" 1
# 11 "C:/c68/include/stdio.h" 2
# 20 "C:/c68/include/stdio.h"
typedef unsigned long  size_t;
# 69 "C:/c68/include/stdio.h"
typedef	struct
	{
	long		_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	unsigned int	_flag;
	int		_file;
	long		_bsiz;
	unsigned char	_ch;
	}
	FILE;


typedef unsigned long fpos_t;
# 92 "C:/c68/include/stdio.h"
extern	FILE	_iob[];
# 108 "C:/c68/include/stdio.h"
 char *	ctermid	(char *) ;




 int	remove	(const char *) ;
 int	rename	(const char *, const char *) ;
 char *	tmpnam	(char *) ;
 FILE *	tmpfile	(void) ;

 int	fclose	(FILE *) ;
 int	fflush	(FILE *) ;

 FILE *	fopen	(const char *, const char *) ;
 FILE *	freopen	(const char *, const char *, FILE *) ;

 void	setbuf	(FILE *, char *) ;
 int	setvbuf	(FILE *, char *, int, size_t) ;
# 132 "C:/c68/include/stdio.h"
 int  fscanf  (FILE *, const char *, ...) ;
 int  scanf   (const char *, ...) ;
 int  sscanf  (const char *, const char *, ...) ;


 int	fprintf	(FILE *, const char *, ...) ;
 int	printf	(const char *, ...) ;
 int	sprintf	(char *, const char *, ...) ;

 int 	vfprintf (FILE *, const char *, char * ) ;
 int 	vprintf	 (const char *, char * ) ;
 int 	vsprintf (char *, const char *, char * ) ;

 int	vscanf  (const char *, char * ) ;
 int	vfscanf (FILE *, const char *, char * ) ;
 int	vsscanf	(const char *, const char *, char * ) ;


 int	fgetc	(FILE *) ;
 char	*fgets	(char *, int, FILE *) ;
 char	*gets	(char *) ;
 int	fputc	(int c, FILE *) ;
 int	fputs	(const char *, FILE *) ;
 int	puts	(const char *) ;

 size_t	fread	(void *, size_t, size_t, FILE *) ;
 size_t	fwrite	(const void *, size_t, size_t, FILE *) ;

 int	fgetpos	(FILE *, fpos_t *) ;
 int	fsetpos	(FILE *, fpos_t *) ;

 int	fseek	(FILE *, long, int) ;
 long	ftell	(FILE *) ;
 void	rewind	(FILE *) ;

 void	perror	(const char *) ;


 FILE	*fdopen	(int, const char *) ;


 FILE *	fopenp	(const char *, const char *) ;
 int 	fungetc	(int, FILE *) ;
 int	pclose	(FILE *) ;
 FILE *	popen	(const char *, const char *) ;
 void	setlinebuf	(FILE *) ;

 void	_binmode	(int) ;
 long 	getl	(FILE *) ;
 long 	putl	(long, FILE *) ;
 short 	getw	(FILE *) ;
 short 	putw	(short, FILE *) ;
 void	_getbuf	(FILE *fp) ;
# 192 "C:/c68/include/stdio.h"
 int	_filbuf	(FILE *) ;
# 4 "./crashers.h" 2
# 1 "C:/c68/include/osbind.h" 1
# 1 "C:/c68/include/ostruct.h" 1
# 22 "C:/c68/include/ostruct.h"
typedef struct {
    long b_free;
    long b_total;
    long b_secsiz;
    long b_clsiz;
} _DISKINFO;


typedef struct {
  short time;
  short date;
} _DOSTIME;


typedef struct
{
        unsigned char maxlen;
        unsigned char actuallen;
        char    buffer[255];
} _CCONLINE;
# 48 "C:/c68/include/ostruct.h"
typedef struct _dta {
    char 	    dta_buf[21];
    char            dta_attribute;
    unsigned short  dta_time;
    unsigned short  dta_date;
    long            dta_size;
    char            dta_name[14];
} _DTA;
# 98 "C:/c68/include/ostruct.h"
typedef struct {
  short recsiz;
  short clsiz;
  short clsizb;
  short rdlen;
  short fsiz;
  short fatrec;
  short datrec;
  short numcl;
  short bflags;
} _BPB;




typedef struct _md {
    struct _md	*md_next;
    long	 md_start;
    long	 md_length;
    long	 md_owner;
} _MD;


typedef struct {
    _MD *mp_free;
    _MD *mp_used;
    _MD *mp_rover;
} _MPB;
# 141 "C:/c68/include/ostruct.h"
typedef struct {
    char    *ibuf;
    short   ibufsiz;
    volatile short   ibufhd;
    volatile short   ibuftl;
    short   ibuflow;
    short   ibufhi;
} _IOREC;


typedef struct {
	char	topmode;
	char	buttons;
	char	xparam;
	char	yparam;
	short	xmax;
	short	ymax;
	short	xstart;
	short	ystart;
} _PARAM;


typedef struct {
    void    (*midivec)	(void) ;
    void    (*vkbderr)	(void) ;
    void    (*vmiderr)	(void) ;
    void    (*statvec)	(void *) ;
    void    (*mousevec)	(void *) ;
    void    (*clockvec)	(void *) ;
    void    (*joyvec)	(void *) ;
    long    (*midisys)	(void) ;
    long    (*ikbdsys)	(void) ;
    char    kbstate;
} _KBDVECS;


typedef struct {
    void *unshift;
    void *shift;
    void *caps;
} _KEYTAB;


typedef struct
{
        void    *pb_scrptr;
        int     pb_offset;
        int     pb_width;
        int     pb_height;
        int     pb_left;
        int     pb_right;
        int     pb_screz;
        int     pb_prrez;
        void    *pb_colptr;
        int     pb_prtype;
        int     pb_prport;
        void    *pb_mask;
} _PBDEF;
# 17 "C:/c68/include/osbind.h" 2
# 33 "C:/c68/include/osbind.h"
 long _trap_1_w		(short) ;
 long _trap_1_ww	(short,short) ;
 long _trap_1_wl	(short,long) ;
 long _trap_1_wlw	(short,long,short) ;
 long _trap_1_wwll	(short,short,long,long) ;
 long _trap_1_wlww	(short,long,short,short) ;
 long _trap_1_www	(short,short,short) ;
 long _trap_1_wll	(short,long,long) ;
 long _trap_1_wwlll	(short,short,long,long,long) ;
 long _trap_1_wwwll	(short,short,short,long,long) ;
 long _trap_13_wl	(short,long) ;
 long _trap_13_w	(short) ;
 long _trap_13_ww	(short,short) ;
 long _trap_13_www	(short,short,short) ;
 long _trap_13_wwlwww	(short,short,long,short,short,short) ;
 long _trap_13_wwl	(short,short,long) ;
 long _trap_14_wwl	(short,short,long) ;
 long _trap_14_wwll	(short,short,long,long) ;
 long _trap_14_ww	(short,short) ;
 long _trap_14_w	(short) ;
 long _trap_14_wllw	(short,long,long,short) ;
 long _trap_14_wl	(short,long) ;
 long _trap_14_www	(short,short,short) ;
 long _trap_14_wllwwwww (short,long,long,short,short,short,short,short) ;
 long _trap_14_wllwwwwlw (short,long,long,short,short,short,short,long,short) ;
 long _trap_14_wllwwwwwlw (short,long,long,short,short,short,short,short,long,short) ;
 long _trap_14_wwwwwww	(short,short,short,short,short,short,short) ;
 long _trap_14_wlll	(short,long,long,long) ;
 long _trap_14_wllww	(short,long,long,short,short) ;
 long _trap_14_wwwwl	(short,short,short,short,long) ;
 long _trap_14_wwwl	(short,short,short,long) ;
 long _trap_14_wlwlw	(short,long,short,long,short) ;
# 103 "C:/c68/include/osbind.h"
 long _trap_1_ 	(short,...) ;
 long _trap_14_ 	(short,...) ;
 long _trap_13_ 	(short,...) ;
# 5 "./crashers.h" 2
# 1 "./render.h" 1
# 1 "./types.h" 1
# 14 "./types.h"
typedef unsigned char UINT8;
typedef unsigned int  UINT16;
typedef unsigned long UINT32;

typedef char bool;
# 15 "./render.h" 2
# 1 "./model.h" 1
# 1 "./player.h" 1
# 1 "./types.h" 1
# 12 "./player.h" 2


typedef struct {
    unsigned int x, y;
    int delta_x;
    int delta_y;
    int health;
    int damage;
    bool is_attacking;
    unsigned int item_cooldown;
    unsigned int attack_cooldown;
    unsigned int h,w;
} Player;





void move_player_vertical(Player *player);





void move_player_horizontal(Player *player);
# 43 "./player.h"
bool update_health(Player *player, int x);





void update_damage(Player *player, int x);
# 56 "./player.h"
int light_attack(const Player *player);





bool player_is_attacking(const Player *player);





void player_set_attacking(Player *player, bool attacking, const int cooldown);





bool player_hitbox_overlaps(const Player *player, int x, int y, int width, int height);
# 11 "./model.h" 2
# 1 "./enemy.h" 1
# 1 "./types.h" 1
# 12 "./enemy.h" 2


typedef struct {
    unsigned int x, y;
    int delta_x;
    int delta_y;
    int health;
    int damage;
    unsigned int w, h;
    bool is_attacking;
    bool active;
} Enemy;
# 30 "./enemy.h"
void move_enemy_vertical(Enemy *enemy);





void move_enemy_horizontal(Enemy *enemy);
# 43 "./enemy.h"
bool update_enemy_health(Enemy *enemy, int x);
# 50 "./enemy.h"
void update_enemy_damage(Enemy *enemy, int x);





int enemy_attack(const Enemy *enemy);





bool enemy_hitbox_overlaps(const Enemy *enemy, int x, int y, int width, int height);
# 12 "./model.h" 2
# 1 "./boss.h" 1
# 1 "./types.h" 1
# 12 "./boss.h" 2


typedef struct {
    unsigned int x, y;
    int delta_x;
    int delta_y;
    int health;
    int damage;
    bool summoned;
    int max_health;
    bool is_attacking;
    bool active;
    unsigned int w, h;
} Boss;





void move_boss_vertical(Boss *boss);





void move_boss_horizontal(Boss *boss);





bool update_boss_health(Boss *boss, int x);





void update_boss_damage(Boss *boss, int x);





int boss_attack(Boss *boss);





bool summon(Boss *boss);
# 68 "./boss.h"
bool boss_hitbox_overlaps(const Boss *boss, int x, int y, int width, int height);
# 13 "./model.h" 2
# 1 "./item.h" 1
# 12 "./item.h"
typedef struct {
    unsigned int x, y;
    int h,w;
    int value;
} Item;





int get_value(Item *item);
# 14 "./model.h" 2
# 1 "./types.h" 1
# 15 "./model.h" 2
# 1 "./health.h" 1
# 12 "./health.h"
typedef struct {
    unsigned int x, y;
    int h,w;
    int value;
} Healthbar;





int update_value(Healthbar *healthbar, int value);
# 16 "./model.h" 2





typedef struct {
    Player player;
    Healthbar healthbar;
    Enemy enemy[16 ];
    int enemy_count;
    Boss boss;
    Item item[4 ];
    int stage;
    bool quit;
} Model;
# 39 "./model.h"
void init_model(Model *model);
void reset_movement(Model *model);
# 16 "./render.h" 2
# 1 "./raster.h" 1
# 1 "./types.h" 1
# 3 "./raster.h" 2
# 19 "./raster.h"
void clear_screen(UINT32 *base);

void clear_region(UINT32 *base, UINT16 row, UINT16 col,
                  UINT16 length, UINT16 width);

void plot_pixel(UINT8 *base, UINT16 row, UINT16 col);

void plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col,
                          UINT16 length);

void plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col,
                        UINT16 length);

void plot_line(UINT32 *base, UINT16 start_row, UINT16 start_col,
               UINT16 end_row, UINT16 end_col);

void plot_rectangle(UINT32 *base, UINT16 row, UINT16 col,
                    UINT16 length, UINT16 width);

void plot_square(UINT32 *base, UINT16 row, UINT16 col, UINT16 side);





void plot_triangle(UINT32 *base, UINT16 row, UINT16 col,
                   UINT16 base_len, UINT16 height, UINT8 direction);





void pbm8(UINT8 *base, UINT16 row, UINT16 col,
                   const UINT8 *bitmap, UINT16 height);

void pbm16(UINT16 *base, UINT16 row, UINT16 col,
                    const UINT16 *bitmap, UINT16 height);

void pbm32(UINT32 *base, UINT16 row, UINT16 col,
                    const UINT32 *bitmap, UINT16 height);





void plot_character(UINT8 *base, UINT16 row, UINT16 col,
                    char ch, const UINT8 *font_table);

void plot_string(UINT8 *base, UINT16 row, UINT16 col,
                 const char *str, const UINT8 *font_table);
# 17 "./render.h" 2
# 1 "./bitmaps.h" 1
# 1 "./types.h" 1
# 19 "./bitmaps.h" 2


extern UINT8 square_bitmap[8];

extern UINT16 rect_bitmap[16];

extern UINT32 tri_bitmap[32];

extern UINT32 player_bitmap[64];

extern UINT32 health_bar_bitmap[12];

extern UINT32 enemy_bitmap[64];

extern UINT32 boss_bitmap[128][4];

extern UINT16 hp_pot_bitmap[16];
# 18 "./render.h" 2
# 1 "./font.h" 1
# 1 "./types.h" 1
# 9 "./font.h" 2


extern const UINT8 font[];
# 19 "./render.h" 2





void render(const Model *model, UINT32 *base);





void render_player(const Player *player, UINT32 *base);





void render_enemy(const Enemy *enemy, Enemy *prev, UINT32 *base);
# 43 "./render.h"
void render_boss(const Boss *boss, UINT32 *base);





void render_item(const Item *item, UINT32 *base);





void render_healthbar(const Healthbar *healthbar, UINT32 *base);
# 6 "./crashers.h" 2
# 1 "./raster.h" 1
# 7 "./crashers.h" 2
# 1 "./model.h" 1
# 8 "./crashers.h" 2
# 1 "./synch.h" 1
# 1 "./player.h" 1
# 11 "./synch.h" 2
# 1 "./enemy.h" 1
# 12 "./synch.h" 2
# 1 "./boss.h" 1
# 13 "./synch.h" 2
# 1 "./model.h" 1
# 14 "./synch.h" 2
# 21 "./synch.h"
void update_player_position(Player *player);





void update_enemy_position(Enemy *enemy, const Player *player);





void update_boss_position(Boss *boss, const Player *player);





void update_player_cooldowns(Player *player);





void update_attack_cooldown(Player *player);





void update_item_cooldown(Player *player);

void spawn_enemy(Model *model, int stage);
# 62 "./synch.h"
void drop_item(Model *model, int stage);
# 9 "./crashers.h" 2
# 1 "./asynch.h" 1
# 1 "./player.h" 1
# 12 "./asynch.h" 2
# 1 "./enemy.h" 1
# 13 "./asynch.h" 2
# 1 "./boss.h" 1
# 14 "./asynch.h" 2
# 1 "./item.h" 1
# 15 "./asynch.h" 2



void on_light_attack(Player *player,int cooldown);
void move_player(Player *player, char key);
void on_use_item(Player *player, Item *item);
# 10 "./crashers.h" 2
# 1 "./cond.h" 1
# 1 "./model.h" 1
# 11 "./cond.h" 2
# 1 "./synch.h" 1
# 12 "./cond.h" 2
# 20 "./cond.h"
bool player_take_damage(Player *player, int damage);
# 29 "./cond.h"
void update_health_HUD(Player *player);
# 38 "./cond.h"
void boss_summon(Boss *boss, Model *model);
# 47 "./cond.h"
void player_hits_enemy(Player *player, Enemy *enemy);





void player_hits_boss(Player *player, Boss *boss);
# 61 "./cond.h"
bool enemy_hits_player(Enemy *enemy, Player *player);
bool boss_hits_player(Boss *boss, Player *player);





void player_dies(void);

bool next_level(const Model *model, int stage);
# 77 "./cond.h"
bool level_end(const Model *model);
# 11 "./crashers.h" 2
# 1 "./input.h" 1
# 19 "./input.h"
int has_input(void);
# 26 "./input.h"
char get_input(void);
# 12 "./crashers.h" 2
# 1 "./types.h" 1
# 13 "./crashers.h" 2
# 20 "./crashers.h"
UINT32 get_time(void);
# 31 "./crashers.h"
void process_async_event(Model *model, char key);

static void process_sync_events(Model *model);

static void process_cond_events(Model *model);
# 4 "crashers.c" 2
# 14 "crashers.c"
UINT8 screenBuffer[32255];

UINT32 get_time() {
    UINT32 *timer = (UINT32 *)0x462;
    UINT32 currTime;
    UINT32 oldSsp;

    oldSsp = _trap_1_wl((short)0x20,(long)(0)) ;
    currTime = *timer;
    _trap_1_wl((short)0x20,(long)(oldSsp)) ;

    return currTime;
}
# 33 "crashers.c"
void process_async_event(Model *model, char key)
{
    switch (key)
    {
        case 'w' :
        case 's' :
        case 'a' :
        case 'd' :
            move_player(&model->player, key);
            break;

        case 'j' :
            on_light_attack(&model->player,
                            model->player.attack_cooldown);
            break;

        case 'e' :
            on_use_item(&model->player, &model->item[0]);
            break;

        case '\x1B' :
            model->quit = 1 ;
            break;

        default:
            break;
    }
}
# 67 "crashers.c"
static void process_sync_events(Model *model)
{
    int i;

    update_player_cooldowns(&model->player);
    update_player_position(&model->player);

    for (i = 0; i < model->enemy_count; i++)
    {
        if (model->enemy[i].active)
            update_enemy_position(&model->enemy[i], &model->player);
    }

    if (model->boss.active)
        update_boss_position(&model->boss, &model->player);
}
# 89 "crashers.c"
void process_cond_events(Model *model)
{
    int  i;
    bool player_died;


    for (i = 0; i < model->enemy_count; i++)
    {
        if (!model->enemy[i].active) continue;

        player_hits_enemy(&model->player, &model->enemy[i]);

        player_died = enemy_hits_player(&model->enemy[i], &model->player);
        if (player_died)
        {
            player_dies();
            model->quit = 1 ;
            return;
        }
    }


    if (model->boss.active)
    {
        player_hits_boss(&model->player, &model->boss);
        boss_summon(&model->boss, model);

        player_died = boss_hits_player(&model->boss, &model->player);
        if (player_died)
        {
            player_dies();
            model->quit = 1 ;
            return;
        }
    }


    if (!model->boss.active && next_level(model, model->stage))
    {
        drop_item(model, model->stage);
        model->stage++;

        if (model->stage < 4)
            spawn_enemy(model, model->stage);
        else
            model->boss.active = 1 ;
    }

    update_health_HUD(&model->player);
    model->quit = level_end(model);
}

int main(void)
{

    void *base = (void*)_trap_14_w((short)0x2) ;
    Model model;
    UINT32 time_then;
    UINT32 time_now;
    UINT32 time_elapsed;
    char key;
    clear_screen(base);
    init_model(&model);
    model.quit = 0 ;
    render(&model, base);
    time_then = get_time();

    while (!model.quit)
    {
        if (has_input())
        {
            key = get_input();
            process_async_event(&model, key);
        }

        time_now = get_time();
        time_elapsed = time_now - time_then;

        if (time_elapsed > 0)
        {
            process_sync_events(&model);
            process_cond_events(&model);

            render(&model, base);
            time_then = time_now;
        }
    }
    reset_movement(&model);

    return 0;
}
