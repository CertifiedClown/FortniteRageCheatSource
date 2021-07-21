#include "no_menu.h"
#include <includes.h>

using namespace no_menu::globals;
no_menu::functions_t no_menu::functions;
no_menu::gui_window_context_t no_menu::globals::window_ctx;

// ========================================================================
bool no_menu::button(const char* id, const vec2 size) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{ window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y };

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    bool result = false;
    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && utils::input::key_pressed(VK_LBUTTON)) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        result = hovered;
    }

    int text_width, text_height;
    functions.get_text_size(id_split[0].c_str(), text_width, text_height);

    window_ctx.render.emplace_back(no_menu_control_render_t{ {draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y + size.y / 2 - text_height / 2}, no_menu_render_type::no_menu_text, global_colors.color_text, id_split[0], vec2{0, 0}, font });
    window_ctx.render.emplace_back(no_menu_control_render_t{ {draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {size.x - 2, size.y - 2} });
    window_ctx.render.emplace_back(no_menu_control_render_t{ {draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_filled_rect, global_colors.control_outline, "", size });

    utils::misc::push_cursor_pos(vec2{ cursor_pos.x + size.x + global_config.item_spacing, cursor_pos.y });
    utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + size.y / 2 + global_config.item_spacing });

    utils::misc::push_font(font);

    return result;
}
// ========================================================================




void no_menu::MiniBox(const char* id) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    //const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    //const vec2 draw_pos{ window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y };

    int text_width, text_height;
    functions.get_text_size(id_split[0].c_str(), text_width, text_height);

    const vec2 cursor_position = vec2({ mouse_pos.x, mouse_pos.y });

    //functions.draw_filled_rect(cursor_position.x + 15, cursor_position.y + 15, text_width + 6, text_height + 6, { 19, 19, 25, 255 });
    window_ctx.render.emplace_back(no_menu_control_render_t{ {cursor_position.x + 18, cursor_position.y + 18}, no_menu_render_type::no_menu_text, {255, 255, 255, 255}, id_split[0], vec2{0, 0}, font });

    //utils::misc::push_cursor_pos(vec2{ cursor_pos.x + text_width + global_config.item_spacing, cursor_pos.y });
    //utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + text_height / 2 + global_config.item_spacing });

    utils::misc::push_font(font);
}

bool no_menu::TextHoverable(const char* id, no_menu::color col) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{ window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y };


    int text_width, text_height;
    functions.get_text_size(id_split[0].c_str(), text_width, text_height);

    const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, text_width, text_height);





    window_ctx.render.emplace_back(no_menu_control_render_t{ {draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_text, col, id_split[0], vec2{0, 0}, font });

    utils::misc::push_cursor_pos(vec2{ cursor_pos.x + text_width + global_config.item_spacing, cursor_pos.y });
    utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + text_height / 2 + global_config.item_spacing });

    utils::misc::push_font(font);

    return hovered;
}



// ========================================================================
bool no_menu::Lineee(const vec2 size, const bool invisible) {


    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
   

    if (invisible) {
        const vec2 draw_pos{ window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y };
        window_ctx.render.emplace_back(no_menu_control_render_t{ {draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, global_colors.control_active_or_clicked, "", {size.x - 2, size.y - 2} });
    }
    


    utils::misc::push_cursor_pos(vec2{ cursor_pos.x + size.x + global_config.item_spacing, cursor_pos.y });
    utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + size.y / 2 + 2 });


    return true;
}
// ========================================================================



// ========================================================================
void no_menu::checkbox(const char *id, bool &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_height = 8;
    const int control_width = 8;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    int text_width, text_height;
    functions.get_text_size(id_split[0].c_str(), text_width, text_height);

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width + 6 + text_width, control_height); !active && hovered && utils::input::key_pressed(VK_LBUTTON)) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        value = !value;
    }

    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 14, draw_pos.y - 2}, no_menu_render_type::no_menu_text, value ? global_colors.color_text : global_colors.color_text_dimmer, id_split[0], vec2{0, 0}, font});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, value ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_filled_rect, global_colors.control_outline, "", {control_width, control_height}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + 14 + text_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + global_config.item_spacing});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
