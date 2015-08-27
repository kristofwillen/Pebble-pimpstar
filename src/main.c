#include <pebble.h>

Window *my_window;
TextLayer *h1[10], *h2[10], *m1[10], *m2[10], *background_layer;
static GFont s_hour_font;



static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  static char hbuffer[] = "00";
  static char mbuffer[] = "00";
  
  for (int i=0;i<10;i++) {
    text_layer_set_text_color(h1[i], GColorLightGray);
    text_layer_set_text_color(h2[i], GColorLightGray);
    text_layer_set_text_color(m1[i], GColorLightGray);
    text_layer_set_text_color(m2[i], GColorLightGray);
  }
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) { strftime(hbuffer, sizeof("00"), "%H", tick_time); } 
  else { strftime(hbuffer, sizeof("00"), "%I", tick_time); }
  
  strftime(mbuffer, sizeof("00"), "%M", tick_time);
  
  int hour  = atoi(hbuffer);
  int quotient  = hour/10;
  int remainder = hour%10;
  
  static char h1text[] = "0";
  snprintf(h1text, sizeof(h1text), "%d", quotient);
  text_layer_set_text_color(h1[quotient],GColorCyan);
  text_layer_set_text(h1[quotient], h1text);
  
  static char h2text[] = "0";
  snprintf(h2text, sizeof(h2text), "%d", remainder);
  text_layer_set_text_color(h2[remainder],GColorRed);
  text_layer_set_text(h2[remainder], h2text);
  
  int minutes   = atoi(mbuffer);
  quotient  = minutes/10;
  remainder = minutes%10;
  
  static char m1text[] = "0";
  snprintf(m1text, sizeof(m1text), "%i", quotient);
  text_layer_set_text_color(m1[quotient],GColorOrange);
  text_layer_set_text(m1[quotient], m1text);
  
  static char m2text[] = "0";
  snprintf(m2text, sizeof(m2text), "%d", remainder);
  text_layer_set_text_color(m2[remainder],GColorGreen);
  text_layer_set_text(m2[remainder], m2text);
  

}  


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}


void handle_init(void) {
  my_window = window_create();

  int i=0;
  static char *buffer[] = {"0","1","2","3","4","5","6","7","8","9"};  
  
  background_layer = text_layer_create(GRect(0,0,144,168));
  text_layer_set_background_color(background_layer, GColorBlack);
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(background_layer));
  
  s_hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OCRA_EXTENDED_12));
  for (i=0;i<10;i++) {
    h1[i] = text_layer_create(GRect(24, 17*i, 24, 17));
    text_layer_set_text_color(h1[i], GColorLightGray);
    text_layer_set_background_color(h1[i],GColorClear);
    text_layer_set_text_alignment(h1[i], GTextAlignmentCenter);
    text_layer_set_text(h1[i], buffer[i]);
    text_layer_set_font(h1[i], s_hour_font);
    layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(h1[i]));
    
    h2[i] = text_layer_create(GRect(48, 17*i, 24, 17));
    text_layer_set_text_color(h2[i], GColorLightGray);
    text_layer_set_background_color(h2[i],GColorClear);
    text_layer_set_text_alignment(h2[i], GTextAlignmentCenter);
    text_layer_set_text(h2[i], buffer[i]);
    text_layer_set_font(h2[i], s_hour_font);
    layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(h2[i]));
    
    m1[i] = text_layer_create(GRect(72, 17*i, 24, 17));
    text_layer_set_text_color(m1[i], GColorLightGray);
    text_layer_set_background_color(m1[i],GColorClear);
    text_layer_set_text_alignment(m1[i], GTextAlignmentCenter);
    text_layer_set_text(m1[i], buffer[i]);
    text_layer_set_font(m1[i], s_hour_font);
    layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(m1[i]));
    
    m2[i] = text_layer_create(GRect(96, 17*i, 24, 17));
    text_layer_set_text_color(m2[i], GColorLightGray);
    text_layer_set_background_color(m2[i],GColorClear);
    text_layer_set_text_alignment(m2[i], GTextAlignmentCenter);
    text_layer_set_text(m2[i], buffer[i]);
    text_layer_set_font(m2[i], s_hour_font);
    layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(m2[i]));
  }
  
  // Make sure the time is displayed from the start
  update_time();
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  window_stack_push(my_window, true);
}

void handle_deinit(void) {
  for (int i=0;i<10;i++) {
    text_layer_destroy(m1[i]);
    text_layer_destroy(m2[i]);
    text_layer_destroy(h1[i]);
    text_layer_destroy(h2[i]);
  }
  text_layer_destroy(background_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
