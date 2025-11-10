# 3710FinalSTM32L476RGCNC
A final project for USU's ECE 3710 class, using an STM32L476RGTx as a CNC pen plotter controller.


Uses hardware derived from https://github.com/IVProjects/Engineering_Projects/tree/main/ProjectFiles/High-Speed%20Pen%20Plotter.

Uses a GRBL CNC shield for an Arduino, which plugs onto the headers on the Nucleo board.

CMake is not used for the actual build flow (in Keil) for the microcontroller, but may be useful to test compilation or functions on a PC.

Sub-items:

Hardware (mechanical)

Hardware (electrical)

Software:

  Interrupt based UART receive/tx
  
  G-code throttling
  
  G-code parser
  
  Acceleration handler
  
  Step generator
  
  Endstop/homing behavior
  
  Pen lift/unlift functions