void no_menu::combobox(const char *id, std::vector<std::string> items, int &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 70;
    const int control_height = 24;

    value = std::clamp(value, 0, static_cast<int>(items.size()) - 1);

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;

        functions.get_text_size(id_split[0].c_str(), text_width, text_height);
        text_height = text_height + 4;
        window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y - 4}, no_menu_render_type::no_menu_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + control_width - 10, draw_pos.y + 4}, no_menu_render_type::no_menu_text, global_colors.color_text, "+", vec2{0, 0}, font});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 4, draw_pos.y + 4}, no_menu_render_type::no_menu_text, global_colors.color_text, items.at(value), vec2{0, 0}, font});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_filled_rect, global_colors.control_outline, "", {control_width, control_height}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});


    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (window_ctx.blocking == utils::hash::hash(id)) {
        for (int i = 1; i <= items.size(); i++) {
            bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);

            if (hovered && utils::input::key_pressed(VK_LBUTTON)) {
                window_ctx.blocking = 0;
                value = i - 1;
            }

            if (!hovered && utils::input::key_pressed(VK_LBUTTON)) {
                window_ctx.blocking = 0;
            }
            bool selected = value == i - 1;
            window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 4, draw_pos.y + (control_height - 1) * i + 4}, no_menu_render_type::no_menu_text, selected ? global_colors.control_active_or_clicked : global_colors.color_text, items.at(i - 1), vec2{0, 0}, font});
            window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + (19 * i) + 1}, no_menu_render_type::no_menu_filled_rect, hovered ? global_colors.color_combo_bg : global_colors.control_idle, "", {control_width - 2, control_height - 2 + 9}});
            window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y + 19 * i}, no_menu_render_type::no_menu_filled_rect, global_colors.control_outline, "", {control_width, control_height + 9}});
        }
    }

    utils::misc::push_font(font);
}

// ========================================================================
void no_menu::multi_combobox(const char *id, std::vector<multi_select_item> items) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 100;
    const int control_height = 20;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y - 4}, no_menu_render_type::no_menu_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    std::string value_str;
    int text_width, text_height;

    for (auto &item_t : items) {
        if (*item_t.value) {
            if (value_str.length() > 0)
                value_str += _(", ");

            value_str += item_t.name;
        }
    }

    functions.get_text_size(value_str.c_str(), text_width, text_height);
    if (text_width > control_width - 18) {
        value_str.resize(control_width / 10);
        value_str += _(" ...");
    }
    if (!value_str.length())
        value_str += _("None");

    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + control_width - 10, draw_pos.y + 4}, no_menu_render_type::no_menu_text, global_colors.color_text, "+", vec2{0, 0}, font});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 4, draw_pos.y + 4}, no_menu_render_type::no_menu_text, global_colors.color_text, value_str, vec2{0, 0}, font});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_filled_rect, global_colors.control_outline, "", {control_width, control_height}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});


    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (window_ctx.blocking == utils::hash::hash(id)) {
        for (int i = 1; i <= items.size(); i++) {
            bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);
            const bool outofbounds = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1), control_width, control_height * i);
            if (hovered && utils::input::key_pressed(VK_LBUTTON)) {
                window_ctx.blocking = utils::hash::hash(id);
                *items[i - 1].value = !*items[i - 1].value;
            }
            if (!outofbounds && utils::input::key_pressed(VK_LBUTTON)) {
                window_ctx.blocking = 0;
            }
            bool selected = *items[i - 1].value >= 1;
            window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 4, draw_pos.y + (control_height - 1) * i + 4}, no_menu_render_type::no_menu_text, selected ? global_colors.control_active_or_clicked : global_colors.color_text, items[i - 1].name.data(), vec2{0, 0}, font});
            window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + (19 * i) + 1}, no_menu_render_type::no_menu_filled_rect, hovered ? global_colors.color_combo_bg : global_colors.control_idle, "", {control_width - 2, control_height - 2}});
            window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y + 19 * i}, no_menu_render_type::no_menu_filled_rect, global_colors.control_outline, "", {control_width, control_height}});
        }
    }

    utils::misc::push_font(font);
}
// ========================================================================


