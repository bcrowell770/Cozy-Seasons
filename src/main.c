#include <pebble.h>
#include <stdlib.h>
#include <string.h>

// Persistent storage key
#define SETTINGS_KEY 2
#define PARTICLE_COUNT 20
#define PARTICLE_FRAME_MS 60

typedef enum {
  SEASON_SPRING = 0,
  SEASON_SUMMER = 1,
  SEASON_AUTUMN = 2,
  SEASON_WINTER = 3
} Season;

typedef enum {
  SEASON_MODE_AUTO = 0,
  SEASON_MODE_MANUAL = 1,
  SEASON_MODE_RANDOM = 2
} SeasonMode;

typedef enum {
  EFFECT_NONE = 0,
  EFFECT_PETALS,
  EFFECT_SEEDS,
  EFFECT_FIREFLIES,
  EFFECT_LEAVES,
  EFFECT_SNOW
} EffectType;

typedef struct ClaySettings {
  GColor TextColor;
  bool AnimateLeaves;
  bool ShowDate;
  bool ShowBtIcon;
  bool BtVibrate;
  uint8_t SeasonMode;
  Season ManualSeason;
  bool LowVisionMode;
} ClaySettings;

typedef struct {
  int x;
  int y;
  int dx;
  int dy;
  int size;
  int step;
  int phase;
  bool active;
} Particle;

static ClaySettings settings;

static Window *s_main_window;
static Layer *s_window_layer;

static TextLayer *s_time_layer;
static TextLayer *s_ampm_layer;
static TextLayer *s_date_layer;
static Layer *s_panel_layer;
static Layer *s_date_pill_layer;

static GFont s_date_font;
static GFont s_ampm_font;
static GFont s_low_vision_time_font;
static GFont s_low_vision_detail_font;

// Bluetooth
static BitmapLayer *s_bt_icon_layer;
static GBitmap *s_bt_icon_bitmap;

// Battery
static Layer *s_battery_layer;
static int s_battery_level = 100;

// Background
static BitmapLayer *s_bg_layer;
static GBitmap *s_bg_bitmap;
static int s_current_hour = -1;
static Season s_current_season = SEASON_AUTUMN;

// Generated seasonal particles
static Layer *s_effect_layer;
static Particle s_particles[PARTICLE_COUNT];
static AppTimer *s_particle_timer;
static bool s_particle_animating = false;
static EffectType s_current_effect = EFFECT_NONE;

static const uint32_t SPRING_IMAGE_IDS[24] = {
  RESOURCE_ID_IMAGE_SPRING_00,
  RESOURCE_ID_IMAGE_SPRING_01,
  RESOURCE_ID_IMAGE_SPRING_02,
  RESOURCE_ID_IMAGE_SPRING_03,
  RESOURCE_ID_IMAGE_SPRING_04,
  RESOURCE_ID_IMAGE_SPRING_05,
  RESOURCE_ID_IMAGE_SPRING_06,
  RESOURCE_ID_IMAGE_SPRING_07,
  RESOURCE_ID_IMAGE_SPRING_08,
  RESOURCE_ID_IMAGE_SPRING_09,
  RESOURCE_ID_IMAGE_SPRING_10,
  RESOURCE_ID_IMAGE_SPRING_11,
  RESOURCE_ID_IMAGE_SPRING_12,
  RESOURCE_ID_IMAGE_SPRING_13,
  RESOURCE_ID_IMAGE_SPRING_14,
  RESOURCE_ID_IMAGE_SPRING_15,
  RESOURCE_ID_IMAGE_SPRING_16,
  RESOURCE_ID_IMAGE_SPRING_17,
  RESOURCE_ID_IMAGE_SPRING_18,
  RESOURCE_ID_IMAGE_SPRING_19,
  RESOURCE_ID_IMAGE_SPRING_20,
  RESOURCE_ID_IMAGE_SPRING_21,
  RESOURCE_ID_IMAGE_SPRING_22,
  RESOURCE_ID_IMAGE_SPRING_23
};

