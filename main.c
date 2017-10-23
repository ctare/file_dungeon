#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define PLAYER_CHIP '9'
#define DEBUG 0
#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3
#define HERE 4

struct MessageBar {
  int point;
  double progress;
};

struct Position {
  int x, y;
};

struct Speed {
  int x, y;
};

void* input_loop(void* args) {
  int* ptr = (int*) args;
  while(1) {
    *ptr = getchar();
  }
  return 0;
}

void create_message_bar(char* message, struct MessageBar* bar, unsigned long length) {
  char sub_message[length];
  sprintf(sub_message, "point %d %.1f%% |", bar -> point, bar -> progress);
  unsigned long progress_bar_len = length - strlen(sub_message) - 1;

  char progress[progress_bar_len + 1];
  progress[progress_bar_len] = '\0';
  memset(progress, ' ', progress_bar_len);
  int percentage = (int)(progress_bar_len * (bar -> progress / 100));
  memset(progress, '#', percentage);
  sprintf(message, "%s%s|", sub_message, progress);
}

void print_map(int map_lines, int map_cols, char map_2d[map_lines][map_cols + 1],
    struct winsize* terminal, struct Position* position, struct Position* player) {

  int mini_ws_row = terminal -> ws_row - (2 + DEBUG);
  char map_line[terminal -> ws_col + 1];
  map_line[terminal -> ws_col] = '\0';

  int print_y = map_lines - position -> y - mini_ws_row;
  for(int i = mini_ws_row; i > 0; i--){
    if(print_y < 0) {
      memset(map_line, ' ', terminal -> ws_col);
      if(player -> y == print_y) {
        map_line[player -> x - position -> x] = PLAYER_CHIP;
      }
      printf("%s\n", map_line);
    } else {
      for(int j = 0; j < terminal -> ws_col; j++){
        int print_x = position -> x + j;
        if(print_x < map_cols) {
          char chip = map_2d[print_y][print_x];
          if(player -> x == print_x && player -> y == print_y) {
            printf("%c", PLAYER_CHIP);
          } else switch(chip) {
            case '*':
              printf("[38;5;220m%c[0m", chip);
              break;
            default:
              printf("%c", chip);
              break;
          }
        } else {
          printf(" ");
        }
      }
      printf("\n");
    }

    print_y++;
  }
}

int load_file(int* var_lines, int* var_cols, struct Position* player,
    struct stat* file_status, char file_data[file_status -> st_size + 1], char* data_name) {
  FILE* map_data;
  if((map_data = fopen(data_name, "r")) == NULL) {
    return EXIT_FAILURE;
  }

  int max_count = 0;
  int lines = 0;
  int count = 0;
  for(int i = 0; i < file_status -> st_size; i++) {
    if((file_data[i] = fgetc(map_data)) == '\n') {
      if(max_count < count) max_count = count;
      count = 0;
      lines++;
    } else {
      if(file_data[i] == PLAYER_CHIP) {
        player -> x = count;
        player -> y = lines;
        file_data[i] = ' ';
      }
      count++;
    }
  }

  *var_lines = lines;
  *var_cols = max_count;

  fclose(map_data);
  return EXIT_SUCCESS;
}

void load_map(int lines, int cols, struct stat* file_status, char map_2d[lines][cols + 1], char file_data[file_status -> st_size]) {
  int now_x = 0, now_y = 0;
  for(int i = 0; i < file_status -> st_size; i++) {
    if(now_x < cols && file_data[i] == '\n') {
      map_2d[now_y][now_x] = ' ';
      i--;
    } else if(file_data[i] == '\n') {
      map_2d[now_y][now_x] = '\0';
      now_x = -1;
      now_y++;
    } else {
      map_2d[now_y][now_x] = file_data[i];
    }
    now_x++;
  }
}

