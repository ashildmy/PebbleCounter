#include <pebble.h>

int shots;
int hits;
int percent;
int misses;

static Window *window;
static TextLayer *count_layer;
static TextLayer *hit_layer;
static TextLayer *miss_layer;
static TextLayer *scoreboard_layer;
static char shot_text[1024];
static char scoreboard_text[1024];

void up_single_click_handler(ClickRecognizerRef recognizer, void *context){
  hits++;
  shots++;  
  percent = (hits * 100) / shots;
  APP_LOG(0, "%i %i %i\n", hits, shots, percent);
  snprintf(shot_text, (6 * sizeof(char)), "%i%c", percent, '%');
  snprintf(scoreboard_text, (40 * sizeof(char)), "Hits: %i\nMisses: %i\nShots: %i", hits, misses, shots);
  text_layer_set_text(count_layer, shot_text);
  text_layer_set_text(scoreboard_layer, scoreboard_text);
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context){
  misses++;
  shots++;
  percent = (hits * 100) / shots;
  APP_LOG(0, "%i %i %i\n", hits, shots, percent);
  snprintf(shot_text, (6 * sizeof(char)), "%i%c", percent, '%');
  snprintf(scoreboard_text, (40 * sizeof(char)), "Hits: %i\nMisses: %i\nShots: %i", hits, misses, shots);
  text_layer_set_text(count_layer, shot_text);
  text_layer_set_text(scoreboard_layer, scoreboard_text);
}

void select_long_click_handler(ClickRecognizerRef recognizer, void *context){
  misses = 0;
  hits = 0;
  shots = 0;

  text_layer_set_text(scoreboard_layer, "Hits: 0\nMisses: 0\nShots: 0");
  text_layer_set_text(count_layer, "0%");
}

void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context){
  
}

void config_provider(Window *window){
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);

  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
}

static void create_count_text(Layer* window_layer) {
  count_layer = text_layer_create(GRect(0, 55, 144, 168));
  text_layer_set_text(count_layer, "0%");
  text_layer_set_text_color(count_layer, GColorBlack);
  text_layer_set_font(count_layer,  fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(count_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(count_layer));
}

static void create_hit_text(Layer* window_layer){
  hit_layer = text_layer_create(GRect(0, 5, 144, 168));
  text_layer_set_text(hit_layer, "Hit->");
  text_layer_set_text_color(hit_layer, GColorBlack);
  text_layer_set_background_color(hit_layer, GColorClear);
  text_layer_set_font(hit_layer,  fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(hit_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(hit_layer));
}

static void create_miss_text(Layer* window_layer){
  miss_layer = text_layer_create(GRect(0, 120, 144, 168));
  text_layer_set_text(miss_layer, "Miss->");
  text_layer_set_text_color(miss_layer, GColorBlack);
  text_layer_set_background_color(miss_layer, GColorClear);
  text_layer_set_font(miss_layer,  fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(miss_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(miss_layer));
}

static void create_scoreboard_text(Layer* window_layer){
  scoreboard_layer = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_text(scoreboard_layer, "Hits: 0\nMisses: 0\nShots: 0");
  text_layer_set_text_color(scoreboard_layer, GColorBlack);
  text_layer_set_background_color(scoreboard_layer, GColorClear);
  text_layer_set_text_alignment(scoreboard_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(scoreboard_layer));
}

static void init(void) {
  window = window_create();
  window_stack_push(window, true);

  shots = 0;
  hits = 0;
  misses = 0;

  window_set_click_config_provider(window, (ClickConfigProvider) config_provider);

  Layer *window_layer = window_get_root_layer(window);	

  create_count_text(window_layer);
  create_hit_text(window_layer);
  create_miss_text(window_layer);
  create_scoreboard_text(window_layer);
}

static void deinit(void) {
  text_layer_destroy(count_layer);
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
