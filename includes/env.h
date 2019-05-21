#ifndef ENV_H
#define ENV_H

/* Game and window related constants */
#define g 0.05
#define boundary_width 50
#define win_height 500
#define win_width 1000

/* Game over menu */
#define go_menu_x 700
#define go_menu_y 350
#define go_menu_width 200
#define go_menu_height 150

/* Obstacles related constants */
#define obs_width 55
#define opening_width 125
#define obs_gap 300
#define obs_no 10000
#define obs_offset 30
#define obs_speed 4
#define obs_start_pos 500

/* Bird related constants */
#define bird_x 250
#define bird_y 200
#define bird_size 10
#define jump_speed 0.1
#define jump_acceleration 0.26
#define jump_acc_time 1

/* Neural network related constants */
#define time_bw_jumps 10
#define population_size 20
#define no_of_layers 3
#define mut_rate 5
//#define learn_rate 0.001
#define disc_factor 0.6

/* misc constants */
#define SPACEBAR 32

