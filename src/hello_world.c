#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

static void window_load(Window *window){

}

static void window_unload(Window *window){

}

static void init(void) {
	window = window_create();
  	window_stack_push(window, true);

	Layer *window_layer = window_get_root_layer(window);	
	GRect bounds = layer_get_frame(window_layer);
	text_layer = text_layer_create((GRect(0, 0, 40, 40)));
	text_layer_set_text(text_layer, "Accuracy Counter");
	text_layer_set_text_color(text_layer, GColorWhite);
	text_layer_set_background_color(text_layer, GColorBlack);
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(text_layer));

//  window_set_window_handlers(window, (WindowHandlers) {
 //   .load = window_load,
  //  .unload = window_unload,
 // });
  //const bool animated = true;
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