// ========================================================================
void no_menu::begin_groupbox(std::string_view title, const vec2 size, const int flags) {
    const unsigned long font = utils::misc::pop_font();


    


    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    int text_width, text_height;

    if (!title.empty())
        functions.get_text_size(title.data(), text_width, text_height);

    const int header_height = title.empty() ? 16 : text_height + 3;

    if (flags & no_menu_groupbox_flags_legacy_design) {
        functions.draw_rect(draw_pos.x - 1, draw_pos.y - 1, size.x + 2, size.y + 2, global_colors.control_outline);
        functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, global_colors.color_groupbox_bg);

        if (!title.empty()) {
            functions.draw_text(draw_pos.x + 4, draw_pos.y - 8, global_colors.color_text, false, title.data());
        }
    }
    else {
        functions.draw_rect(draw_pos.x - 1, draw_pos.y - 1, size.x + 2, size.y + 2, global_colors.control_outline);
        functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, global_colors.color_groupbox_bg);

        functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, header_height, global_colors.color_groupbox_header);

        if (!title.empty()) {
            functions.draw_text(draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y + 2, global_colors.color_text, false, title.data());
        }
    }

    window_ctx.next_cursor_pos = vec2{cursor_pos.x, cursor_pos.y + size.y + 10};

    utils::misc::push_cursor_pos(vec2{ cursor_pos.x + 8, cursor_pos.y + ((flags & no_menu_groupbox_flags_legacy_design) ? 0 : header_height) + 8 });

    utils::misc::push_font(font);
}

// ========================================================================
void no_menu::end_groupbox() {
    utils::misc::push_cursor_pos(window_ctx.next_cursor_pos);
    window_ctx.next_cursor_pos = {};
}
// ========================================================================


// ========================================================================
void no_menu::key_bind(const char *id, int &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 80;
    const int control_height = 20;

    value = std::clamp(value, 0, 255);

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y - 4}, no_menu_render_type::no_menu_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active) {
        for (int i = 0; i < 256; i++) {
            if (utils::input::key_pressed(i)) {
                if (keys_list[i] != _("Error"))
                    value = i;

                window_ctx.blocking = 0;
            }
        }
    }

    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 4, draw_pos.y + 4}, no_menu_render_type::no_menu_text, global_colors.color_text, active ? "Press any key" : keys_list[value].data(), vec2{0, 0}, font});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {control_width, control_height}});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_filled_rect, global_colors.control_outline, "", {control_width + 2, control_height + 2}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});

    utils::misc::push_font(font);
}
// ========================================================================



// ========================================================================
void no_menu::listbox(const char *id, std::vector<multi_select_item> items) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 100;
    const int control_height = 20;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y - 4}, no_menu_render_type::no_menu_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    for (int i = 1; i <= items.size(); i++) {
        const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * (i - 1), control_width, control_height);

        if (hovered && utils::input::key_pressed(VK_LBUTTON)) {
            window_ctx.blocking = 0;
            *items[i - 1].value = !*items[i - 1].value;
        }
        window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 4, draw_pos.y + (control_height - 1) * (i - 1) + 4}, no_menu_render_type::no_menu_text, *items[i - 1].value || hovered ? global_colors.control_active_or_clicked : global_colors.color_text, items[i - 1].name.data(), vec2{0, 0}, font});
    }


    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, global_colors.control_idle, "", {control_width - 2, static_cast<float>(control_height * items.size() - 2)}});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_filled_rect, global_colors.control_outline, "", {control_width, static_cast<float>(control_height * items.size())}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) + control_height * (items.size() - 1)});

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
void no_menu::slider_int(const char *id, const int min, const int max, int &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const int control_width = 120;
    const int control_height = 10;

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y - 4}, no_menu_render_type::no_menu_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    if (window_ctx.blocking == 0 && utils::input::mouse_in_region(draw_pos.x - (control_height - 2), draw_pos.y, 8, 10) && utils::input::key_pressed(VK_LBUTTON))
        value = std::clamp(value - 1, min, max);
    else if (window_ctx.blocking == 0 && utils::input::mouse_in_region(draw_pos.x + control_width, draw_pos.y, 8, 10) && utils::input::key_pressed(VK_LBUTTON))
        value = std::clamp(value + 1, min, max);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (utils::input::key_down(VK_LBUTTON) && window_ctx.blocking == utils::hash::hash(id)) {
        float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
        int value_mapped = static_cast<int>(value_unmapped / control_width * (max - min) + min);

        value = value_mapped;
    }
    else if (!utils::input::key_down(VK_LBUTTON) && window_ctx.blocking == utils::hash::hash(id)) {
        window_ctx.blocking = 0;
    }

    const int dynamic_width = (static_cast<float>(value) - min) / (max - min) * control_width - 2;

    int text_width, text_height;
    std::string value_str = std::to_string(value);
    functions.get_text_size(value_str.c_str(), text_width, text_height);

    int text_x = dynamic_width - text_width;

    if (text_x < 0)
        text_x = 0;

    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x - (control_height - 2), draw_pos.y - 2}, no_menu_render_type::no_menu_text, global_colors.color_text_dimmer, "-", vec2{0, 0}, font});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + (control_width + 4), draw_pos.y - 2}, no_menu_render_type::no_menu_text, global_colors.color_text_dimmer, "+", vec2{0, 0}, font});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + text_x, draw_pos.y}, no_menu_render_type::no_menu_text, global_colors.color_text, value_str, vec2{0, 0}, font});

    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, global_colors.color_slider, "", {static_cast<float>(dynamic_width), control_height - 2}});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_filled_rect, global_colors.control_outline, "", {control_width, control_height}});


    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + 14 + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});

    utils::misc::push_font(font);
}

