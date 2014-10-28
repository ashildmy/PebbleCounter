#include <pebble.h>

int shots;

static Window *window;
static TextLayer *text_layer;

void up_single_click_handler(ClickRecognizerRef recognizer, void *context){
  shots++;  
  APP_LOG(0, "%i\n", shots);
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context){
  if(shots > 0){
    shots--;  
  }
  APP_LOG(0, "%i\n", shots);
}

void config_provider(Window *window){
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
}

static void init(void) {
  const bool animate = true;
  shots = 0;
  window = window_create();
  window_stack_push(window, animate);

  window_set_click_config_provider(window, (ClickConfigProvider) config_provider);

  Layer *window_layer = window_get_root_layer(window);	
  text_layer = text_layer_create((GRect(0, 0, 144, 25)));
  text_layer_set_text(text_layer, "Accuracy Counter");
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_background_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