static const uint32_t SUMMER_IMAGE_IDS[24] = {
  RESOURCE_ID_IMAGE_SUMMER_00,
  RESOURCE_ID_IMAGE_SUMMER_01,
  RESOURCE_ID_IMAGE_SUMMER_02,
  RESOURCE_ID_IMAGE_SUMMER_03,
  RESOURCE_ID_IMAGE_SUMMER_04,
  RESOURCE_ID_IMAGE_SUMMER_05,
  RESOURCE_ID_IMAGE_SUMMER_06,
  RESOURCE_ID_IMAGE_SUMMER_07,
  RESOURCE_ID_IMAGE_SUMMER_08,
  RESOURCE_ID_IMAGE_SUMMER_09,
  RESOURCE_ID_IMAGE_SUMMER_10,
  RESOURCE_ID_IMAGE_SUMMER_11,
  RESOURCE_ID_IMAGE_SUMMER_12,
  RESOURCE_ID_IMAGE_SUMMER_13,
  RESOURCE_ID_IMAGE_SUMMER_14,
  RESOURCE_ID_IMAGE_SUMMER_15,
  RESOURCE_ID_IMAGE_SUMMER_16,
  RESOURCE_ID_IMAGE_SUMMER_17,
  RESOURCE_ID_IMAGE_SUMMER_18,
  RESOURCE_ID_IMAGE_SUMMER_19,
  RESOURCE_ID_IMAGE_SUMMER_20,
  RESOURCE_ID_IMAGE_SUMMER_21,
  RESOURCE_ID_IMAGE_SUMMER_22,
  RESOURCE_ID_IMAGE_SUMMER_23
};

static const uint32_t AUTUMN_IMAGE_IDS[24] = {
  RESOURCE_ID_IMAGE_AUTUMN_00,
  RESOURCE_ID_IMAGE_AUTUMN_01,
  RESOURCE_ID_IMAGE_AUTUMN_02,
  RESOURCE_ID_IMAGE_AUTUMN_03,
  RESOURCE_ID_IMAGE_AUTUMN_04,
  RESOURCE_ID_IMAGE_AUTUMN_05,
  RESOURCE_ID_IMAGE_AUTUMN_06,
  RESOURCE_ID_IMAGE_AUTUMN_07,
  RESOURCE_ID_IMAGE_AUTUMN_08,
  RESOURCE_ID_IMAGE_AUTUMN_09,
  RESOURCE_ID_IMAGE_AUTUMN_10,
  RESOURCE_ID_IMAGE_AUTUMN_11,
  RESOURCE_ID_IMAGE_AUTUMN_12,
  RESOURCE_ID_IMAGE_AUTUMN_13,
  RESOURCE_ID_IMAGE_AUTUMN_14,
  RESOURCE_ID_IMAGE_AUTUMN_15,
  RESOURCE_ID_IMAGE_AUTUMN_16,
  RESOURCE_ID_IMAGE_AUTUMN_17,
  RESOURCE_ID_IMAGE_AUTUMN_18,
  RESOURCE_ID_IMAGE_AUTUMN_19,
  RESOURCE_ID_IMAGE_AUTUMN_20,
  RESOURCE_ID_IMAGE_AUTUMN_21,
  RESOURCE_ID_IMAGE_AUTUMN_22,
  RESOURCE_ID_IMAGE_AUTUMN_23
};

static const uint32_t WINTER_IMAGE_IDS[24] = {
  RESOURCE_ID_IMAGE_WINTER_00,
  RESOURCE_ID_IMAGE_WINTER_01,
  RESOURCE_ID_IMAGE_WINTER_02,
  RESOURCE_ID_IMAGE_WINTER_03,
  RESOURCE_ID_IMAGE_WINTER_04,
  RESOURCE_ID_IMAGE_WINTER_05,
  RESOURCE_ID_IMAGE_WINTER_06,
  RESOURCE_ID_IMAGE_WINTER_07,
  RESOURCE_ID_IMAGE_WINTER_08,
  RESOURCE_ID_IMAGE_WINTER_09,
  RESOURCE_ID_IMAGE_WINTER_10,
  RESOURCE_ID_IMAGE_WINTER_11,
  RESOURCE_ID_IMAGE_WINTER_12,
  RESOURCE_ID_IMAGE_WINTER_13,
  RESOURCE_ID_IMAGE_WINTER_14,
  RESOURCE_ID_IMAGE_WINTER_15,
  RESOURCE_ID_IMAGE_WINTER_16,
  RESOURCE_ID_IMAGE_WINTER_17,
  RESOURCE_ID_IMAGE_WINTER_18,
  RESOURCE_ID_IMAGE_WINTER_19,
  RESOURCE_ID_IMAGE_WINTER_20,
  RESOURCE_ID_IMAGE_WINTER_21,
  RESOURCE_ID_IMAGE_WINTER_22,
  RESOURCE_ID_IMAGE_WINTER_23
};

static Season get_current_season(struct tm *tick_time);
static Season season_from_string(const char *value);
static uint8_t season_mode_from_string(const char *value);
static Season get_random_season_for_day(struct tm *tick_time);
static Season get_effective_season(struct tm *tick_time);
static uint32_t get_background_resource_for_hour(Season season, int hour);
static void update_time(void);
static void update_background(Season season, int hour);
static void refresh_bt_icon(void);
static void apply_layout(void);
static GColor get_panel_color(void);
static void stop_particle_animation(void);
static void start_particle_animation(void);
static void particle_timer_callback(void *context);
static void effect_layer_update_proc(Layer *layer, GContext *ctx);
static void panel_layer_update_proc(Layer *layer, GContext *ctx);
static EffectType get_effect_for_season_and_hour(Season season, int hour);
static bool is_summer_night_hour(int hour);
static GColor get_particle_color(EffectType effect, int index);