// ========================================================================
void no_menu::slider_float(const char *id, const float min, const float max, float &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 120;
    const int control_height = 10;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y - 4}, no_menu_render_type::no_menu_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    if (window_ctx.blocking == 0 && utils::input::mouse_in_region(draw_pos.x - (control_height - 2), draw_pos.y, 8, 10) && utils::input::key_pressed(VK_LBUTTON))
        value = std::clamp(value - 1, min, max);
    else if (window_ctx.blocking == 0 && utils::input::mouse_in_region(draw_pos.x + control_width, draw_pos.y, 8, 10) && utils::input::key_pressed(VK_LBUTTON))
        value = std::clamp(value + 1, min, max);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (utils::input::key_down(VK_LBUTTON) && window_ctx.blocking == utils::hash::hash(id)) {
        float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
        float value_mapped = static_cast<float>((value_unmapped / static_cast<float>(control_width)) * (max - min) + min);

        value = value_mapped;
    }
    else if (!utils::input::key_down(VK_LBUTTON) && window_ctx.blocking == utils::hash::hash(id)) {
        window_ctx.blocking = 0;
    }

    const float dynamic_width = (static_cast<float>(value) - min) / (max - min) * control_width - 2;

    int text_width, text_height;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << value;
    std::string value_str = ss.str();
    functions.get_text_size(value_str.c_str(), text_width, text_height);

    int text_x = dynamic_width - text_width;

    if (text_x < 0)
        text_x = 0;

    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x - (control_height - 2), draw_pos.y - 2}, no_menu_render_type::no_menu_text, global_colors.color_text_dimmer, "-", vec2{0, 0}, font});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + (control_width + 4), draw_pos.y - 2}, no_menu_render_type::no_menu_text, global_colors.color_text_dimmer, "+", vec2{0, 0}, font});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + text_x, draw_pos.y}, no_menu_render_type::no_menu_text, global_colors.color_text, value_str, vec2{0, 0}, font});

    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, global_colors.color_slider, "", {dynamic_width, control_height - 2}});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_filled_rect, global_colors.control_outline, "", {control_width, control_height}});


    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + 14 + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
bool no_menu::clickable_text(const char *id) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    int text_width, text_height;
    functions.get_text_size(id_split[0].c_str(), text_width, text_height);

    const bool active = window_ctx.blocking == utils::hash::hash(id);
    const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, text_width, text_height);

    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_text, (hovered || window_ctx.blocking == utils::hash::hash(id)) ? global_colors.control_active_or_clicked : global_colors.color_text, id_split[0], vec2{0, 0}, font});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + text_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + text_height / 2 + global_config.item_spacing});

    bool result = false;

    if (!active && hovered && utils::input::key_pressed(VK_LBUTTON)) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        result = hovered;
    }

    utils::misc::push_font(font);

    return result;
}

// ========================================================================
void no_menu::text(const char *text) {
    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    int text_width, text_height;
    functions.get_text_size(text, text_width, text_height);

    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_text, global_colors.color_text, text, vec2{0, 0}, font});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + text_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + text_height / 2 + global_config.item_spacing});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
