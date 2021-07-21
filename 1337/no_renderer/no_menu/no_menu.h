#pragma once

#include <cstdint>
#include <stack>
#include <vector>
#include <string>
#include <string_view>
#include <type_traits>
#include <windows.h>
#include <algorithm>
#include <iomanip>
#include <sstream>
#define no_menu_API __declspec(dllexport)


namespace no_menu_functions {


}

namespace no_menu {
    // Multi selectable item.
    struct multi_select_item {
      std::string_view name;
      bool *value;
    };
    // Two dimensional vector.
    struct vec2 {
      float x, y;
    };
    // Color with 4 paremeters; red, green, blue and alpha.
    struct color {
      int r, g, b, a;
    };

    /// "Proxy" functions definitions.
    using line_t = std::add_pointer_t<void(int x, int y, int x2, int y2, color color) noexcept>;
    using rect_t = std::add_pointer_t<void(int x, int y, int x2, int y2, color color) noexcept>;
    using filled_rect_t = std::add_pointer_t<void(int x, int y, int x2, int y2, color color) noexcept>;
    using text_t = std::add_pointer_t<void(int x, int y, color color, bool center, const char *text) noexcept>;
    using get_text_size_t = std::add_pointer_t<void(const char *text, int &width, int &height) noexcept>;
    using get_frametime = std::add_pointer_t<float() noexcept>;
    using get_key_state_discord_t = std::add_pointer_t<short(int key_to_get) noexcept>;
    
    ///

    // "Proxy" functions stuff...
    struct functions_t {
      line_t draw_line;
      rect_t draw_rect;
      filled_rect_t draw_filled_rect;
      text_t draw_text;
      get_text_size_t get_text_size;
      get_frametime get_frametime;
      get_key_state_discord_t get_key_state_discord;
    };
    extern functions_t functions;

    // Flags for window appereance and its behavior.
    // ex: (no_menu_window_flags_no_border | no_menu_window_flags_no_titlebar) will cause window to be borderless and without title bar.
    enum no_menu_window_flags {
      no_menu_window_flags_none = 0,
      no_menu_window_flags_no_border = 1 << 0,
      no_menu_window_flags_no_titlebar = 1 << 1,
      no_menu_window_flags_no_ontoggle_animation = 1 << 2,
      no_menu_window_flags_no_move = 1 << 3,
      no_menu_window_flags_always_open = 1 << 4,
    };

    // Flags for text input appereance.
    // ex: (no_menu_text_input_flags_password) will convert text input (ex: "abcdef") to "******".
    enum no_menu_text_input_flags {
      no_menu_text_input_flags_none = 0,
      no_menu_text_input_flags_password = 1 << 0
    };

    // Flags for groupboxes appereance.
    // ex: (no_menu_groupbox_flags_title_centered) will center align title of groupbox.
    enum no_menu_groupbox_flags {
      no_menu_groupbox_flags_none = 0,
      no_menu_groupbox_flags_legacy_design = 1 << 0,
    };

    enum class no_menu_render_type {
      no_menu_line = 1,
      no_menu_rect,
      no_menu_filled_rect,
      no_menu_text
    };

    struct no_menu_control_render_t {
      vec2 draw_position;
      no_menu_render_type render_type;
      color color;
      std::string text;
      vec2 size;
      unsigned long font = 0;
    };

    struct gui_window_context_t {
      uint32_t blocking;
      std::stack<vec2> cursor_pos;
      std::stack<unsigned long> fonts;
      std::vector<no_menu_control_render_t> render;
      vec2 position;
      vec2 size;
      vec2 next_cursor_pos;
      bool dragging;
      bool opened;
      int alpha;
    };

    struct key_code_info {
      int vk;

      char regular;
      char shift;
    };

    namespace globals {
        constexpr std::string_view keys_list[]{
                "Error", "Left Mouse", "Right Mouse", "Break", "Middle Mouse", "Mouse 4", "Mouse 5",
                "Error", "Backspace", "TAB", "Error", "Error", "Error", "ENTER", "Error", "Error", "SHIFT",
                "CTRL", "ALT", "PAUSE", "CAPS LOCK", "Error", "Error", "Error", "Error", "Error", "Error",
                "Error", "Error", "Error", "Error", "Error", "SPACEBAR", "PG UP", "PG DOWN", "END", "HOME", "Left",
                "Up", "Right", "Down", "Error", "Print", "Error", "Print Screen", "Insert", "Delete", "Error", "0", "1",
                "2", "3", "4", "5", "6", "7", "8", "9", "Error", "Error", "Error", "Error", "Error", "Error",
                "Error", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
                "V", "W", "X", "Y", "Z", "Left Windows", "Right Windows", "Error", "Error", "Error", "NUM 0", "NUM 1",
                "NUM 2", "NUM 3", "NUM 4", "NUM 5", "NUM 6", "NUM 7", "NUM 8", "NUM 9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3",
                "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21",
                "F22", "F23", "F24", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
                "NUM LOCK", "SCROLL LOCK", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
                "Error", "Error", "Error", "Error", "Error", "Error", "Error", "LSHIFT", "RSHIFT", "LCONTROL",
                "RCONTROL", "LMENU", "RMENU", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
                "Error", "Error", "Error", "Next Track", "Previous Track", "Stop", "Play/Pause", "Error", "Error",
                "Error", "Error", "Error", "Error", ";", "+", ",", "-", ".", "/?", "~", "Error", "Error",
                "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
                "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
                "Error", "Error", "Error", "Error", "Error", "Error", "[{", "\\|", "}]", "'\"", "Error",
                "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
                "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
                "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
                "Error", "Error"
        };