int load_file_status(struct stat* file_status, char* data_name) {
  return stat(data_name, file_status) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

int load_terminal(struct winsize* terminal) {
  return ioctl(STDOUT_FILENO, TIOCGWINSZ, terminal) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

int error(char* message) {
  printf("error: %s\n", message);
  return EXIT_FAILURE;
}

int file_load_error(char* file_name) {
  char error_message[100];
  sprintf(error_message, "Failed to load file status. Do the file(%s) exists?", file_name);
  return error(error_message);
}

char get_block(int lines, int cols, char map_2d[lines][cols + 1], struct Position* player, int direction) {
  switch(direction) {
    case TOP:
      return map_2d[player -> y - 1][player -> x];
    case BOTTOM:
      return map_2d[player -> y + 1][player -> x];
    case LEFT:
      return map_2d[player -> y][player -> x - 1];
    case RIGHT:
      return map_2d[player -> y][player -> x + 1];
    case HERE:
      return map_2d[player -> y][player -> x];
    default:
      return 0;
  }
}

int main(int argc, char* argv[]) {
  struct winsize terminal;
  struct stat file_status;
  struct Position player;
  struct Position position;
  struct MessageBar bar;
  struct termios now_settings;
  int lines, cols;

  char* file_data;
  if(argc > 1){
    if(load_terminal(&terminal) == EXIT_FAILURE) {
      return error("Failed to load terminal size.");
    }

    if(load_file_status(&file_status, argv[1]) == EXIT_FAILURE) {
      return file_load_error(argv[1]);
    }

    file_data = malloc(file_status.st_size + 1);
    if(load_file(&lines, &cols, &player, &file_status, file_data, argv[1])) {
      return file_load_error(argv[1]);
    }
  } else {
    return error("Please file name.");
  }

  char map_2d[lines][cols + 1];
  load_map(lines, cols, &file_status, map_2d, file_data);
  free(file_data);

  char message[terminal.ws_col];

  tcgetattr(STDIN_FILENO, &now_settings);
  struct termios new_settings = now_settings;
  new_settings.c_lflag &= ~ICANON;
  new_settings.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

  int input = 0;
  pthread_t input_thread;
  pthread_create(&input_thread, NULL, input_loop, &input);

  struct Speed player_speed;
  while(1) {
    // draw
    bar.progress = position.x / (double)cols * 100;

    create_message_bar(message, &bar, sizeof message);
    printf("[0;0H%s\n", message);

    if(get_block(lines, cols, map_2d, &player, HERE) == '*') {
      map_2d[player.y][player.x] = ' ';
      bar.point++;
    }
    print_map(lines, cols, map_2d, &terminal, &position, &player);

    // controll
    switch(input) {
      case ' ':
        if(get_block(lines, cols, map_2d, &player, BOTTOM) == '#') {
          player_speed.y = 3;
        }
        input = 0;
        break;
      default: break;
    }

    // move
    position.x += 1;
    if(get_block(lines, cols, map_2d, &player, RIGHT) != '#') {
      player.x += 1;
    }

    if(player_speed.y > 0) {
      player_speed.y--;
      player.y--;
    } else if(player_speed.y == 0) {
      player_speed.y--;
    } else {
      if(map_2d[player.y + 1][player.x] != '#') {
        player.y += 1;
      }
    }

    if(player.y >= lines) {
      player.y--;
      position.y = 0;
    }

    // is end?
    if(player.x - position.x < 0) {
      puts("game over");
      goto END;
    }

    if(position.x > cols - 1) {
      break;
    }

    // flush and sleep
    fflush(stdout);
    usleep(80000);
  }

  // after game clear
  bar.progress = 100;
  create_message_bar(message, &bar, sizeof message);
  printf("[0;0H%s\n", message);
  printf("game clear  \n");
  /* while(1) { */
  /*   switch(input) { */
  /*     case '\n': */
  /*       goto END; */
  /*     default: */
  /*       break; */
  /*   } */
  /* } */

END:
  tcsetattr(STDIN_FILENO, TCSANOW, &now_settings);
  return EXIT_SUCCESS;
}
