#ifndef UTILS_H_
#define UTILS_H_

#define PathBase    "A:/400Plus/"
#define PathLogs    PathBase "logs/"
#define PathLang    PathBase "lang/"
#define PathPresets PathBase "presets/"

// this is the way canon checks the pointers
// if (PTR_VALID(some_ptr)) { ... } else { ... }
// this will check if the pointer is even and not 0
// returns the pointer if it's valid, otherwise returns 0;
#define PTR_VALID(x)  ((x&1)-1 & x)

#define MEM(mem) (*(int*)((int)(mem)))

// ((((int)(x)) & 0xF0000000) == 0xC0000000) ? shamem_read(x) :

#define MEMX(x) ( \
	((((int)(x)) & 0xF0000000) == 0xE0000000) ? (int)0xDEADBEAF : \
	((((int)(x)) & 0xF0000000) == 0x70000000) ? (int)0xDEADBEAF : \
	((((int)(x)) & 0xF0000000) == 0x80000000) ? (int)0xDEADBEAF : \
	*(int*)(x) \
)

// it's slow
#undef FGETS_USE_SLOW

#define TIME_RESOLUTION 1000

#define BEEP_LED_LENGTH  25
#define INTERCOM_WAIT     1
#define EVENT_WAIT        5
#define RELEASE_WAIT    250

#define SHUTTER_LAG_1ST 250
#define SHUTTER_LAG_2ND 100

#define MIRROR_LAG_1ST 2000
#define MIRROR_LAG_2ND 2100

extern void calculate_dof(int focal_length, int focus_distance, int av, char *min, char *max);

extern void beep(void);

extern void enter_factory_mode(void);
extern void exit_factory_mode (void);
extern void start_debug_mode  (void);

extern void dump_log            (void);
extern void dump_memory         (void);
extern void dump_memory_after_5s(void);

extern void print_info(void);

extern int send_to_intercom(int message, int parm);

extern void wait_for_camera(void);

extern int  shutter_release      (void);
extern int  shutter_release_bulb(int time);

extern int  print_icu_info(void);
extern int  press_button  (int button);

extern int remote_on (void);
extern int remote_off(void);
void remote_delay    (int x);

extern int display_on (void);
extern int display_off(void);

extern void led_flash(int delay);

extern int strlen_utf8(const char *);

// convert string to upper case in-place
extern void stoupper(char *s);

extern int timestamp(void);

char* strncpy0(char* dest, const char* src, size_t size);

#ifdef FGETS_USE_SLOW
// this version will read byte-by-byte ... it is slow
char * hack_fgets_simple_but_slow(char *s, int n, int fd);
#define hack_fgets_init() do { } while (0)
#define hack_fgets hack_fgets_simple_but_slow
#else
// WARNING: please read the comments in utils.c about this routine.
// though it is faster than reading byte-by-byte, it has a special init too.
char * hack_fgets_faster(char *s, int n, int fd);
#define hack_fgets_init() hack_fgets_faster(NULL, 0, -1)
#define hack_fgets hack_fgets_faster
#endif

#endif /* UTILS_H_ */
