#include <pebble.h>

static Window *window;
static TextLayer *count_layer;

static void window_load(Window *window){

}

static void window_unload(Window *window){

}

static void create_count_text(Layer* window_layer) {
  count_layer = text_layer_create((GRect(0, 40, 144, 100)));
  text_layer_set_text(count_layer, "0");
  text_layer_set_text_color(count_layer, GColorBlack);
  text_layer_set_font(count_layer,  fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(count_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(count_layer));
}

static void init(void) {
  window = window_create();
  window_stack_push(window, true);

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
