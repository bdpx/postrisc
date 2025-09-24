#pragma once 

struct linux_logo_info_type {
       int width;
       int no_periods;
       int preserve_xy;
       int skip_bogomips;
       int offset;
       int plain_ascii;
       int banner_mode;
       int wipe_screen;
       int show_uptime;
       int show_load;
       int narrow_logo;
       int display_logo_only;
       int display_sysinfo_only;
       int display_usertext;
       int custom_format;
       int pretty_output;
       int center_sysinfo;
       char user_text[BUFSIZ];
       char format[BUFSIZ];
};
