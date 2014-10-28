#include <pebble.h>

int shots;
int hits;
int percent;

static Window *window;
static TextLayer *count_layer;
static char shot_text[1024];

void up_single_click_handler(ClickRecognizerRef recognizer, void *context){
  hits++;
  shots++;  
  percent = (hits * 100) / shots;
  APP_LOG(0, "%i %i %i\n", hits, shots, percent);
  snprintf(shot_text, (6 * sizeof(char)), "%i %c", percent, '%');
  text_layer_set_text(count_layer, shot_text);
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context){
  /* if(shots > 0){ */
  /*   shots--;   */
  /* } */
  shots++;
  percent = (hits * 100) / shots;
  APP_LOG(0, "%i %i %i\n", hits, shots, percent);
  snprintf(shot_text, (6 * sizeof(char)), "%i %c", percent, '%');
  text_layer_set_text(count_layer, shot_text);
}

void config_provider(Window *window){
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
}

static void create_count_text(Layer* window_layer) {
  count_layer = text_layer_create((GRect(0, 40, 144, 100)));
  text_layer_set_text(count_layer, "0%%");
  text_layer_set_text_color(count_layer, GColorBlack);
  text_layer_set_font(count_layer,  fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(count_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(count_layer));
}

static void init(void) {
  window = window_create();
  window_stack_push(window, true);

  shots = 0;

  window_set_click_config_provider(window, (ClickConfigProvider) config_provider);

  Layer *window_layer = window_get_root_layer(window);	
  GRect bounds = layer_get_frame(window_layer);

  create_count_text(window_layer);
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