        static key_code_info special_characters[22] = {
                {48,  '0',  ')'},
                {49,  '1',  '!'},
                {50,  '2',  '@'},
                {51,  '3',  '#'},
                {52,  '4',  '$'},
                {53,  '5',  '%'},
                {54,  '6',  '^'},
                {55,  '7',  '&'},
                {56,  '8',  '*'},
                {57,  '9',  '('},
                {32,  ' ',  ' '},
                {192, '`',  '~'},
                {189, '-',  '_'},
                {187, '=',  '+'},
                {219, '[',  '{'},
                {220, '\\', '|'},
                {221, ']',  '}'},
                {186, ';',  ':'},
                {222, '\'', '"'},
                {188, ',',  '<'},
                {190, '.',  '>'},
                {191, '/',  '?'}
        };

        // Color definition. Can be changed at any time just simply by editing this struct.
        static struct {
          color window_border_inner_fill{60, 60, 60, 255};
          color window_border_fill{40, 40, 40, 255};
          color window_border_color{10, 10, 10, 255};
          color window_background{40, 40, 40, 255};

          color control_outline{23, 23, 30, 255};
          color control_active_or_clicked{108, 92, 231, 255};
          color control_idle{62, 62, 72, 255};

          color color_groupbox_bg{50, 50, 50, 255};
          color color_text{203, 203, 203, 255};
          color color_text_dimmer{99, 110, 114, 255};
          color color_slider{108, 92, 231, 255};
          color color_combo_bg{108, 92, 231, 255};
          color color_groupbox_header{26, 26, 26, 150};
        } global_colors;

        static struct {
          // Base position of first drawn control (px). DO NOT change if its necessary
          no_menu::vec2 base_pos{16, 23};
          // Spacing between items (px)
          int item_spacing = 16;
          // Key that will toggle menu visibility unless no_menu_window_flags_always_open is set
          int menu_toggle_key = VK_INSERT;
        } global_config;

        // Window definitions.
        extern gui_window_context_t window_ctx;

        // "Proxy" functions stuff...
        //inline no_menu::functions_t no_menu::functions;

        // Globals
        static vec2 mouse_pos;
        static vec2 previous_mouse_pos;

        // Input handling stuff
        static bool key_state[256];
        static bool prev_key_state[256];

        // Check for input polling.
        static bool input_loop_started = false;
    }

    // Start Input loop
    no_menu_API void window_input(LPCSTR classname, LPCSTR windowname);

    no_menu_API void poll_input(HWND hwnd);

    namespace utils {
        namespace input {
            bool key_pressed(int key);

            bool key_down(int key);

            bool key_released(int key);

            bool mouse_in_region(int x, int y, int w, int h);

        }

        namespace hash {
            std::vector<std::string> split_str(const char *str, char separator);

            uint32_t hash(const char *str, uint32_t value = 0x811c9dc5);
        }

        namespace misc {
            no_menu_API void push_cursor_pos(vec2 pos);

            no_menu_API vec2 pop_cursor_pos();

            no_menu_API void push_font(unsigned long font);

            no_menu_API unsigned long pop_font();
        }

    }

    no_menu_API bool begin_window(std::string_view title, vec2 default_size, int MenuToggleKey, int flags = 0);

    no_menu_API void end_window();

    no_menu_API void begin_groupbox(std::string_view title, vec2 size, int flags = 0);

    no_menu_API void end_groupbox();

    no_menu_API void checkbox(const char *id, bool &value);

    no_menu_API void toggle_button(const char *id, vec2 size, bool &value);

    no_menu_API bool button(const char* id, vec2 size);

    no_menu_API bool TextHoverable(const char* id, no_menu::color col);

    no_menu_API void MiniBox(const char* id);

    no_menu_API bool Lineee(vec2 size, bool invisible);

    no_menu_API void key_bind(const char *id, int &value);

    no_menu_API void text_input(const char *id, std::string &value, int max_length = 16, int flags = 0);

    no_menu_API void slider_int(const char *id, int min, int max, int &value);

    no_menu_API void slider_float(const char *id, float min, float max, float &value);

    no_menu_API void combobox(const char *, std::vector<std::string> items, int &value);

    no_menu_API void multi_combobox(const char *id, std::vector<multi_select_item> items);

    no_menu_API void listbox(const char *id, std::vector<multi_select_item> items);

    no_menu_API bool clickable_text(const char *id);

    no_menu_API void text(const char *text);

    no_menu_API void separator(int space, bool invisible);

    no_menu_API void SetWidgetPosition(int x, int y);

    no_menu_API void next_column(int pusher_x = 174, int pusher_y = 14);

    no_menu_API void back_column(int back_x = 174, int back_y = 14);

    no_menu_API void same_line(float x_axis = -1);

    no_menu_API void backup_line();

}
