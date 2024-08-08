/*
 * Copyright (C) 2023-2024 TheRealOne78 <bajcsielias78@gmail.com>
 *
 * This file is part of the OAWP project
 *
 * OAWP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OAWP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OAWP. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEBUG
# define DEBUG false
#endif

/* Image processing and displaying */
#include <Imlib2.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

/* Config */
#include <libconfig.h>

/* Basic programming */
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

/* OAWP created headers */
#include "fancy-text.h"
#include "info.h"
#include "dir-handler.h"
#include "oawp.h"
#include "arg.h"
#include "log.h"
#include "pixmanip.h"
#include "sig-handlers.h"

/* == DEFAULT PARAMETERS == */

/* Basic config/argument variables */
//char params_config.im_dir_path[PATH_MAX];                  /* path to images directory, from configuration file */ //TODO remove this
//char params_args.im_dir_path[PATH_MAX];                    /* path to images directory, from user argument (not -c) */ //TODO remove this

char conf_path[PATH_MAX];          /* path to configuration file */
unsigned img_count;                         /* number of images */
char **img_path;                            /* pointers to paths of images, from configuration file */ //TODO: replace with linked list
double frame_time = DEFAULT_FRAME_TIME;     /* time between frames */ /* The time set is the default one */

/*
 * TODO
 * The following commented variables will be replaced by the new params_t type.
 * The commented variables are deprecated and will be removed as soon as
 * everything was fully implemented
 */

//bool isArgConf = false;                    /* If true, the configuration file from argument will be used */ //replaced with params.hasConf

//bool hasArgTime = false;                   /* If true, time from user argument will be used */
//bool params_args.has_im_dir_path = false;                    /* If true, the directory will be used from user argument */

//TODO:DEPRECATED: deprecate this
bool using_static_wallpaper = false;         /* If true, OAWP will run only once to set a static wallpaper */
//bool hasArgStaticWallpaper = false;        /* If true, it will be used the Static Wallpaper from user argument */

//bool params_args.has_fit_opt = false;                    /* If true, the fit option from user argument will be used - Order 0 */
//bool hasConfFit = false;                   /* If true, the fit option from configuration file will be used - Order 1 */
//char defaultFitOpt[] = DEFAULT_FIT_OPTION; /* Default Fit Option - Order 2 */ //TODO: use enums
//char *fitOpt;                              /* The final fit option */
//

