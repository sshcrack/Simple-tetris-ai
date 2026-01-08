# Simple Tetris AI

A Tetris AI that uses a genetic algorithm to evolve neural network parameters for optimal gameplay.

## Overview

This project implements an AI that learns to play Tetris through evolutionary computation. The AI evaluates board states using a neural network with hand-crafted features and learns optimal weights through a genetic algorithm.

## Features

- **Neural Network**: Single hidden layer (3 neurons, ReLU activation)
- **Genetic Algorithm**: Population of 300 agents with fitness-weighted crossover
- **Multi-threaded Training**: Utilizes all available CPU cores for parallel agent evaluation
- **2-piece Lookahead**: Evaluates future piece positions for better decisions
- **Real-time Visualization**: Watch the trained AI play in the terminal

## Neural Network Architecture

```
Input Layer (4 features):
  - Aggregate Height: Sum of all column heights
  - Completed Lines: Number of fully filled rows
  - Holes: Empty spaces covered by blocks
  - Bumpiness: Sum of adjacent column height differences

Hidden Layer (3 neurons, ReLU activation)

Output Layer (1 value): Board quality score
```

Total parameters: 20 (16 weights + 4 biases)

## Genetic Algorithm Parameters

| Parameter | Value |
|-----------|-------|
| Population size | 300 agents |
| Games per evaluation | 3 games |
| Max pieces per game | 50,000 |
| Elitism rate | 30% replaced per generation |
| Selection pool | 10% of population |
| Crossover method | Fitness-weighted average |

## Building

Requirements:
- Meson build system
- C++ compiler with C++20 support
- No external dependencies (standard library only)

```bash
# Configure the build
meson setup build

# Build the project
ninja -C build
```

This creates two executables:
- `build/train` - Training program
- `build/tetris` - Visual gameplay viewer

## Usage

### Training the AI

```bash
cd build
./train
```

The training process:
- Creates a population of 300 random agents
- Evaluates each agent by playing 3 games
- Displays generation and agent progress
- Shows average and highest fitness per generation
- Automatically saves best parameters to `data/bestParams.txt`

Training continues indefinitely until manually stopped.

### Watching the AI Play

```bash
cd build
./tetris
```

This loads the best trained parameters and displays the game in real-time with:
- Cleared lines count
- Current record
- Best move calculation (direction and rotation)
- Visual game board (20×10 grid)

## Project Structure

```
Simple-tetris-ai/
├── include/
│   ├── ml/                # Machine learning headers
│   │   ├── brain.h        # AI decision-making
│   │   ├── genetic.h      # Genetic algorithm
│   │   ├── neural-net.h   # Neural network structure
│   │   ├── population.h   # Population management
│   │   └── utils.h        # Utility functions
│   └── tetris/            # Tetris game headers
│       ├── constants.h    # Piece shapes
│       ├── grid.h         # Game grid
│       └── piece.h        # Tetromino pieces
├── src/
│   ├── ml/                # ML implementation
│   ├── tetris/            # Tetris implementation
│   ├── terminal.cpp       # Game viewer
│   └── train.cpp          # Training entry point
├── data/
│   ├── bestParams.txt     # Best neural network
│   └── record.txt         # High score record
└── meson.build            # Build configuration
```

## Algorithm Details

### Crossover
Offspring parameters are calculated as fitness-weighted averages:
```
offspring_param = (fitness1 × parent1_param + fitness2 × parent2_param)
                  / (fitness1 + fitness2)
```

### Selection
For each offspring:
1. Randomly select 10% of the population
2. Choose the top 2 performers from this selection
3. Create offspring through crossover
4. Replace the worst-performing agents

### Evaluation
- Each agent plays 3 complete games
- Fitness is the average score across all games
- Games end on game over or after 50,000 pieces

## License

This project is provided as-is for educational purposes.