static void prv_default_settings(void) {
  settings.TextColor = GColorWhite;
  settings.ShowDate = true;
  settings.AnimateLeaves = true;
  settings.LowVisionMode = false;
  settings.ShowBtIcon = true;
  settings.BtVibrate = true;
  settings.SeasonMode = SEASON_MODE_AUTO;
  settings.ManualSeason = SEASON_AUTUMN;
}

static void prv_save_settings(void) {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void prv_load_settings(void) {
  prv_default_settings();

  if (persist_exists(SETTINGS_KEY)) {
    ClaySettings persisted_settings;
    memset(&persisted_settings, 0, sizeof(persisted_settings));

    int stored_size = persist_get_size(SETTINGS_KEY);
    if (stored_size > 0) {
      int bytes_to_read = stored_size < (int)sizeof(persisted_settings) ? stored_size : (int)sizeof(persisted_settings);
      persist_read_data(SETTINGS_KEY, &persisted_settings, bytes_to_read);

      settings.TextColor = persisted_settings.TextColor;
      settings.AnimateLeaves = persisted_settings.AnimateLeaves;
      settings.ShowDate = persisted_settings.ShowDate;
      settings.ShowBtIcon = persisted_settings.ShowBtIcon;
      settings.BtVibrate = persisted_settings.BtVibrate;
      settings.SeasonMode = persisted_settings.SeasonMode;
      settings.ManualSeason = persisted_settings.ManualSeason;

      if (stored_size >= (int)sizeof(ClaySettings)) {
        settings.LowVisionMode = persisted_settings.LowVisionMode;
      }
    }
  }
}

static void prv_update_display(void) {
  text_layer_set_text_color(s_time_layer, settings.TextColor);
  text_layer_set_text_color(s_ampm_layer, settings.TextColor);
  text_layer_set_text_color(s_date_layer, settings.TextColor);

  apply_layout();

  if (s_panel_layer) {
    layer_mark_dirty(s_panel_layer);
  }

  if (s_date_pill_layer) {
    layer_mark_dirty(s_date_pill_layer);
  }

  if (s_battery_layer) {
    layer_mark_dirty(s_battery_layer);
  }

  if (s_effect_layer) {
    layer_mark_dirty(s_effect_layer);
  }
}

static void battery_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  int fill_width = (bounds.size.w * s_battery_level) / 100;

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, bounds, 1, GCornersAll);

  graphics_context_set_fill_color(ctx, settings.TextColor);
  graphics_fill_rect(ctx, GRect(0, 0, fill_width, bounds.size.h), 0, GCornerNone);
}

static GColor get_panel_color(void) {
  switch (settings.TextColor.argb) {
    case GColorBlackARGB8:
    case GColorDarkGrayARGB8:
    case GColorOxfordBlueARGB8:
    case GColorBulgarianRoseARGB8:
    case GColorDarkCandyAppleRedARGB8:
    case GColorDarkGreenARGB8:
    case GColorMidnightGreenARGB8:
      return GColorLightGray;
    default:
      return GColorBlack;
  }
}

static void panel_layer_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, get_panel_color());
  graphics_fill_rect(ctx, layer_get_bounds(layer), settings.LowVisionMode ? 8 : 6, GCornersAll);
}

static void battery_callback(BatteryChargeState state) {
  s_battery_level = state.charge_percent;
  if (s_battery_layer) {
    layer_mark_dirty(s_battery_layer);
  }
}

static Season get_current_season(struct tm *tick_time) {
  int month = tick_time->tm_mon + 1;

  if (month == 12 || month == 1 || month == 2) {
    return SEASON_WINTER;
  } else if (month >= 3 && month <= 5) {
    return SEASON_SPRING;
  } else if (month >= 6 && month <= 8) {
    return SEASON_SUMMER;
  } else {
    return SEASON_AUTUMN;
  }
}

static Season season_from_string(const char *value) {
  if (!value) {
    return SEASON_AUTUMN;
  }

  if (strcmp(value, "spring") == 0) {
    return SEASON_SPRING;
  } else if (strcmp(value, "summer") == 0) {
    return SEASON_SUMMER;
  } else if (strcmp(value, "winter") == 0) {
    return SEASON_WINTER;
  } else {
    return SEASON_AUTUMN;
  }
}

