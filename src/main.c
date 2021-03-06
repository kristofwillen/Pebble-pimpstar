#include <pebble.h>

  
Window *my_window;
TextLayer *h1[10], *h2[10], *m1[10], *m2[10], *background_layer, *battery_layer, *am_layer, *date1_layer, *date2_layer;
static GFont s_hour_font, s_battery_font, s_am_font;
AppTimer *timer;
const int delta = 300;
int loopcounter = 0;
int hquotient, mquotient = 0;
int hrest, mrest         = 0;
static int batteryLevel = 100;


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



static void battery_handler(BatteryChargeState new_state) {
  batteryLevel = (int)new_state.charge_percent;
}


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  srand((unsigned) time(&temp));
  static char hbuffer[] = "00";
  static char mbuffer[] = "00";
  static char pbuffer[] = "00";
  static char dbuffer[]=  "00";
  static char d1str[] = "0";
  static char d2str[] = "0";
  
  timer = app_timer_register(delta, (AppTimerCallback) timer_callback, NULL);
  loopcounter = 0;
  
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
  
  // The Martini glass...
  strftime(pbuffer, sizeof("00"),"%H", tick_time);
  int phour = atoi(pbuffer);
  if (phour <= 12) { text_layer_set_text_color(am_layer, GColorDarkGray); }
  else { text_layer_set_text_color(am_layer, GColorSunsetOrange); }
  
  int hour  = atoi(hbuffer);
  hquotient = hour/10;
  hrest     = hour%10;
  int minutes = atoi(mbuffer);
  mquotient   = minutes/10;
  mrest       = minutes%10;
  
  // Display date on DateLayer
  strftime(dbuffer, sizeof(dbuffer), "%d", tick_time);
  int iday = atoi(dbuffer);
  int dquot = iday/10;
  int drem  = iday%10;
  snprintf(d1str, sizeof(d1str), "%d", dquot);
  snprintf(d2str, sizeof(d2str), "%d", drem);
  text_layer_set_text(date1_layer, d1str);
  text_layer_set_text(date2_layer, d2str);
  
  // battery icon color
  // TODO - check less for battery state ! Now 10 minutes !
  if (mquotient == 0) { 
    battery_handler(battery_state_service_peek());
  }
  if (batteryLevel <= 20) { text_layer_set_text_color(battery_layer, GColorSunsetOrange); }
  else  {
    if (batteryLevel <= 40) { text_layer_set_text_color(battery_layer, GColorPastelYellow); }
    else {
       if (batteryLevel <= 60) { text_layer_set_text_color(battery_layer, GColorVividCerulean); }
       else { text_layer_set_text_color(battery_layer, GColorDarkGray); }
    }
  }
}  



static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}


void handle_init(void) {
  my_window = window_create();

  int i=0;
  int ypos = 0;
  static char *buffer[] = {"0","1","2","3","4","5","6","7","8","9"};  
  
  background_layer = text_layer_create(GRect(0,0,144,168));
  text_layer_set_background_color(background_layer, GColorBlack);
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(background_layer));
  
  s_hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_COMPUTER_24));
  //s_hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OCRA_EXTENDED_18));
  for (i=0;i<10;i++) {
    ypos = 16*i-5;
    h1[i] = text_layer_create(GRect(24, ypos, 24, 28));
    text_layer_set_background_color(h1[i],GColorClear);
    text_layer_set_text_alignment(h1[i], GTextAlignmentCenter);
    text_layer_set_text(h1[i], buffer[i]);
    text_layer_set_font(h1[i], s_hour_font);
    layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(h1[i]));
    
    h2[i] = text_layer_create(GRect(48, ypos, 24, 28));
    text_layer_set_background_color(h2[i],GColorClear);
    text_layer_set_text_alignment(h2[i], GTextAlignmentCenter);
    text_layer_set_text(h2[i], buffer[i]);
    text_layer_set_font(h2[i], s_hour_font);
    layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(h2[i]));
    
    m1[i] = text_layer_create(GRect(72, ypos, 24, 28));
    text_layer_set_background_color(m1[i],GColorClear);
    text_layer_set_text_alignment(m1[i], GTextAlignmentCenter);
    text_layer_set_text(m1[i], buffer[i]);
    text_layer_set_font(m1[i], s_hour_font);
    layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(m1[i]));
    
    m2[i] = text_layer_create(GRect(96, ypos, 24, 28));
    text_layer_set_background_color(m2[i],GColorClear);
    text_layer_set_text_alignment(m2[i], GTextAlignmentCenter);
    text_layer_set_text(m2[i], buffer[i]);
    text_layer_set_font(m2[i], s_hour_font);
    layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(m2[i]));
  }
  
  
  battery_layer = text_layer_create(GRect(0,-3,24,34));
  s_battery_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SMARTPHONE_ICONS_24));
  text_layer_set_text_color(battery_layer, GColorDarkGray);
  text_layer_set_font(battery_layer, s_battery_font);
  text_layer_set_text(battery_layer, "r");
  text_layer_set_background_color(battery_layer,GColorClear);
  text_layer_set_text_alignment(battery_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(battery_layer));
  
  
  am_layer = text_layer_create(GRect(120,-3,24,34));
  s_am_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ALCOHOL_24));
  text_layer_set_text_color(am_layer, GColorDarkGray);
  text_layer_set_font(am_layer, s_am_font);
  text_layer_set_text(am_layer, "m");
  text_layer_set_background_color(am_layer,GColorClear);
  text_layer_set_text_alignment(am_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(am_layer));
  
  date1_layer = text_layer_create(GRect(120,59,24,34));
  text_layer_set_text_color(date1_layer, GColorDarkGray);
  text_layer_set_font(date1_layer, s_hour_font);
  text_layer_set_text(date1_layer, "0");
  text_layer_set_background_color(date1_layer,GColorClear);
  text_layer_set_text_alignment(date1_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(date1_layer));
  
  date2_layer = text_layer_create(GRect(120,75,24,34));
  text_layer_set_text_color(date2_layer, GColorDarkGray);
  text_layer_set_font(date2_layer, s_hour_font);
  text_layer_set_text(date2_layer, "0");
  text_layer_set_background_color(date2_layer,GColorClear);
  text_layer_set_text_alignment(date2_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(date2_layer));
  
  // Make sure the time is displayed from the start
  update_time();
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Get the current battery level
  battery_handler(battery_state_service_peek());
  
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
  text_layer_destroy(date1_layer);
  text_layer_destroy(date2_layer);
  text_layer_destroy(battery_layer);
  text_layer_destroy(am_layer);
  app_timer_cancel(timer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