void no_menu::text_input(const char *id, std::string &value, const int max_length, const int flags) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 150;
    const int control_height = 24;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(id_split[0].c_str(), text_width, text_height);
        text_height = text_height + 2;
        window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y - 4}, no_menu_render_type::no_menu_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    const bool active = window_ctx.blocking == utils::hash::hash(id);
    const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height);

    if (hovered && utils::input::key_pressed(VK_LBUTTON) && !active) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active) {
        if (utils::input::key_pressed(VK_ESCAPE) || utils::input::key_pressed(VK_RETURN) || (!hovered && utils::input::key_pressed(VK_LBUTTON))) {
            window_ctx.blocking = 0;
        }
        else if (utils::input::key_pressed(VK_BACK) && !value.empty()) {
            value.pop_back();
        }
        else if (value.length() < max_length) {
            for (int i = 32; i <= 222; i++) {
                if ((i > 32 && i < 48) || (i > 57 && i < 65) || (i > 90 && i < 186))
                    continue;

                if (i > 57 && i <= 90) {
                    if (utils::input::key_pressed(i))
                        value += utils::input::key_down(VK_SHIFT) ? static_cast<char>(i) : static_cast<char>(i + 32);
                }
                else {
                    if (utils::input::key_pressed(i)) {
                        for (int j = 0; j < sizeof(special_characters); j++) {
                            if (special_characters[j].vk == i)
                                value += utils::input::key_down(VK_SHIFT) ? special_characters[j].shift : special_characters[j].regular;
                        }
                    }
                }
            }
        }
    }

    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 4, draw_pos.y + 4}, no_menu_render_type::no_menu_text, global_colors.color_text, flags & no_menu_text_input_flags_password ? std::string(value.length(), '*').c_str() : value.c_str(), vec2{0, 0}, font});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_filled_rect, global_colors.control_outline, "", {control_width, control_height}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
void no_menu::toggle_button(const char *id, const vec2 size, bool &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && utils::input::key_pressed(VK_LBUTTON)) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        value = !value;
    }

    int text_width, text_height;
    functions.get_text_size(id_split[0].c_str(), text_width, text_height);

    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y + size.y / 2 - text_height / 2}, no_menu_render_type::no_menu_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, no_menu_render_type::no_menu_filled_rect, value ? global_colors.control_active_or_clicked : global_colors.control_idle, "", size});
    window_ctx.render.emplace_back(no_menu_control_render_t{{draw_pos.x, draw_pos.y}, no_menu_render_type::no_menu_filled_rect, global_colors.control_outline, "", {size.x + 2, size.y + 2}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + size.x + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + size.y / 2 + global_config.item_spacing});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