static uint8_t season_mode_from_string(const char *value) {
  if (!value) {
    return SEASON_MODE_AUTO;
  }

  if (strcmp(value, "manual") == 0) {
    return SEASON_MODE_MANUAL;
  } else if (strcmp(value, "random") == 0) {
    return SEASON_MODE_RANDOM;
  } else {
    return SEASON_MODE_AUTO;
  }
}

static Season get_random_season_for_day(struct tm *tick_time) {
  int day_seed = (tick_time->tm_year + 1900) * 1000 + tick_time->tm_yday;
  return (Season)(day_seed % 4);
}

static Season get_effective_season(struct tm *tick_time) {
  switch (settings.SeasonMode) {
    case SEASON_MODE_MANUAL:
      return settings.ManualSeason;
    case SEASON_MODE_RANDOM:
      return get_random_season_for_day(tick_time);
    case SEASON_MODE_AUTO:
    default:
      return get_current_season(tick_time);
  }
}

static uint32_t get_background_resource_for_hour(Season season, int hour) {
  if (hour < 0 || hour > 23) {
    return RESOURCE_ID_IMAGE_AUTUMN_00;
  }

  switch (season) {
    case SEASON_SPRING:
      return SPRING_IMAGE_IDS[hour];
    case SEASON_SUMMER:
      return SUMMER_IMAGE_IDS[hour];
    case SEASON_WINTER:
      return WINTER_IMAGE_IDS[hour];
    case SEASON_AUTUMN:
    default:
      return AUTUMN_IMAGE_IDS[hour];
  }
}

static void update_background(Season season, int hour) {
  if (hour < 0 || hour > 23) {
    return;
  }

  if (hour == s_current_hour && season == s_current_season) {
    return;
  }

  s_current_hour = hour;
  s_current_season = season;

  if (s_bg_bitmap) {
    gbitmap_destroy(s_bg_bitmap);
    s_bg_bitmap = NULL;
  }

  s_bg_bitmap = gbitmap_create_with_resource(get_background_resource_for_hour(season, hour));
  bitmap_layer_set_bitmap(s_bg_layer, s_bg_bitmap);
}

static bool is_summer_night_hour(int hour) {
  return (hour >= 20 || hour < 6);
}

static EffectType get_effect_for_season_and_hour(Season season, int hour) {
  switch (season) {
    case SEASON_SPRING:
      return EFFECT_PETALS;
    case SEASON_SUMMER:
      return is_summer_night_hour(hour) ? EFFECT_FIREFLIES : EFFECT_SEEDS;
    case SEASON_AUTUMN:
      return EFFECT_LEAVES;
    case SEASON_WINTER:
      return EFFECT_SNOW;
    default:
      return EFFECT_NONE;
  }
}

static GColor get_particle_color(EffectType effect, int index) {
  switch (effect) {
    case EFFECT_PETALS:
      return (index % 2 == 0) ? GColorPastelYellow : GColorWhite;

    case EFFECT_SEEDS:
      return GColorWhite;

    case EFFECT_FIREFLIES:
      return GColorYellow;

    case EFFECT_LEAVES:
      switch (index % 3) {
        case 0: return GColorOrange;
        case 1: return GColorYellow;
        default: return GColorRed;
      }

    case EFFECT_SNOW:
      return GColorWhite;

    case EFFECT_NONE:
    default:
      return settings.TextColor;
  }
}

static void stop_particle_animation(void) {
  if (s_particle_timer) {
    app_timer_cancel(s_particle_timer);
    s_particle_timer = NULL;
  }

  s_particle_animating = false;
  s_current_effect = EFFECT_NONE;

  for (int i = 0; i < PARTICLE_COUNT; i++) {
    s_particles[i].active = false;
  }

  if (s_effect_layer) {
    layer_set_hidden(s_effect_layer, true);
    layer_mark_dirty(s_effect_layer);
  }
}