int main(int argc, char *argv[]) {
    /* Set up a handler for the SIGTERM and SIGINT signal */
    signal(SIGTERM, term_handler);
    signal(SIGINT,  term_handler);

    /* Configure log file */
#ifdef _WIN32
    {
        // NOTE: Scope for tmp_path
        char tmp_path[PATH_MAX];

        format_path(tmp_path, "%localappdata%\\Temp\\oawp-logfile.txt");
        FILE *log_file = fopen(tmp_path, "a");
        // NOTE: tmp_path goes out of scope
    }
#else
    FILE *log_file = fopen("/tmp/oawp-logfile.txt", "a");
#endif
    if (!log_file) {
        fprintf(stderr, "Failed to open log file!\n");
        exit(EXIT_FAILURE);
    }

    /* Configure logger level */
    if(DEBUG) {
        log_set_level(LOG_INFO);
        log_add_fp(log_file, LOG_INFO);
    } else {
        log_set_level(LOG_DEBUG);
        log_add_fp(log_file, LOG_DEBUG);
    }

    // TODO: Free these
    ImPaths* im_paths_conf = (ImPaths*)malloc(sizeof(ImPaths));
    ImPaths* im_paths_arg  = (ImPaths*)malloc(sizeof(ImPaths));
    im_paths_init(im_paths_arg);
    im_paths_init(im_paths_conf);

    params_t params_args;
    params_t params_config;

    if (arg_get_opt(argc, (const char**)argv, &params_args)) {
        log_fatal("Something went wrong with argument parsing.");
        exit(EXIT_FAILURE);
    }

    /* print OAWP color logo in ASCII art. */
    puts_logo_auto();

    config_t cfg;
    const char *tmp_cfg_str;

    config_init(&cfg);

    /* Format the path from relative to absolute */
    {
        // NOTE: Scope for tmp_path
        char tmp_path[PATH_MAX];
        format_path(tmp_path, DEFAULT_CONFIG_FILE_PATH);
        if(! params_args.has_conf_path)
            strcpy(conf_path, tmp_path);
        // NOTE: tmp_path goes out of scope
    }

    log_debug("configuration file path: \"%s\"", conf_path);

    // Read the file. If there is an error, report it and exit.
    if(! config_read_file(&cfg, conf_path)) {
        log_fatal("%s:%d - %s", config_error_file(&cfg),
                  config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    /*
     * To make it easy and on-demand, first read a parameter, add it, and then
     * check/manipulate opts/config parameters.
     *
     * Opts always have a higher priority than configuration files.
     *
     * For clarity, reading is noted as [ START/END_READ ] and manipulation is
     * noted with [ START/END_MANIP ], with a NOTE: sequence at the begining to
     * make it more clear for some Emacs users.
     */

    // NOTE: [ START_READ ]
    if(config_lookup_bool(&cfg, "debug", &params_config.debug)) {
        params_config.has_debug = true;
        params_config.debug = params_config.debug;
    } else {
        params_config.has_debug = false;
        params_config.debug = false;
        log_error("No 'debug' setting in configuration file.");
    }
    // NOTE: [ STOP_READ ]

    // NOTE: [ START_MANIP ]
    if(params_args.debug || params_config.debug) {
        log_set_level(LOG_DEBUG);
        log_debug("Enabled debug");
    }
    // NOTE: [ STOP_MANIP ]


    // TODO
    // NOTE: [ START_READ ]
    if(! params_args.has_static_wallpaper &&
       config_lookup_string(&cfg, "static-wallpaper", &tmp_cfg_str)) {
        strcpy(params_config.static_wallpaper, tmp_cfg_str);
        params_config.has_static_wallpaper = true;
    }
    // NOTE: [ STOP_READ ]

    // NOTE: [ START_MANIP ]
    {
        // NOTE: Scope for tmp_path, tmp_path2
        char tmp_path[PATH_MAX];
        char* p_tmp_path2;

        if(params_args.has_static_wallpaper)
            p_tmp_path2 = params_args.static_wallpaper;
        else if(params_config.has_static_wallpaper)
            p_tmp_path2 = params_config.static_wallpaper;
        else
            p_tmp_path2 = NULL;

        if(p_tmp_path2 != NULL) {
            format_path(tmp_path, p_tmp_path2);

            // TODO Check access in Windows
            if(access(tmp_path, F_OK) != 0) {
                log_error("%s from 'static-wallpaper' does not exist.", tmp_path);
                exit(EXIT_FAILURE);
            }

            if(access(tmp_path, R_OK) != 0) {
                log_error("%s from 'static-wallpaper' cannot be read. Please check the file permissions.", tmp_path);
                exit(EXIT_FAILURE);
            }

            // TODO: Don't rely on this
            //img_path = (char**)malloc(1 * sizeof(char*));
            //img_path[0] = (char*)malloc(PATH_MAX * sizeof(char));
            //strcpy(img_path[0], params_config.static_wallpaper);
            //img_count++;
        }
        // NOTE: tmp_path goes out of scope
    }


    // NOTE: [ STOP_MANIP ]


    // TODO
    // NOTE: [ START_READ ]
    if(params_args.has_im_dir_path && !using_static_wallpaper) {
        im_paths_get(params_args.im_dir_path, im_paths_arg);
    }
    else if(!params_args.has_im_dir_path &&
            config_lookup_string(&cfg, "path", &tmp_cfg_str) &&
            !using_static_wallpaper) {
        strcpy(params_config.im_dir_path, tmp_cfg_str);

        im_paths_get(params_args.im_dir_path, im_paths_arg);
        //getImgCount(params_config.im_dir_path);   //TODO: Do that somewhere else
        //im_paths_get(params_config.im_dir_path);  //TODO: Do that somewhere else
    }
    // NOTE: [ STOP_READ ]


    // NOTE: [ START_MANIP ]
    else if(! using_static_wallpaper) {
        log_error("No 'path' setting in configuration file.");
        exit(EXIT_FAILURE);
    }


    // TODO
    // NOTE: [ START_READ ]
    if(! params_args.has_frame_time &&
       config_lookup_float(&cfg, "time", &params_config.frame_time) &&
       !using_static_wallpaper) {
        if(params_config.frame_time < MIN_FRAME_TIME) {
            log_error("Time cannot be less than %lf.", MIN_FRAME_TIME);
            exit(EXIT_FAILURE);
        }

        log_debug("frame_time: %lf", frame_time);
        params_config.has_frame_time = true;
    }
    // NOTE: [ STOP_READ ]
   

    // NOTE: [ START_MANIP ]
    else if(! using_static_wallpaper) {
        params_config.has_frame_time = false;
        log_warn("No 'time' setting in configuration file. Using default '0.07' seconds as time parameter.");
    }


    // TODO
    // NOTE: [ START_READ ]
    if(config_lookup_string(&cfg, "fit", &tmp_cfg_str) &&
       !params_args.has_fit_opt &&
       !using_static_wallpaper) {
        params_config.fit_opt = fit_atoe(tmp_cfg_str);
        params_config.has_fit_opt = true;
    }
    // NOTE: [ STOP_READ ]

    // NOTE: [ START_MANIP ]
    else
        params_config.has_fit_opt = false;
    // NOTE: [ STOP_MANIP ]

    // Free cfg
    config_destroy(&cfg);


   
    /* NOTE: TODO: DEPRECATED:
     * From here on, everything in the main function should be deprecated, as
     * we're nolonger be using ImLib after restructuring's done.
     */

    log_debug("Loading images ...");

    Imlib_Image images[img_count];
    if(!using_static_wallpaper) {
        for(int tmp = 0; tmp < img_count; tmp++) {
            images[tmp] = imlib_load_image(img_path[tmp]);
            log_debug("Imlib loaded %s", img_path[tmp]);
        }
    } else {
        images[0] = imlib_load_image((img_path)[0]);

        log_debug("Imlib loaded %s", (img_path)[0]);
    }
    // TODO: free im_paths
    //freeUsingPath();

    // Loading the monitors, counting them and getting the resolution

    log_debug("Loading monitors ...");

    Display *display = XOpenDisplay(NULL);
    if (!display) {
        log_error("Could not open XDisplay");
        exit(42);
    }

    const int screen_count = XScreenCount(display);

    log_debug("Found %d screens", screen_count);

    Monitor *monitors = malloc(sizeof(Monitor) * screen_count);
    for(int current_screen = 0; current_screen < screen_count;
        ++current_screen)
    {

        log_debug("Running screen %d", current_screen);

        const int width  = DisplayWidth(display, current_screen);
        const int height = DisplayHeight(display, current_screen);
        const int depth  = DefaultDepth(display, current_screen);
        Visual *vis      = DefaultVisual(display, current_screen);
        const int cm     = DefaultColormap(display, current_screen);

        log_debug("Screen %d: width: %d, height: %d, depth: %d",
                  current_screen, width, height, depth);

        Window root = RootWindow(display, current_screen);
        Pixmap pixmap = XCreatePixmap(display, root, width, height, depth);

        monitors[current_screen].width          = width;
        monitors[current_screen].height         = height;
        monitors[current_screen].root           = root;
        monitors[current_screen].pixmap         = pixmap;
        monitors[current_screen].render_context = imlib_context_new();
        imlib_context_push(monitors[current_screen].render_context);
        imlib_context_set_display(display);
        imlib_context_set_visual(vis);
        imlib_context_set_colormap(cm);
        imlib_context_set_drawable(pixmap);
        imlib_context_set_color_range(imlib_create_color_range());
        imlib_context_pop();
    }

    log_debug("Loaded %d screens.", screen_count);

    // Rendering the images on the screens found at the choosen time interval, forever

    log_debug("Starting render loop ...");

    struct timespec timeout;
    double time_nsec_raw = frame_time;
    if(time_nsec_raw > 1.0)
        time_nsec_raw -= floor(time_nsec_raw);
    timeout.tv_sec  = floor(frame_time);
    timeout.tv_nsec = time_nsec_raw * 1e9;

    while(img_count) {
        for(int cycle = 0; cycle < img_count; ++cycle) {

            Imlib_Image current = images[cycle % img_count];

            for(int monitor = 0; monitor < screen_count; ++monitor) {
                Monitor *c_monitor = &monitors[monitor];
                imlib_context_push(c_monitor->render_context);
                imlib_context_set_dither(1);
                imlib_context_set_blend(1);
                imlib_context_set_image(current);

                imlib_render_image_on_drawable(0, 0);

                set_root_atoms(display, c_monitor);
                XKillClient(display, AllTemporary);
                XSetCloseDownMode(display, RetainTemporary);
                XSetWindowBackgroundPixmap(display, c_monitor->root, c_monitor->pixmap);
                XClearWindow(display, c_monitor->root);
                XFlush(display);
                XSync(display, False);
                imlib_context_pop();
            }

            if(using_static_wallpaper) {
                log_debug("Using static wallpaper detected, exiting ...");
                exit(EXIT_SUCCESS);
            }

            nanosleep(&timeout, NULL);
        }
    }
}


/// This function should clear and load the images to X11 screen
void set_root_atoms(Display *restrict display, Monitor *restrict monitor) {

    Atom atom_root, atom_eroot, type;
    unsigned char *data_root, *data_eroot;
    int format;
    unsigned long length, after;

    atom_root  = XInternAtom(display, "_XROOTMAP_ID"    , True);
    atom_eroot = XInternAtom(display, "ESETROOT_PMAP_ID", True);

    // doing this to clean up after old background
    if (atom_root != None && atom_eroot != None) {
        XGetWindowProperty(display, monitor->root, atom_root, 0L, 1L, False,
                           AnyPropertyType, &type, &format, &length, &after,
                           &data_root);

        if (type == XA_PIXMAP) {
            XGetWindowProperty(display, monitor->root, atom_eroot, 0L, 1L, False,
                               AnyPropertyType, &type, &format, &length, &after,
                               &data_eroot);

            if (data_root && data_eroot && type == XA_PIXMAP &&
                *((Pixmap *)data_root) == *((Pixmap *)data_eroot))
                XKillClient(display, *((Pixmap *)data_root));
        }
    }

    atom_root  = XInternAtom(display, "_XROOTPMAP_ID"   , False);
    atom_eroot = XInternAtom(display, "ESETROOT_PMAP_ID", False);

    // setting new background atoms
    XChangeProperty(display, monitor->root, atom_root, XA_PIXMAP, 32,
                    PropModeReplace, (unsigned char *)&monitor->pixmap, 1);
    XChangeProperty(display, monitor->root, atom_eroot, XA_PIXMAP, 32,
                    PropModeReplace, (unsigned char *)&monitor->pixmap, 1);
}
