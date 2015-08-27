#include <pebble.h>

  
Window *my_window;
TextLayer *h1[10], *h2[10], *m1[10], *m2[10], *background_layer, *date_layer;
static GFont s_hour_font;
AppTimer *timer;
const int delta = 300;
int loopcounter = 0;
int hquotient, mquotient = 0;
int hrest, mrest         = 0;


void timer_callback(void *data) {
 
  int randi = rand()%10;
  GColor GColList[10];
  GColList[0] = GColorVividCerulean;  // BLUE
  GColList[1] = GColorYellow;         // YELLOW
  GColList[2] = GColorGreen;          // GREEN
  GColList[3] = GColorRed;            // RED
  GColList[4] = GColorMagenta;        // VIOLET
  GColList[5] = GColorChromeYellow;   // ORANGE
  GColList[6] = GColorCyan;           // BLUE
  GColList[7] = GColorGreen;          // GREEN
  GColList[8] = GColorFashionMagenta; // VIOLET  
  GColList[9] = GColorFolly;          // RED
  
  text_layer_set_text_color(h1[randi],GColorBlue);
  randi = rand()%10;
  text_layer_set_text_color(h2[randi],GColorBlue);
  randi = rand()%10;
  text_layer_set_text_color(m1[randi],GColorBlue);
  randi = rand()%10;
  text_layer_set_text_color(m2[randi],GColorBlue);
  
  //Register next execution
  if (loopcounter < 9) {
    timer = app_timer_register(delta, (AppTimerCallback) timer_callback, NULL);
  }
  else {
    for (int i=0;i<10;i++) {
      text_layer_set_text_color(h1[i], GColorDarkGray);
      text_layer_set_text_color(h2[i], GColorDarkGray);
      text_layer_set_text_color(m1[i], GColorDarkGray);
      text_layer_set_text_color(m2[i], GColorDarkGray);
      
      text_layer_set_text_color(h1[hquotient],GColList[hquotient]);
      text_layer_set_text_color(h2[hrest],GColList[hrest]);
      text_layer_set_text_color(m1[mquotient],GColList[mquotient]);
      text_layer_set_text_color(m2[mrest],GColList[mrest]);
    }   
  } 
  loopcounter = loopcounter + 1;
}


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  srand((unsigned) time(&temp));
  static char hbuffer[] = "00";
  static char mbuffer[] = "00";
  static char s_date_buffer[10];
  static char s_vert_date[20];
  
  timer = app_timer_register(delta, (AppTimerCallback) timer_callback, NULL);
  loopcounter = 0;
  
  for (int i=0;i<10;i++) {
    text_layer_set_text_color(h1[i], GColorDarkGray);
    text_layer_set_text_color(h2[i], GColorDarkGray);
    text_layer_set_text_color(m1[i], GColorDarkGray);
    text_layer_set_text_color(m2[i], GColorDarkGray);
  }
  
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) { strftime(hbuffer, sizeof("00"), "%H", tick_time); } 
  else { strftime(hbuffer, sizeof("00"), "%I", tick_time); }
  strftime(mbuffer, sizeof("00"), "%M", tick_time);
  
  int hour  = atoi(hbuffer);
  hquotient = hour/10;
  hrest     = hour%10;
  //text_layer_set_text_color(h1[hquotient],GColList[hquotient]);
  //text_layer_set_text_color(h2[hrest],GColList[hrest]);
  
  int minutes = atoi(mbuffer);
  mquotient   = minutes/10;
  mrest       = minutes%10;
  //text_layer_set_text_color(m1[mquotient],GColList[mquotient]);
  //text_layer_set_text_color(m2[mrest],GColList[mrest]);
  
  // Display date on DateLayer
  strftime(s_date_buffer, sizeof(s_date_buffer), "%a %d", tick_time);
  text_layer_set_text(date_layer, s_date_buffer);
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
  
  s_hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COMPUTER_24));
  //s_hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OCRA_EXTENDED_18));
  for (i=0;i<10;i++) {
    h1[i] = text_layer_create(GRect(24, 16*i, 24, 28));
    text_layer_set_text_color(h1[i], GColorDarkGray);
    text_layer_set_background_color(h1[i],GColorClear);
    text_layer_set_text_alignment(h1[i], GTextAlignmentCenter);
    text_layer_set_text(h1[i], buffer[i]);
    text_layer_set_font(h1[i], s_hour_font);
    layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(h1[i]));
    
    h2[i] = text_layer_create(GRect(48, 16*i, 24, 28));
    text_layer_set_text_color(h2[i], GColorDarkGray);
    text_layer_set_background_color(h2[i],GColorClear);
    text_layer_set_text_alignment(h2[i], GTextAlignmentCenter);
    text_layer_set_text(h2[i], buffer[i]);
    text_layer_set_font(h2[i], s_hour_font);
    layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(h2[i]));
    
    m1[i] = text_layer_create(GRect(72, 16*i, 24, 28));
    text_layer_set_text_color(m1[i], GColorDarkGray);
    text_layer_set_background_color(m1[i],GColorClear);
    text_layer_set_text_alignment(m1[i], GTextAlignmentCenter);
    text_layer_set_text(m1[i], buffer[i]);
    text_layer_set_font(m1[i], s_hour_font);
    layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(m1[i]));
    
    m2[i] = text_layer_create(GRect(96, 16*i, 24, 28));
    text_layer_set_text_color(m2[i], GColorDarkGray);
    text_layer_set_background_color(m2[i],GColorClear);
    text_layer_set_text_alignment(m2[i], GTextAlignmentCenter);
    text_layer_set_text(m2[i], buffer[i]);
    text_layer_set_font(m2[i], s_hour_font);
    layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(m2[i]));
  }
  
  date_layer = text_layer_create(GRect(120,0,24,168));
  text_layer_set_text_color(date_layer, GColorDarkGray);
  text_layer_set_font(date_layer, s_hour_font);
  text_layer_set_background_color(date_layer,GColorClear);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  //layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(date_layer));
  
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
  text_layer_destroy(date_layer);
  app_timer_cancel(timer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