static void start_particle_animation(void) {
  if (s_particle_animating || !settings.AnimateLeaves) {
    return;
  }

  GRect bounds = layer_get_bounds(s_window_layer);
  int hour = s_current_hour >= 0 ? s_current_hour : 12;

  s_current_effect = get_effect_for_season_and_hour(s_current_season, hour);
  if (s_current_effect == EFFECT_NONE) {
    return;
  }

  s_particle_animating = true;

  for (int i = 0; i < PARTICLE_COUNT; i++) {
    Particle *p = &s_particles[i];
    p->active = true;
    p->step = 0;
    p->phase = i * 2;

    switch (s_current_effect) {
      case EFFECT_SNOW:
        p->x = rand() % bounds.size.w;
        p->y = -(rand() % 60) - (i * 10);
        p->dx = 0;
        p->dy = 1 + (i % 2);
        p->size = (i % 3 == 0) ? 4 : 3;
        break;

      case EFFECT_PETALS:
        p->x = -(rand() % 20) - (i * 6);
        p->y = 45 + (rand() % 70);
        p->dx = 1 + (i % 2);
        p->dy = (i % 2 == 0) ? 0 : 1;
        p->size = 4;
        break;

      case EFFECT_SEEDS:
        p->x = -(rand() % 20) - (i * 8);
        p->y = 35 + (rand() % 80);
        p->dx = 1;
        p->dy = 0;
        p->size = 3;
        break;

      case EFFECT_FIREFLIES:
        p->x = 20 + rand() % (bounds.size.w - 40);
        p->y = 45 + rand() % 70;
        p->dx = (i % 2 == 0) ? 1 : -1;
        p->dy = (i % 3 == 0) ? 1 : 0;
        p->size = (i % 4 == 0) ? 4 : 3;
        break;

      case EFFECT_LEAVES:
        p->x = -(rand() % 20) - (i * 8);
        p->y = 75 + (rand() % 45);
        p->dx = 2 + (i % 2);
        p->dy = (i % 2 == 0) ? 0 : 1;
        p->size = (i % 3 == 0) ? 5 : 4;
        break;

      case EFFECT_NONE:
      default:
        p->active = false;
        break;
    }
  }

  if (s_effect_layer) {
    layer_set_hidden(s_effect_layer, false);
    layer_mark_dirty(s_effect_layer);
  }

  s_particle_timer = app_timer_register(PARTICLE_FRAME_MS, particle_timer_callback, NULL);
}

static void particle_timer_callback(void *context) {
  (void)context;

  if (!s_particle_animating) {
    return;
  }

  static const int wobble_pattern[] = {0, 1, 1, 0, 0, -1, -1, 0};
  GRect bounds = layer_get_bounds(s_window_layer);
  bool any_active = false;

  for (int i = 0; i < PARTICLE_COUNT; i++) {
    Particle *p = &s_particles[i];
    if (!p->active) {
      continue;
    }

    any_active = true;
    p->step++;

    switch (s_current_effect) {
      case EFFECT_SNOW:
        p->y += p->dy;
        p->x += wobble_pattern[(p->step + p->phase) % 8];
        if (p->y > bounds.size.h) {
          p->active = false;
        }
        break;

      case EFFECT_PETALS:
        p->x += p->dx;
        p->y += p->dy + wobble_pattern[(p->step + p->phase) % 8];
        if (p->x > bounds.size.w + 8 || p->y > bounds.size.h + 8 || p->y < -8) {
          p->active = false;
        }
        break;

      case EFFECT_SEEDS:
        p->x += p->dx;
        p->y += wobble_pattern[(p->step + p->phase) % 8];
        if (p->x > bounds.size.w + 8 || p->y > bounds.size.h + 8 || p->y < -8) {
          p->active = false;
        }
        break;

      case EFFECT_FIREFLIES:
        p->x += p->dx;
        p->y += wobble_pattern[(p->step + p->phase) % 8] / 2;
        if (p->x < 0 || p->x > bounds.size.w || p->y < 30 || p->y > bounds.size.h - 20) {
          p->active = false;
        }
        break;

      case EFFECT_LEAVES:
        p->x += p->dx;
        p->y += p->dy + wobble_pattern[(p->step + p->phase) % 8];
        if (p->x > bounds.size.w + 10 || p->y > bounds.size.h + 10 || p->y < -10) {
          p->active = false;
        }
        break;

      case EFFECT_NONE:
      default:
        p->active = false;
        break;
    }
  }

  if (!any_active) {
    stop_particle_animation();
    return;
  }

  if (s_effect_layer) {
    layer_mark_dirty(s_effect_layer);
  }

  s_particle_timer = app_timer_register(PARTICLE_FRAME_MS, particle_timer_callback, NULL);
}

static void effect_layer_update_proc(Layer *layer, GContext *ctx) {
  (void)layer;

  if (!s_particle_animating || s_current_effect == EFFECT_NONE) {
    return;
  }

  for (int i = 0; i < PARTICLE_COUNT; i++) {
    Particle *p = &s_particles[i];
    if (!p->active) {
      continue;
    }

    switch (s_current_effect) {
      case EFFECT_SNOW:
        graphics_context_set_fill_color(ctx, get_particle_color(EFFECT_SNOW, i));
        graphics_fill_rect(ctx, GRect(p->x, p->y, p->size, p->size), 1, GCornersAll);
        break;

      case EFFECT_PETALS:
        graphics_context_set_fill_color(ctx, get_particle_color(EFFECT_PETALS, i));
        graphics_fill_rect(ctx, GRect(p->x, p->y, p->size, p->size - 1), 1, GCornersAll);
        break;

      case EFFECT_SEEDS:
        graphics_context_set_stroke_color(ctx, get_particle_color(EFFECT_SEEDS, i));
        graphics_draw_line(ctx, GPoint(p->x, p->y), GPoint(p->x + p->size + 1, p->y));
        graphics_draw_pixel(ctx, GPoint(p->x - 1, p->y - 1));
        graphics_draw_pixel(ctx, GPoint(p->x - 1, p->y + 1));
        break;

      case EFFECT_FIREFLIES:
        if (((p->step / 2) + i) % 4 != 0) {
          graphics_context_set_fill_color(ctx, get_particle_color(EFFECT_FIREFLIES, i));
          graphics_fill_rect(ctx, GRect(p->x, p->y, p->size, p->size), 1, GCornersAll);
        }
        break;

      case EFFECT_LEAVES:
        graphics_context_set_fill_color(ctx, get_particle_color(EFFECT_LEAVES, i));
        graphics_fill_rect(ctx, GRect(p->x, p->y, p->size, p->size - 1), 1, GCornersAll);
        break;

      case EFFECT_NONE:
      default:
        break;
    }
  }
}

