#include "pebble.h"

#define NUM_MENU_SECTIONS 1
//#define NUM_MENU_ICONS 5
#define NUM_FIRST_MENU_ITEMS 5
#define ACCEL_STEP_MS 200

static Window *window;
static Window *messageWindow;
static Window *splash;
static AppTimer *timer;

// Key values for AppMessage Dictionary
enum {
  STATUS_KEY = 0,
  MESSAGE_KEY = 1
};

// This is a menu layer
// You have more control than with a simple menu layer
static MenuLayer *menu_layer;
static TextLayer *message_layer;
static TextLayer *splash_layer;

// Menu items can optionally have an icon drawn with them
//static GBitmap *menu_icons[NUM_MENU_ICONS];

//static int current_icon = 0;

// You can draw arbitrary things in a menu item such as a background
//static GBitmap *menu_background;

int oldangle = 0;

char *option0 = "asfdsafsdfasfd";
char *option1 = "afasfsfsfaksjf";
char *option2 = "basvaecweaceas";
char *option3 = "awgbzxacwecgal";
char *option4 = "Adfavvawceckja";
char *message_text = "From: Austin Eng\n This is a test message. I really hope it works! This is a test message. I really hope it works! This is a test message. I really hope it works!";

// A callback is used to specify the amount of sections of menu items
// With this, you can dynamically add and remove sections
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

// Each section has a number of items;  we use a callback to specify this
// You can also dynamically add and remove items using this
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;

    default:
      return 0;
  }
}

// A callback is used to specify the height of the section header
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  // This is a define provided in pebble.h that you may use for the default height
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

// Here we draw what each header is
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "User Options");
      break;

  }
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  //cell_index->section = NUM_MENU_ICONS;
  int len;
  // Determine which section we're going to draw in
  switch (cell_index->section) {
    case 0:
      // Use the row to specify which item we'll draw
      switch (cell_index->row) {
        case 0:
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, option0, NULL, NULL);
          break;

        case 1:
          // This is a basic menu icon with a cycling icon
          menu_cell_basic_draw(ctx, cell_layer, option1, NULL, NULL);
          break;

        case 2:
          menu_cell_basic_draw(ctx, cell_layer, option2, NULL, NULL);
          break;

        case 3:
          menu_cell_basic_draw(ctx, cell_layer, option3, NULL, NULL);
          break;

        case 4:
          menu_cell_basic_draw(ctx, cell_layer, option4, NULL, NULL);
          break;

      }
      break;
  }
}

