# Snake Game in C with OpenGL

This is a simple implementation of the classic Snake game written in C using OpenGL. The game is designed to work on macOS with Intel processors and Linux. It uses the GLUT library to handle windowing and input.

## Features
- Simple and responsive gameplay.
- Snake grows each time it eats food.
- Game over condition when the snake hits itself.
- The game is rendered using OpenGL, providing a smooth experience.

## Prerequisites

To run the game, you need to have OpenGL and GLUT installed on your machine. The following instructions show how to install the required libraries on macOS and Linux.

### macOS (Intel processors)
1. Install `freeglut` via Homebrew:
   ```bash
   brew install freeglut
   ```
   
2. Once you have the necessary libraries installed, you can compile the game using gcc:
 ```bash
   gcc -o snake snake.c -lGL -lGLU -lglut
  ```

3. After compiling the game, run the following command to start the game:
 ```bash
   ./snake
```
## Controls
Up Arrow: Move the snake up.
Down Arrow: Move the snake down.
Left Arrow: Move the snake left.
Right Arrow: Move the snake right.

## Game Over
The game ends when the snake runs into itself. After the game over, you can restart by running the game again.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments
This game was created using OpenGL and GLUT libraries.
Special thanks to the open-source community for their contributions to these libraries.