static void refresh_bt_icon(void) {
  if (!s_bt_icon_layer) {
    return;
  }

  bool connected = connection_service_peek_pebble_app_connection();
  bool show_icon = (!connected && settings.ShowBtIcon);

  layer_set_hidden(bitmap_layer_get_layer(s_bt_icon_layer), !show_icon);
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  (void)axis;
  (void)direction;

  if (!settings.AnimateLeaves) {
    return;
  }

  stop_particle_animation();
  start_particle_animation();
}

static void apply_layout(void) {
  if (!s_window_layer || !s_time_layer || !s_ampm_layer || !s_date_layer || !s_panel_layer || !s_date_pill_layer ||
      !s_battery_layer || !s_bt_icon_layer) {
    return;
  }

  GRect bounds = layer_get_bounds(s_window_layer);
  bool is_24h = clock_is_24h_style();
  bool show_ampm = !is_24h;
  bool show_date = settings.ShowDate;

  if (settings.LowVisionMode) {
    const int panel_y = 10;
    const int panel_h = 50;
    layer_set_frame(s_panel_layer, GRect(2, panel_y, bounds.size.w - 4, panel_h));

    text_layer_set_font(s_time_layer, s_low_vision_time_font);
    if (show_ampm) {
      text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
      layer_set_frame(text_layer_get_layer(s_time_layer), GRect(2, panel_y - 1, 108, 42));
      layer_set_frame(text_layer_get_layer(s_ampm_layer), GRect(110, panel_y + 10, 24, 24));
    } else {
      text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
      layer_set_frame(text_layer_get_layer(s_time_layer), GRect(6, panel_y - 1, 132, 42));
      layer_set_frame(text_layer_get_layer(s_ampm_layer), GRect(132, panel_y + 10, 1, 24));
    }

    text_layer_set_font(s_ampm_layer, s_low_vision_detail_font);
    text_layer_set_text_alignment(s_ampm_layer, GTextAlignmentLeft);

    text_layer_set_font(s_date_layer, s_low_vision_detail_font);
    text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
    layer_set_frame(s_date_pill_layer, GRect(28, 122, bounds.size.w - 56, 26));
    layer_set_frame(text_layer_get_layer(s_date_layer), GRect(24, 119, bounds.size.w - 48, 28));

    layer_set_frame(s_battery_layer, GRect(20, bounds.size.h - 14, bounds.size.w - 40, 4));
    layer_set_frame(bitmap_layer_get_layer(s_bt_icon_layer), GRect(bounds.size.w - 36, (bounds.size.h / 2) - 15, 30, 30));
  } else {
    const int panel_y = 4;
    const int panel_h = 40;
    layer_set_frame(s_panel_layer, GRect(10, panel_y, bounds.size.w - 20, panel_h));

    text_layer_set_font(s_time_layer, s_low_vision_time_font);
    if (show_ampm) {
      text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
      layer_set_frame(text_layer_get_layer(s_time_layer), GRect(2, panel_y - 2, 108, 42));
      layer_set_frame(text_layer_get_layer(s_ampm_layer), GRect(110, panel_y + 9, 22, 18));
    } else {
      text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
      layer_set_frame(text_layer_get_layer(s_time_layer), GRect(6, panel_y - 2, 132, 42));
      layer_set_frame(text_layer_get_layer(s_ampm_layer), GRect(130, panel_y + 9, 1, 18));
    }

    text_layer_set_font(s_ampm_layer, s_ampm_font);
    text_layer_set_text_alignment(s_ampm_layer, GTextAlignmentLeft);

    text_layer_set_font(s_date_layer, s_date_font);
    text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
    layer_set_frame(s_date_pill_layer, GRect(0, 0, 1, 1));
    layer_set_frame(text_layer_get_layer(s_date_layer), GRect(16, 122, bounds.size.w - 32, 28));

    layer_set_frame(s_battery_layer, GRect(20, bounds.size.h - 12, bounds.size.w - 40, 2));
    layer_set_frame(bitmap_layer_get_layer(s_bt_icon_layer), GRect(bounds.size.w - 36, (bounds.size.h / 2) - 15, 30, 30));
  }

  layer_set_hidden(s_panel_layer, !settings.LowVisionMode);
  layer_set_hidden(s_date_pill_layer, !(settings.LowVisionMode && show_date));
  layer_set_hidden(text_layer_get_layer(s_ampm_layer), !show_ampm);
  layer_set_hidden(text_layer_get_layer(s_date_layer), !show_date);
}

