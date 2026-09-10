#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

void setNonCanonicalMode(int enable) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (enable) {
        tty.c_lflag &= ~(ICANON | ECHO);
    } else {
        tty.c_lflag |= (ICANON | ECHO);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int main() {
    int ch;

    setNonCanonicalMode(1);

    printf("Press any key to see its value. Press 'q' to quit.\n");

    while (1) {
        if (kbhit()) {
            ch = getchar();
            if (ch == 27) { // Escape sequence
                if (getchar() == 91) { // '['
                    switch (getchar()) {
                        case 'A':
                            printf("Up arrow key pressed\n");
                            break;
                        case 'B':
                            printf("Down arrow key pressed\n");
                            break;
                        case 'C':
                            printf("Right arrow key pressed\n");
                            break;
                        case 'D':
                            printf("Left arrow key pressed\n");
                            break;
                    }
                }
            } else {
                printf("Key pressed: %c (ASCII: %d)\n", ch, ch);
                if (ch == 'q') {
                    break;
                }
            }
        }
    }

    setNonCanonicalMode(0);
    return 0;
}