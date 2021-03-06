#include <pebble.h>
#include "util.h"

int shots;
int hits;
int misses;
int tempHit = 0;
int pad = 5;
int maxStreak = 0;
float percent;

static Window *window;
static Window *startWindow;
static TextLayer *count_layer;
static TextLayer *hit_layer;
static TextLayer *miss_layer;
static TextLayer *scoreboard_layer;
static TextLayer *fire_status_layer;
static char shot_text[1024];
static char scoreboard_text[1024];
static char percent_text[1024];


void up_single_click_handler(ClickRecognizerRef recognizer, void *context){
  hits++;
  shots++;  
  tempHit++;
  percent = (float)hits / shots;
  floatToString(percent_text, sizeof(percent_text), percent);
  strcat(percent_text, "%");
  snprintf(scoreboard_text, (40 * sizeof(char)), "Hits: %i\nMisses: %i\nShots: %i", hits, misses, shots);

  if(tempHit == 2){
    text_layer_set_text(fire_status_layer, "Heating Up!");
    vibes_double_pulse();
  }else if(tempHit == 3){
    text_layer_set_text(fire_status_layer, "Fire!");
    vibes_long_pulse();
  }
  
  if(maxStreak < hits){
    maxStreak = hits;
  }
  
  text_layer_set_text(count_layer, percent_text);
  text_layer_set_text(scoreboard_layer, scoreboard_text);
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context){
  misses++;
  shots++;
  tempHit = 0;
  text_layer_set_text(fire_status_layer, "");
  percent = (float)hits / shots;
  floatToString(percent_text, sizeof(percent_text), percent);
  strcat(percent_text, "%");
  snprintf(scoreboard_text, (40 * sizeof(char)), "Hits: %i\nMisses: %i\nShots: %i", hits, misses, shots);
  text_layer_set_text(count_layer, percent_text);
  text_layer_set_text(scoreboard_layer, scoreboard_text);
}

void select_long_click_handler(ClickRecognizerRef recognizer, void *context){
  misses = 0;
  hits = 0;
  shots = 0;
  tempHit = 0;
  maxStreak = 0;

  text_layer_set_text(scoreboard_layer, "Hits: 0\nMisses: 0\nShots: 0");
  text_layer_set_text(count_layer, "0.00%");
  text_layer_set_text(fire_status_layer, "");
}

void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context){
  
}

void config_provider(Window *window){
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);

  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
}

static void create_count_text(Layer* window_layer) {
  GRect bounds = layer_get_bounds(window_layer);
  count_layer = text_layer_create(GRect(pad, 60, bounds.size.w - pad, bounds.size.h));
  text_layer_set_text(count_layer, "0.00%");
  text_layer_set_text_color(count_layer, GColorBlack);
  GFont customFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_HELVETICA_NEUE_BOLD_34));
  text_layer_set_font(count_layer,  customFont);
  text_layer_set_text_alignment(count_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(count_layer));
}

static void create_hit_text(Layer* window_layer){
  GRect bounds = layer_get_bounds(window_layer);
  hit_layer = text_layer_create(GRect(pad, 3, bounds.size.w - pad, bounds.size.h));
  text_layer_set_text(hit_layer, "Hit ");
  text_layer_set_text_color(hit_layer, GColorBlack);
  text_layer_set_background_color(hit_layer, GColorClear);
  text_layer_set_font(hit_layer,  fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(hit_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(hit_layer));
}

static void create_miss_text(Layer* window_layer){
  GRect bounds = layer_get_bounds(window_layer);
  miss_layer = text_layer_create(GRect(pad, 120-3, bounds.size.w - pad, bounds.size.h));
  text_layer_set_text(miss_layer, "Miss ");
  text_layer_set_text_color(miss_layer, GColorBlack);
  text_layer_set_background_color(miss_layer, GColorClear);
  text_layer_set_font(miss_layer,  fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(miss_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(miss_layer));
}

static void create_scoreboard_text(Layer* window_layer){
  GRect bounds = layer_get_bounds(window_layer);
  scoreboard_layer = text_layer_create(GRect(pad, 2, bounds.size.w, bounds.size.h));
  text_layer_set_text(scoreboard_layer, "Hits: 0\nMisses: 0\nShots: 0");
  text_layer_set_text_color(scoreboard_layer, GColorBlack);
  text_layer_set_background_color(scoreboard_layer, GColorClear);
  text_layer_set_text_alignment(scoreboard_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(scoreboard_layer));
}

static void create_fire_text(Layer* window_layer){
  GRect bounds = layer_get_bounds(window_layer);
  fire_status_layer = text_layer_create(GRect(pad, 120-3, bounds.size.w, bounds.size.h));
  text_layer_set_text_color(fire_status_layer, GColorBlack);
  text_layer_set_background_color(fire_status_layer, GColorClear);
  text_layer_set_font(fire_status_layer,  fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(fire_status_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(fire_status_layer));
}

static void init(void) {
  window = window_create();
  window_stack_push(window, true);
  /* startWindow = window_create(); */
  /* window_stack_push(startWindow, true); */

  shots = 0;
  hits = 0;
  misses = 0;
  maxStreak = 0;

  window_set_click_config_provider(window, (ClickConfigProvider) config_provider);

  Layer *window_layer = window_get_root_layer(window);	

  create_count_text(window_layer);
  create_hit_text(window_layer);
  create_miss_text(window_layer);
  create_scoreboard_text(window_layer);
  create_fire_text(window_layer);
}

static void deinit(void) {
  text_layer_destroy(count_layer);
  text_layer_destroy(hit_layer);
  text_layer_destroy(scoreboard_layer);
  text_layer_destroy(miss_layer);
  text_layer_destroy(fire_status_layer);
  window_destroy(window);
  /* window_destroy(startWindow); */
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