static void update_time(void) {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_time_buffer[8];
  static char s_ampm_buffer[3];

  if (clock_is_24h_style()) {
    strftime(s_time_buffer, sizeof(s_time_buffer), "%H:%M", tick_time);
    s_ampm_buffer[0] = '\0';
  } else {
    int hour = tick_time->tm_hour % 12;
    if (hour == 0) {
      hour = 12;
    }

    snprintf(s_time_buffer, sizeof(s_time_buffer), "%d:%02d", hour, tick_time->tm_min);
    snprintf(s_ampm_buffer, sizeof(s_ampm_buffer), "%s", tick_time->tm_hour < 12 ? "AM" : "PM");
  }

  text_layer_set_text(s_time_layer, s_time_buffer);
  text_layer_set_text(s_ampm_layer, s_ampm_buffer);

  static char s_date_buffer[16];
  strftime(s_date_buffer, sizeof(s_date_buffer), "%a %b %d", tick_time);
  text_layer_set_text(s_date_layer, s_date_buffer);

  apply_layout();

  Season season = get_effective_season(tick_time);
  update_background(season, tick_time->tm_hour);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  (void)tick_time;
  (void)units_changed;
  update_time();
}

static void bluetooth_callback(bool connected) {
  bool show_icon = (!connected && settings.ShowBtIcon);
  layer_set_hidden(bitmap_layer_get_layer(s_bt_icon_layer), !show_icon);

  if (!connected && settings.BtVibrate) {
    vibes_double_pulse();
  }
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  (void)context;

  Tuple *text_color_t = dict_find(iterator, MESSAGE_KEY_TextColor);
  if (text_color_t) {
    settings.TextColor = GColorFromHEX(text_color_t->value->int32);
  }

  Tuple *show_date_t = dict_find(iterator, MESSAGE_KEY_ShowDate);
  if (show_date_t) {
    settings.ShowDate = show_date_t->value->int32 == 1;
  }

  Tuple *low_vision_mode_t = dict_find(iterator, MESSAGE_KEY_LowVisionMode);
  if (low_vision_mode_t) {
    settings.LowVisionMode = low_vision_mode_t->value->int32 == 1;
  }

  Tuple *animate_leaves_t = dict_find(iterator, MESSAGE_KEY_AnimateLeaves);
  if (animate_leaves_t) {
    settings.AnimateLeaves = animate_leaves_t->value->int32 == 1;
    if (!settings.AnimateLeaves) {
      stop_particle_animation();
    }
  }

  Tuple *show_bt_icon_t = dict_find(iterator, MESSAGE_KEY_ShowBtIcon);
  if (show_bt_icon_t) {
    settings.ShowBtIcon = show_bt_icon_t->value->int32 == 1;
  }

  Tuple *bt_vibrate_t = dict_find(iterator, MESSAGE_KEY_BtVibrate);
  if (bt_vibrate_t) {
    settings.BtVibrate = bt_vibrate_t->value->int32 == 1;
  }

  Tuple *season_mode_t = dict_find(iterator, MESSAGE_KEY_SeasonMode);
  if (season_mode_t) {
    settings.SeasonMode = season_mode_from_string(season_mode_t->value->cstring);
  }

  Tuple *manual_season_t = dict_find(iterator, MESSAGE_KEY_ManualSeason);
  if (manual_season_t) {
    settings.ManualSeason = season_from_string(manual_season_t->value->cstring);
  }

  if (text_color_t || show_date_t || low_vision_mode_t || animate_leaves_t || show_bt_icon_t ||
      bt_vibrate_t || season_mode_t || manual_season_t) {
    prv_save_settings();
    prv_update_display();
    s_current_hour = -1;
    stop_particle_animation();
    update_time();
    refresh_bt_icon();
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  (void)context;
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped! Reason: %d", reason);
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  (void)iterator;
  (void)context;
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed! Reason: %d", reason);
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  (void)iterator;
  (void)context;
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void prv_unobstructed_will_change(GRect final_unobstructed_screen_area, void *context) {
  (void)final_unobstructed_screen_area;
  (void)context;
  layer_set_hidden(bitmap_layer_get_layer(s_bt_icon_layer), true);
}

static void prv_unobstructed_change(AnimationProgress progress, void *context) {
  (void)progress;
  (void)context;
}

static void prv_unobstructed_did_change(void *context) {
  (void)context;
  refresh_bt_icon();
}

static void main_window_load(Window *window) {
  s_window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(s_window_layer);

  s_bg_bitmap = NULL;
  s_bg_layer = bitmap_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  layer_add_child(s_window_layer, bitmap_layer_get_layer(s_bg_layer));

  s_effect_layer = layer_create(bounds);
  layer_set_update_proc(s_effect_layer, effect_layer_update_proc);
  layer_set_hidden(s_effect_layer, true);
  layer_add_child(s_window_layer, s_effect_layer);

  s_panel_layer = layer_create(GRect(0, 0, bounds.size.w, 40));
  layer_set_update_proc(s_panel_layer, panel_layer_update_proc);

  s_date_pill_layer = layer_create(GRect(0, 0, bounds.size.w, 20));
  layer_set_update_proc(s_date_pill_layer, panel_layer_update_proc);

  s_battery_layer = layer_create(GRect(20, bounds.size.h - 12, bounds.size.w - 40, 2));
  layer_set_update_proc(s_battery_layer, battery_update_proc);

  s_date_font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_ampm_font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_low_vision_time_font = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
  s_low_vision_detail_font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);

  s_time_layer = text_layer_create(GRect(0, 0, bounds.size.w, 60));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, settings.TextColor);
  text_layer_set_font(s_time_layer, s_low_vision_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
  text_layer_set_overflow_mode(s_time_layer, GTextOverflowModeTrailingEllipsis);

  s_ampm_layer = text_layer_create(GRect(0, 0, 24, 20));
  text_layer_set_background_color(s_ampm_layer, GColorClear);
  text_layer_set_text_color(s_ampm_layer, settings.TextColor);
  text_layer_set_font(s_ampm_layer, s_ampm_font);
  text_layer_set_text_alignment(s_ampm_layer, GTextAlignmentLeft);
  text_layer_set_overflow_mode(s_ampm_layer, GTextOverflowModeTrailingEllipsis);

  s_date_layer = text_layer_create(GRect(0, bounds.size.h - 42, bounds.size.w, 32));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, settings.TextColor);
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_date_layer, GTextOverflowModeTrailingEllipsis);

  s_bt_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_ICON);
  s_bt_icon_layer = bitmap_layer_create(GRect(bounds.size.w - 36, (bounds.size.h / 2) - 15, 30, 30));
  bitmap_layer_set_bitmap(s_bt_icon_layer, s_bt_icon_bitmap);
  bitmap_layer_set_compositing_mode(s_bt_icon_layer, GCompOpSet);

  layer_add_child(s_window_layer, s_panel_layer);
  layer_add_child(s_window_layer, s_date_pill_layer);
  layer_add_child(s_window_layer, s_battery_layer);
  layer_add_child(s_window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_ampm_layer));
  layer_add_child(s_window_layer, text_layer_get_layer(s_date_layer));
  layer_add_child(s_window_layer, bitmap_layer_get_layer(s_bt_icon_layer));

  prv_update_display();
  update_time();
  battery_callback(battery_state_service_peek());
  refresh_bt_icon();

  prv_unobstructed_change(0, NULL);
  prv_unobstructed_did_change(NULL);

  UnobstructedAreaHandlers handlers = {
    .will_change = prv_unobstructed_will_change,
    .change = prv_unobstructed_change,
    .did_change = prv_unobstructed_did_change
  };
  unobstructed_area_service_subscribe(handlers, NULL);
}

static void main_window_unload(Window *window) {
  (void)window;

  stop_particle_animation();

  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_ampm_layer);
  text_layer_destroy(s_date_layer);

  layer_destroy(s_panel_layer);
  layer_destroy(s_date_pill_layer);
  layer_destroy(s_battery_layer);
  layer_destroy(s_effect_layer);

  gbitmap_destroy(s_bt_icon_bitmap);
  bitmap_layer_destroy(s_bt_icon_layer);

  if (s_bg_bitmap) {
    gbitmap_destroy(s_bg_bitmap);
  }
  bitmap_layer_destroy(s_bg_layer);
}

static void init(void) {
  prv_load_settings();

  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_callback
  });

  battery_state_service_subscribe(battery_callback);
  accel_tap_service_subscribe(accel_tap_handler);

  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  app_message_open(256, 256);
}

static void deinit(void) {
  unobstructed_area_service_unsubscribe();
  battery_state_service_unsubscribe();
  accel_tap_service_unsubscribe();
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