bool no_menu::begin_window(std::string_view title, const vec2 windowsize, const int MenuToggleKey, const int flags) {
    if (!input_loop_started)
        throw std::exception(_("Input loop didnt start or didnt start properly."));;

   // window_ctx.position.x = 300;
   // window_ctx.position.y = 300;


    if (!(flags & no_menu_window_flags_always_open)) {
        if (utils::input::key_pressed(MenuToggleKey))
            window_ctx.opened = !window_ctx.opened;
    }
    else
        window_ctx.opened = true;

    if (const int prev_alpha = window_ctx.alpha; !(flags & no_menu_window_flags_no_ontoggle_animation)) {
        const int fade_factor = static_cast<int>(1.0f / 0.15f * functions.get_frametime() * 255);
        window_ctx.alpha = std::clamp(window_ctx.alpha + (window_ctx.opened ? fade_factor : -fade_factor), 0, 255);

        if (window_ctx.alpha != prev_alpha) {
            global_colors.window_border_inner_fill.a = window_ctx.alpha;
            global_colors.window_border_fill.a = window_ctx.alpha;
            global_colors.window_border_color.a = window_ctx.alpha;
            global_colors.window_background.a = window_ctx.alpha;

            global_colors.control_outline.a = window_ctx.alpha;
            global_colors.control_active_or_clicked.a = window_ctx.alpha;
            global_colors.control_idle.a = window_ctx.alpha;

            global_colors.color_groupbox_bg.a = window_ctx.alpha;
            global_colors.color_text.a = window_ctx.alpha;
            global_colors.color_text_dimmer.a = window_ctx.alpha;
            global_colors.color_slider.a = window_ctx.alpha;
        }
    }

    if (window_ctx.opened || window_ctx.alpha > 0) {
        if (!(flags & no_menu_window_flags_no_move)) {
            if ((flags & no_menu_window_flags_no_border ? utils::input::mouse_in_region(window_ctx.position.x + 9, window_ctx.position.y + 14, window_ctx.size.x - 18, 14)
                                                     : utils::input::mouse_in_region(window_ctx.position.x - 6, window_ctx.position.y - 10, window_ctx.size.x + 12, 16))
                && utils::input::key_pressed(VK_LBUTTON) && !window_ctx.dragging) {
                window_ctx.dragging = true;
            }
            else if (utils::input::key_down(VK_LBUTTON) && window_ctx.dragging) {
                const vec2 mouse_delta{mouse_pos.x - previous_mouse_pos.x, mouse_pos.y - previous_mouse_pos.y};
                const vec2 new_position{window_ctx.position.x + mouse_delta.x, window_ctx.position.y + mouse_delta.y};

                window_ctx.position = new_position;
            }
            else if (!utils::input::key_down(VK_LBUTTON) && window_ctx.dragging) {
                window_ctx.dragging = false;
            }
        }

        if (window_ctx.size.x < 1 && window_ctx.size.y < 1)
            window_ctx.size = windowsize;

        if (!(flags & no_menu_window_flags_no_border)) {
            functions.draw_filled_rect(window_ctx.position.x - 6, window_ctx.position.y - 10, window_ctx.size.x + 12, window_ctx.size.y + 16, global_colors.window_border_inner_fill);
            functions.draw_filled_rect(window_ctx.position.x - 5, window_ctx.position.y - 9, window_ctx.size.x + 10, window_ctx.size.y + 14, global_colors.window_border_color);
            functions.draw_filled_rect(window_ctx.position.x - 4, window_ctx.position.y - 8, window_ctx.size.x + 8, window_ctx.size.y + 12, global_colors.window_border_fill);
            functions.draw_filled_rect(window_ctx.position.x, window_ctx.position.y + 7, window_ctx.size.x, window_ctx.size.y - 7, global_colors.window_border_color);
            functions.draw_filled_rect(window_ctx.position.x + 1, window_ctx.position.y + 8, window_ctx.size.x - 2, window_ctx.size.y - 9, global_colors.window_border_inner_fill);
            functions.draw_filled_rect(window_ctx.position.x + 8, window_ctx.position.y + 15, window_ctx.size.x - 16, window_ctx.size.y - 23, global_colors.window_border_color);
        }

        if (!(flags & no_menu_window_flags_no_titlebar)) {
            int text_width, text_height;
            functions.get_text_size(title.data(), text_width, text_height);
            functions.draw_text((window_ctx.position.x + window_ctx.size.x * 0.5) - text_width / 2, (window_ctx.position.y + (window_ctx.size.y * 0.010) - 6) - text_height / 2, global_colors.color_text, true, title.data());
        }
            

        functions.draw_filled_rect(window_ctx.position.x + 9, window_ctx.position.y + 16, window_ctx.size.x - 18, window_ctx.size.y - 25, global_colors.window_background);


        utils::misc::push_font(NULL);
        utils::misc::push_cursor_pos(global_config.base_pos);
    }

    return window_ctx.opened || window_ctx.alpha > 0;
}

// ========================================================================
void no_menu::end_window() {
    for (int i = window_ctx.render.size() - 1; i >= 0; i--) {
        switch (window_ctx.render[i].render_type) {
            case no_menu_render_type::no_menu_line:
                functions.draw_line(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].size.x, window_ctx.render[i].size.y, window_ctx.render[i].color);
                break;
            case no_menu_render_type::no_menu_rect:
                functions.draw_rect(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].size.x, window_ctx.render[i].size.y, window_ctx.render[i].color);
                break;
            case no_menu_render_type::no_menu_filled_rect:
                functions.draw_filled_rect(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].size.x, window_ctx.render[i].size.y, window_ctx.render[i].color);
                break;
            case no_menu_render_type::no_menu_text:
                functions.draw_text(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].color, false, window_ctx.render[i].text.c_str());
                break;
        }
    }

    window_ctx.render.clear();

    while (!window_ctx.cursor_pos.empty())
        window_ctx.cursor_pos.pop();
}
// ========================================================================

// ========================================================================
void no_menu::separator(int space, bool invisible){
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + space });
}

// ========================================================================
void no_menu::next_column(const int pusher_x, const int pusher_y) {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 new_cursor_pos{cursor_pos.x + pusher_x, global_config.base_pos.y + pusher_y};

    if (window_ctx.next_cursor_pos.y != 0)
        new_cursor_pos.y += 14;

    utils::misc::push_cursor_pos(new_cursor_pos);
}

// ========================================================================

void no_menu::SetWidgetPosition(const int x, const int y) {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 new_cursor_pos{ global_config.base_pos.x + x, global_config.base_pos.y + y };

    if (window_ctx.next_cursor_pos.y != 0)
        new_cursor_pos.y += 14;

    utils::misc::push_cursor_pos(new_cursor_pos);
}

