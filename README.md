# 3710FinalSTM32L476RGCNC
A final project for USU's ECE 3710 class, using an STM32L476RGTx as a CNC pen plotter controller.


Uses hardware derived from https://github.com/IVProjects/Engineering_Projects/tree/main/ProjectFiles/High-Speed%20Pen%20Plotter.

Uses a GRBL CNC shield for an Arduino, which plugs onto the headers on the Nucleo board.

CMake is not used for the actual build flow (in Keil) for the microcontroller, but may be useful to test compilation or functions on a PC.
Gcode sending software: G sender, universal gcode sender, pronter face

# Sub-items:

Hardware (mechanical)
Hardware (electrical)

## Software:

  ### Interrupt based UART receive/tx
  gets characters one by one based interrupts
  when the new character in the new line or carriage return char we have a full gcode command
  pass full command it to the gcode parser
  
  ### G-code throttling
  done through acknowledgement system
  
  ### G-code parser
  nested case statement that calls specific functions based on the G command
  make it a separate function called in main that is passed a whole Gcode string
  
  ### Step generator
  parameterize the line or arc in time (systick) then use systick interrupt to drive a step in the direction needed (bresenham algorithm)
  
  ### Endstop/homing behavior
  while not endstop move then delay 
  inputs Gcode homing command G28, and keypad input for manual homing
  
  ### Pen lift/unlift functions
  separate function that runs through all steps needed lift or lower the pen
  this is done internally without using the step buffer and systick to allow it to run independently and be tunable

  ### to be implemented at the end (bells and whistles)
  Keypad interface for jogging and homing
  Acceleration handler 

  # timeline (division of labor)

  1. finish hardware (J)
  2. UART recieving and ack system and testing gcode senders (N)
  3. limited G-code parser with just line, home, and pen lift/unlift (N)
  4. line function in parser (J)
  5. investigate inkscape gcode for pen lifter (J)
  6. test minimally viable project (draw something simple) (J&N)
  7. add bells and whistles (J&N)


  
