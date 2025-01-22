#include <libGUI.hpp>
#include <libKeyboard.hpp>
#include <libTimer.hpp>
#include <printf.hpp>
#include <PROGRAM_PONG.hpp>
#include <utils.hpp>

#define PADDLE_SPEED 10
#define BALL_SPEED_RATIO 5
#define PADDLE_HEIGHT 20
#define PADDLE_PADDING 15
#define PADDLE_X 15
#define BALL_SIZE 4

namespace PONG {

int leftPos; //defined later
int rightPos; //defined later
int ballX; //defined later
int ballY; //defined later
int ballDX = 1;
int ballDY = 1;
int timer = 0;
int leftScore, rightScore = 0;
bool lastScored = false;
GuiTextRenderer* renderer = nullptr;
string* score = nullptr;

#define printScore()                                                     \
    renderer->putString(*score, 5, 0);                                   \
    renderer->putString(intToStr(leftScore, 10), 45, 0);                 \
    renderer->putString(*score, renderer->screenWidth - PADDLE_PADDING - 40, 0); \
    renderer->putString(intToStr(rightScore, 10), renderer->screenWidth - PADDLE_PADDING, 0);

bool loop();

void main() {
    GuiTextRenderer temp;
    renderer = &temp;
    renderer->setDrawColor(WHITE);
    renderer->setTextFont(&Uni2Terminus12x6psf);
    renderer->clearBox();
    updateStdout(*renderer);

    leftPos = (renderer->screenHeight / 2) + (PADDLE_HEIGHT / 2);
    rightPos = (renderer->screenHeight / 2) - (PADDLE_HEIGHT / 2);
    ballX = renderer->screenWidth / 2;
    ballY = renderer->screenHeight / 2;

    string tmp = "Score: ";
    score = &tmp;

    // print initial score
    printScore();

    while (true) {
        sleep(5);

        if (loop()) {
            renderer->clearBox();
            printf("Game Over\n");
            printf("Press Escape to exit\n");
            printf("Press any other button to play again\n");
            leftPos = (renderer->screenHeight / 2) + (PADDLE_HEIGHT / 2);
            rightPos = (renderer->screenHeight / 2) - (PADDLE_HEIGHT / 2);
            ballX = renderer->screenWidth / 2;
            ballY = renderer->screenHeight / 2;
            timer = 0;

            sleep(500);
            KB::waitForKeyboard();

            while (KB::getKeyBufferIndex() > 0) {
                char c = KB::popKeyBuffer();
                if (c == Escape_ASCII) {
                    printf("Program Exiting\n");
                    return;
                }
            }
            renderer->clearBox();

            // update score
            printScore();
        }
    }
}

bool loop() {
    // Draw everything as black
    renderer->putRect(ballX, ballY, 4, 4, BLACK);
    renderer->putLine(PADDLE_PADDING, leftPos, PADDLE_HEIGHT, true, BLACK);
    renderer->putLine(renderer->screenWidth - PADDLE_PADDING, rightPos, PADDLE_HEIGHT, true, BLACK);

    if (KB::getKeyBufferIndex() > 0) {
        char character = KB::popKeyBuffer();

        switch (character) {
            case 'w':
                leftPos -= PADDLE_SPEED;
                if (leftPos < 0) {
                    leftPos = 0;
                }
                break;
            case 's':
                leftPos += PADDLE_SPEED;
                if (leftPos + PADDLE_HEIGHT > renderer->screenHeight) {
                    leftPos = renderer->screenHeight - PADDLE_HEIGHT;
                }
                break;
            case 'i':
                rightPos -= PADDLE_SPEED;
                if (rightPos < 0) {
                    rightPos = 0;
                }
                break;
            case 'k':
                rightPos += PADDLE_SPEED;
                if (rightPos + PADDLE_HEIGHT > renderer->screenHeight) {
                    rightPos = renderer->screenHeight - PADDLE_HEIGHT;
                }
                break;
            default:
                break;
        }
    }

    if (ballY <= 0 || ballY + BALL_SIZE >= renderer->screenHeight) {
        ballDY *= -1;
    }

#define BALL_L_X_Intersect (ballX <= PADDLE_PADDING && ballX + BALL_SIZE >= PADDLE_PADDING)
#define BALL_R_X_INTERSECT \
    (ballX <= renderer->screenWidth - PADDLE_PADDING && ballX + BALL_SIZE >= renderer->screenWidth - PADDLE_PADDING)
#define BALL_L_Y_INTERSECT (ballY <= leftPos + PADDLE_HEIGHT && ballY + BALL_SIZE >= leftPos)
#define BALL_R_Y_INTERSECT (ballY <= rightPos + PADDLE_HEIGHT && ballY + BALL_SIZE >= rightPos)

    if ((BALL_L_X_Intersect && BALL_L_Y_INTERSECT) || (BALL_R_X_INTERSECT && BALL_R_Y_INTERSECT)) {
        ballDX *= -1;
    }

    if (ballX <= 0) {
        rightScore++;
        return true;
    } else if (ballX >= renderer->screenWidth) {
        leftScore++;
        return true;
    }

    timer++;
    if (timer % BALL_SPEED_RATIO == 0) {
        ballY += ballDY;
    }
    ballX += ballDX;

    // reprint score incase ball overlaps it
    // (reprint every 200 frames to avoid lag)
    if (timer % 200 == 0) {
        printScore();
    }

    // ClearScreenGUI();

    renderer->putRect(ballX, ballY, 4, 4, RED);
    renderer->putLine(PADDLE_PADDING, leftPos, PADDLE_HEIGHT, true, MAGENTA);
    renderer->putLine(renderer->screenWidth - PADDLE_PADDING, rightPos, PADDLE_HEIGHT, true, MAGENTA);

    return false;
}

}  // namespace PONG
