# Skateboarding Simulator

This is a **skateboarding game prototype** built in **Unreal Engine 5.3** using **C++ **

## Features

- **Skateboard Movement Mechanics**

  - Acceleration (**E**), Deceleration (**Q**), and Jump (**Spacebar**)
  - **Basic skateboard rotation** dynamically adjusting to ground slope
  - Uses **Mixamo animations** for jumping and movement

- **Game Mode & Scoring System**

  - Implements `IScoreInterface` to manage player score and game-over detection
  - `AObstacle` triggers scoring when successfully bypassed
  - **+10 points** per obstacle cleared

- **User Interface (HUD)**

  - Displays **real-time score updates** using `USkaterHUDWidget`
  - Shows **positive reinforcement messages** and **reset prompts** for engagement

- **Collision & Gameplay Interactions**

  - `AObstacle` handles both **score updates** and **game-over detection** using two collision zones
  - `ISkaterInterface` ensures valid skaters trigger scoring

## Code Structure & Design

- **Modular C++ implementation** with minimal coupling
- `ASkaterHUD : public AHUD, public ISkaterHUDInterface` - **Decoupled UI management**
- `ASkaterCharacter : public ACharacter, public ISkaterInterface, public ISkaterCharacterInterface`
  - `ISkaterInterface`: Defines **win conditions** and interactions
  - `ISkaterCharacterInterface`: **Handles animation communication** without direct references

## Future Improvements

There are many ways this project could be expanded and improved. Given more time, I would:

- **Enhance skateboard physics** for better realism
- **Retarget Mixamo skeleton** to improve foot placement, preventing back leg clipping
- **Transition movement to a physics-based system** rather than using direct mathematical calculations
- **Expand gameplay** by adding trick animations and interactive obstacles

## Development Time

The total time spent on this project was **14 hours**. While I didn’t track time for each specific task, I focused on making steady progress and refining the implementation along the way.

## How to Run

1. Clone the repository from GitHub.
2. Open the project in **Unreal Engine 5.3**.
3. Compile and run the game to test skateboard movement and mechanics.

## Contact

I appreciate the opportunity to work on this task and welcome any feedback.

**Ahmed Farrag**\
[Website](https://ahmedfarrag.me/)