// ========================================================================

void no_menu::back_column(const int back_x, const int back_y) {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 new_cursor_pos{ cursor_pos.x - back_x, global_config.base_pos.y + back_y };

    if (window_ctx.next_cursor_pos.y != 0)
        new_cursor_pos.y += 14;

    utils::misc::push_cursor_pos(new_cursor_pos);
}

// ========================================================================
void no_menu::same_line(const float x_axis) {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();

    if (x_axis != -1)
        utils::misc::push_cursor_pos(vec2{global_config.base_pos.x + x_axis, cursor_pos.x});
}

// ========================================================================
void no_menu::backup_line() {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();

    utils::misc::push_cursor_pos(vec2{window_ctx.next_cursor_pos.x, cursor_pos.y});
}
// ========================================================================

// ========================================================================
std::vector<std::string> no_menu::utils::hash::split_str(const char *str, const char separator) {
    std::vector<std::string> output;
    std::string substring;
    std::istringstream stream{str};

    while (std::getline(stream, substring, separator))
        output.push_back(substring);

    return output;
}

// ========================================================================
uint32_t no_menu::utils::hash::hash(const char *str, const uint32_t value) {
    return *str ? hash(str + 1, (value ^ *str) * 0x1000193ull) : value;
}
// ========================================================================

// ========================================================================
// Function for starting our input loop.
void no_menu::window_input(LPCSTR classname, LPCSTR windowname) {

    for (int i = 0; i < 256; i++) {
        prev_key_state[i] = key_state[i];
        key_state[i] = functions.get_key_state_discord(i);
    }

    POINT p_mouse_pos;
    GetCursorPos(&p_mouse_pos);
    ScreenToClient(FindWindowA(classname, windowname), &p_mouse_pos);
    previous_mouse_pos = mouse_pos;
    mouse_pos = vec2{static_cast<float>(p_mouse_pos.x), static_cast<float>(p_mouse_pos.y)};

    if (!input_loop_started)
        input_loop_started = true;
}

// ========================================================================
// Function for starting our input loop.
void no_menu::poll_input(HWND hwnd) {
    if (!hwnd)
        throw std::exception(_("No window from where input should be read from specified in function parameter."));

    for (int i = 0; i < 256; i++) {
        prev_key_state[i] = key_state[i];
        key_state[i] = functions.get_key_state_discord(i);
    }

    POINT p_mouse_pos;
    GetCursorPos(&p_mouse_pos);
    ScreenToClient(hwnd, &p_mouse_pos);
    previous_mouse_pos = mouse_pos;

    mouse_pos = vec2{static_cast<float>(p_mouse_pos.x), static_cast<float>(p_mouse_pos.y)};

    if (!input_loop_started)
        input_loop_started = true;
}

// ========================================================================
bool no_menu::utils::input::key_pressed(const int key) {
    return key_state[key] && !prev_key_state[key];
}

// ========================================================================
bool no_menu::utils::input::key_down(const int key) {
    return key_state[key];
}

// ========================================================================
bool no_menu::utils::input::key_released(const int key) {
    return !key_state[key] && prev_key_state[key];
}

// ========================================================================
// Check if mouse is hovered over specified region.
bool no_menu::utils::input::mouse_in_region(const int x, const int y, const int w, const int h) {
    return mouse_pos.x > x && mouse_pos.y > y && mouse_pos.x < w + x && mouse_pos.y < h + y;
}
// ========================================================================

// ========================================================================
// Push cursor position to the stack defined in window context.
void no_menu::utils::misc::push_cursor_pos(const vec2 pos) {
    window_ctx.cursor_pos.push(pos);
}

// ========================================================================
// Pop cursor position from the stack defined in window context.
no_menu::vec2 no_menu::utils::misc::pop_cursor_pos() {
    const vec2 pos = window_ctx.cursor_pos.top();
    window_ctx.cursor_pos.pop();
    return pos;
}

// ========================================================================
// Pop font from the stack defined in window context.
void no_menu::utils::misc::push_font(const unsigned long font) {
    window_ctx.fonts.push(font);
}

// ========================================================================
// Push font to the stack defined in window context.
unsigned long no_menu::utils::misc::pop_font() {
    const unsigned long font = window_ctx.fonts.top();
    window_ctx.fonts.pop();
    return font;
}
// ========================================================================