// Here we capture when a user selects a menu item
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // Use the row to specify which item will receive the select action
  /*switch (cell_index->row) {
    // This is the menu item with the cycling icon
    case 1:
      // Cycle the icon
      current_icon = (current_icon + 1) % NUM_MENU_ICONS;
      // After changing the icon, mark the layer to have it updated
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
  }*/

}
void splash_window_load(Window *splashWindow) {
  Layer *window_layer = window_get_root_layer(splashWindow);
  GRect bounds = layer_get_frame(window_layer);

  splash_layer = text_layer_create(bounds);
  text_layer_set_font(splash_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_background_color(splash_layer, GColorClear);
  text_layer_set_text_color(splash_layer, GColorBlack);
  text_layer_set_text(splash_layer, "Welcome to Swiper!");

  layer_add_child(window_layer, text_layer_get_layer(splash_layer));
}
void splash_window_unload(Window *splashWindow) {
  text_layer_destroy(splash_layer);
}

void message_window_load(Window *messageWindow) {
  Layer *window_layer = window_get_root_layer(messageWindow);
  GRect bounds = layer_get_frame(window_layer);

  message_layer = text_layer_create(bounds);
  text_layer_set_font(message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_background_color(message_layer, GColorClear);
  text_layer_set_text_color(message_layer, GColorBlack);
  text_layer_set_text(message_layer, message_text);

  layer_add_child(window_layer, text_layer_get_layer(message_layer));
}

void message_window_unload(Window *messageWindow) {
  text_layer_destroy(message_layer);
}

// This initializes the menu upon window load
void window_load(Window *window) {
  // Here we load the bitmap assets
  // resource_init_current_app must be called before all asset loading
  //int num_menu_icons = 0;
  //menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_BIG_WATCH);
  //menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_SECTOR_WATCH);
  //menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_BINARY_WATCH);
  //menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_BINARY_WATCH);

  // And also load the background
  //menu_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_BRAINS);

  // Now we prepare to initialize the menu layer
  // We need the bounds to specify the menu layer's viewport size
  // In this case, it'll be the same as the window's
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  menu_layer = menu_layer_create(bounds);

  // Set all the callbacks for the menu layer
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(menu_layer, window);

  // Add it to the window for display
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(menu_layer);

  // Cleanup the menu icons
  //for (int i = 0; i < NUM_MENU_ICONS; i++) {
  //  gbitmap_destroy(menu_icons[i]);
  //}

  // And cleanup the background
  //gbitmap_destroy(menu_background);
}

void changeOption(int num, char* text){
  if (num == 0) {
    strcpy(option0, text);
  } else if (num == 1) {
    strcpy(option1, text);
  } else if (num == 2) {
    strcpy(option2, text);
  } else if (num == 3) {
    strcpy(option3, text);
  } else if (num == 4) {
    strcpy(option4, text);
  }
}

// Write message to buffer & send
void send_message(void){
  APP_LOG(APP_LOG_LEVEL_INFO, "Sending stuff!");
  DictionaryIterator *iter;

  app_message_outbox_begin(&iter);
  dict_write_uint8(iter, STATUS_KEY, 0x1);

  dict_write_end(iter);
    app_message_outbox_send();
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
  /*Tuple *tuple1;
  Tuple *tuple2;

  tuple1 = dict_find(received, STATUS_KEY);
  if(tuple1) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Status Key!");
    APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE , "Received Status: %d", (int)tuple1->value->uint32);
  }

  tuple2 = dict_find(received, MESSAGE_KEY);
  if(tuple2) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Message Key!");
    APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE , "Received Message: %s", tuple2->value->cstring);
  }*/

  /*if(tuple1) {
    if((int)tuple1->value->uint32 == 11) {
      if(tuple2) {
        MenuIndex index;
        index.row = atoi(tuple2->value->cstring);
        index.section = 0;
        menu_layer_set_selected_index(menu_layer, index , MenuRowAlignNone, true);
      }
    }
  }*/

  Tuple *tuple = dict_read_first(received);
  while(tuple) {
    switch (tuple->key) {
      case 0:
        APP_LOG(APP_LOG_LEVEL_INFO, "got a 0");
      case 1:
        changeOption(0, tuple->value->cstring);
        APP_LOG(APP_LOG_LEVEL_INFO, "setting element 0");
        break;
      case 2:
        changeOption(1, tuple->value->cstring);
        APP_LOG(APP_LOG_LEVEL_INFO, "setting element 1");
        break;
      case 3:
        changeOption(2, tuple->value->cstring);
        APP_LOG(APP_LOG_LEVEL_INFO, "setting element 2");
        break;
      case 4:
        changeOption(3, tuple->value->cstring);
        APP_LOG(APP_LOG_LEVEL_INFO, "setting element 3");
        break;
      case 5:
        changeOption(4, tuple->value->cstring);
        APP_LOG(APP_LOG_LEVEL_INFO, "setting element 4");
        break;
      case 6:
        APP_LOG(APP_LOG_LEVEL_INFO, "moving index to %d", (((int)tuple->value->uint16)));
        MenuIndex index;
        index.row = (((int)tuple->value->uint16));
        index.section = 0;
        menu_layer_set_selected_index(menu_layer, index , MenuRowAlignCenter, true);
        break;
      case 7:
        APP_LOG(APP_LOG_LEVEL_INFO, "switching to message page");
        strcpy(message_text, tuple->value->cstring);

        if(window_stack_get_top_window()!=messageWindow){
          if(window_stack_contains_window(messageWindow)){
            window_stack_pop(true);
          } else {
            window_stack_push(messageWindow, true);
          }
        }
        break;
      case 8:
        APP_LOG(APP_LOG_LEVEL_INFO, "switching to list page");
        if(window_stack_get_top_window()!=window){
          if(window_stack_contains_window(window)){
            window_stack_pop(true);
          } else {
            window_stack_push(window, true);
          }
        }
    }
    tuple = dict_read_next(received);
  }
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

unsigned long isqrt(unsigned long x)
{
    register unsigned long op, res, one;

    op = x;
    res = 0;

    /* "one" starts at the highest power of four <= than the argument. */
    one = 1 << 30;  /* second-to-top bit set */
    while (one > op) one >>= 2;

    while (one != 0) {
        if (op >= res + one) {
            op -= res + one;
            res += one << 1;  // <-- faster than 2 * one
        }
        res >>= 1;
        one >>= 2;
    }
    return res;
}

static void timer_callback(void *data) {
  AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };
  int angle;
  double sum_of_squares;
  long square_root;

  accel_service_peek(&accel);

  sum_of_squares = (double)(accel.x * accel.x) + (double)(accel.y * accel.y) + (double)(accel.z * accel.z);
  square_root = isqrt((long)sum_of_squares);
  angle = accel.z * -100 / (int)square_root;

  if(angle > 90 && oldangle<=90) {
    APP_LOG(APP_LOG_LEVEL_INFO, "in the zone");

    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    Tuplet value = TupletInteger(11, 12);
    dict_write_tuplet(iter, &value);
    app_message_outbox_send();
  }
  oldangle=angle;

  timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}

static void init() {
  messageWindow = window_create();
  window = window_create();
  splash = window_create();

  window_set_window_handlers(messageWindow, (WindowHandlers) {
    .load = message_window_load,
    .unload = message_window_unload,
  });

  // Setup the window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_set_window_handlers(splash, (WindowHandlers) {
    .load = splash_window_load,
    .unload = splash_window_unload,
  });

  accel_data_service_subscribe(0, NULL);
  timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}

static void deinit() {
  window_destroy(messageWindow);
  window_destroy(window);
  accel_data_service_unsubscribe();
}

int main(void) {

  init();

  window_stack_push(splash, true);

  // Register AppMessage handlers
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);

  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  send_message();

  app_event_loop();

  deinit();

}