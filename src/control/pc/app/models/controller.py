import logging
import pygame
from models.position import Position

# Constants for PS5 controller axes and buttons
AXIS_LEFT_STICK_HORIZONTAL = 0
AXIS_LEFT_STICK_VERTICAL = 1
AXIS_RIGHT_STICK_HORIZONTAL = 2
AXIS_RIGHT_STICK_VERTICAL = 3
AXIS_LEFT_TRIGGER = 4
AXIS_RIGHT_TRIGGER = 5

BUTTON_CROSS = 0
BUTTON_CIRCLE = 1
BUTTON_SQUARE = 2
BUTTON_TRIANGLE = 3
BUTTON_SHARE = 4  # Create button on PS5
BUTTON_PS = 5
BUTTON_OPTIONS = 6
BUTTON_LEFT_STICK_IN = 7
BUTTON_RIGHT_STICK_IN = 8
BUTTON_LEFT_BUMPER = 9
BUTTON_RIGHT_BUMPER = 10
BUTTON_DPAD_UP = 11
BUTTON_DPAD_DOWN = 12
BUTTON_DPAD_LEFT = 13
BUTTON_DPAD_RIGHT = 14
BUTTON_TOUCHPAD_CLICK = 15
# BUTTON_MUTE = 16  # PS5 specific (uncomment if your controller reports this button)


class MyController:
    """
    Custom controller class to handle PS5 DualSense controller inputs.
    
    Supports two navigation modes:
    - "manual": Uses left joystick Y-axis for Position.x and right joystick Y-axis for Position.y
    - "assisted": Uses left joystick X-axis for Position.x and Y-axis for Position.y
    """

    def __init__(self, navigation: str) -> None:
        pygame.init()
        pygame.joystick.init()

        self.position = Position(0, 0)
        self._joystick_connected = False
        self.navigation_mode = navigation
        
        # Track button states for continuous input
        self._button_states = {
            BUTTON_DPAD_UP: False,
            BUTTON_DPAD_DOWN: False,
            BUTTON_DPAD_LEFT: False,
            BUTTON_DPAD_RIGHT: False
        }
        
        # Store previous position to detect changes
        self._previous_x = 0
        self._previous_y = 0
        
        logging.info(f"[MyController] Initialized with navigation mode: {self.navigation_mode}")
        logging.debug(f"[MyController] Available joysticks: {pygame.joystick.get_count()}")

        # Check for joysticks
        if pygame.joystick.get_count() == 0:
            logging.critical("[MyController] No joysticks found.")
            self._joystick_connected = False
            exit(1)
        else:
            try:
                self.joystick = pygame.joystick.Joystick(0)
                self.joystick.init()
                self._joystick_connected = True
                logging.info(f"[MyController] Joystick: '{self.joystick.get_name()}' initialized.")
                logging.info(f"[MyController] Axes: {self.joystick.get_numaxes()}, Buttons: {self.joystick.get_numbuttons()}")
                
            except Exception as e:
                logging.critical(f"[MyController] Failed to initialize joystick: {e}")
                self._joystick_connected = False
                exit(1)
                

    def handle_events(self) -> None:
        """
        Handle events from the PS5 controller.
        """
        # Store previous position before processing events
        old_x, old_y = self.position.x, self.position.y
        
        # Get all events
        events = pygame.event.get()
        if events:
            logging.debug(f"[MyController] Processing {len(events)} events.")

        for event in events:
            if event.type == pygame.JOYAXISMOTION:
                self._handle_axis_motion(event)
            elif event.type == pygame.JOYBUTTONDOWN:
                self._handle_button_press(event)
                # Track button state when pressed
                if event.button in self._button_states:
                    self._button_states[event.button] = True
            elif event.type == pygame.JOYBUTTONUP:
                # Track button state when released
                if event.button in self._button_states:
                    self._button_states[event.button] = False

        # If no events, we can still poll the joystick directly (important for continuous movement)
        if self._joystick_connected:
            try:
                # Only check D-pad in assisted mode
                if self.navigation_mode == "assisted" and self._check_dpad_held():
                    # If D-pad is being used, we don't process analog sticks
                    pass
                # Otherwise process analog sticks normally
                elif self.navigation_mode == "assisted":
                    # Assisted mode: use left stick X/Y for control
                    x_val = self.joystick.get_axis(AXIS_LEFT_STICK_HORIZONTAL)
                    y_val = self.joystick.get_axis(AXIS_LEFT_STICK_VERTICAL)
                    
                    # Debug raw values
                    if abs(x_val) > 0.05 or abs(y_val) > 0.05:
                        logging.debug(f"[MyController] Raw joystick values - X: {x_val:.3f}, Y: {y_val:.3f}")
                    
                    # Apply deadzone
                    deadzone = 0.05
                    if abs(x_val) < deadzone:
                        x_val = 0
                    if abs(y_val) < deadzone:
                        y_val = 0
                    
                    # Update position - INVERT Y AXIS for intuitive movement
                    self.position.x = max(min(x_val, 1.0), -1.0)
                    self.position.y = max(min(-y_val, 1.0), -1.0)  # Negate for correct direction
                    
                else:  # "manual" mode
                    # Manual mode: use left stick Y for X position, right stick Y for Y position
                    left_y_val = self.joystick.get_axis(AXIS_LEFT_STICK_VERTICAL)
                    right_y_val = self.joystick.get_axis(AXIS_RIGHT_STICK_VERTICAL)
                    
                    # Debug raw values
                    if abs(left_y_val) > 0.05 or abs(right_y_val) > 0.05:
                        logging.debug(f"[MyController] Raw joystick values - Left Y: {left_y_val:.3f}, Right Y: {right_y_val:.3f}")
                    
                    # Apply deadzone
                    deadzone = 0.05
                    if abs(left_y_val) < deadzone:
                        left_y_val = 0
                    if abs(right_y_val) < deadzone:
                        right_y_val = 0
                    
                    # Update position - INVERT BOTH for intuitive movement
                    self.position.x = max(min(-left_y_val, 1.0), -1.0)  # Negate for correct direction
                    self.position.y = max(min(-right_y_val, 1.0), -1.0)  # Negate for correct direction
                
                # Debug position changes
                if self.position.x != old_x or self.position.y != old_y:
                    logging.debug(f"[MyController] Position updated to: {self.position}")
                    
            except Exception as e:
                logging.error(f"[MyController] Error polling joystick: {e}")

    def _check_dpad_held(self) -> bool:
        """
        Check if any D-pad button is being held down and update position accordingly.
        Returns True if any D-pad button is being held.
        """
        # Skip D-pad checks in manual mode
        if not self._joystick_connected or self.navigation_mode == "manual":
            return False
            
        # Check D-pad buttons state by polling the joystick
        dpad_up = self.joystick.get_button(BUTTON_DPAD_UP) == 1
        dpad_down = self.joystick.get_button(BUTTON_DPAD_DOWN) == 1
        dpad_left = self.joystick.get_button(BUTTON_DPAD_LEFT) == 1
        dpad_right = self.joystick.get_button(BUTTON_DPAD_RIGHT) == 1
        
        # Update our internal state tracking
        self._button_states[BUTTON_DPAD_UP] = dpad_up
        self._button_states[BUTTON_DPAD_DOWN] = dpad_down
        self._button_states[BUTTON_DPAD_LEFT] = dpad_left
        self._button_states[BUTTON_DPAD_RIGHT] = dpad_right
        
        # Process D-pad input
        if dpad_up:
            self.position.x = 0
            self.position.y = 1
            return True
        elif dpad_down:
            self.position.x = 0
            self.position.y = -1
            return True
        elif dpad_left:
            self.position.x = -1
            self.position.y = 0
            return True
        elif dpad_right:
            self.position.x = 1
            self.position.y = 0
            return True
            
        return False

    def _handle_axis_motion(self, event: pygame.event.Event) -> None:
        """
        Handle axis motion events from the PS5 controller.
        """
        if self.navigation_mode == "assisted":
            # Assisted mode: standard mapping
            if event.axis == AXIS_LEFT_STICK_HORIZONTAL:
                self.position.x = max(min(event.value, 1.0), -1.0)
                logging.debug(f"[MyController] Left stick horizontal: {self.position.x}")
            elif event.axis == AXIS_LEFT_STICK_VERTICAL:
                # INVERT vertical axis for intuitive forward/backward control
                self.position.y = max(min(-event.value, 1.0), -1.0)  # Negate for correct direction
                logging.debug(f"[MyController] Left stick vertical: {self.position.y}")
            elif event.axis == AXIS_RIGHT_STICK_HORIZONTAL:
                self._on_right_stick_horizontal(event.value)
            elif event.axis == AXIS_RIGHT_STICK_VERTICAL:
                self._on_right_stick_vertical(event.value)
            elif event.axis == AXIS_LEFT_TRIGGER:
                self._on_left_trigger(event.value)
            elif event.axis == AXIS_RIGHT_TRIGGER:
                self._on_right_trigger(event.value)
        else:
            # Manual mode: Y-axis values only
            if event.axis == AXIS_LEFT_STICK_VERTICAL:
                # INVERT for intuitive control
                self.position.x = max(min(-event.value, 1.0), -1.0)  # Negate for correct direction
                logging.debug(f"[MyController] Left stick vertical (x position): {self.position.x}")
            elif event.axis == AXIS_RIGHT_STICK_VERTICAL:
                # INVERT for intuitive control
                self.position.y = max(min(-event.value, 1.0), -1.0)  # Negate for correct direction
                logging.debug(f"[MyController] Right stick vertical (y position): {self.position.y}")
            elif event.axis == AXIS_LEFT_STICK_HORIZONTAL:
                self._on_left_stick_horizontal(event.value)
            elif event.axis == AXIS_RIGHT_STICK_HORIZONTAL:
                self._on_right_stick_horizontal(event.value)
            elif event.axis == AXIS_LEFT_TRIGGER:
                self._on_left_trigger(event.value)
            elif event.axis == AXIS_RIGHT_TRIGGER:
                self._on_right_trigger(event.value)

    def _handle_button_press(self, event: pygame.event.Event) -> None:
        """
        Handle button press events from the PS5 controller.
        """
        if event.button == BUTTON_CROSS:
            self._on_cross_button()
        elif event.button == BUTTON_CIRCLE:
            self._on_circle_button()
        elif event.button == BUTTON_SQUARE:
            self._on_square_button()
        elif event.button == BUTTON_TRIANGLE:
            self._on_triangle_button()
        elif event.button == BUTTON_SHARE:
            self._on_share_button()
        elif event.button == BUTTON_PS:
            self._on_ps_button()
        elif event.button == BUTTON_OPTIONS:
            self._on_options_button()
        elif event.button == BUTTON_LEFT_STICK_IN:
            self._on_left_stick_in()
        elif event.button == BUTTON_RIGHT_STICK_IN:
            self._on_right_stick_in()
        elif event.button == BUTTON_LEFT_BUMPER:
            self._on_left_bumper()
        elif event.button == BUTTON_RIGHT_BUMPER:
            self._on_right_bumper()
        # Only handle D-pad in assisted mode
        elif self.navigation_mode == "assisted":
            if event.button == BUTTON_DPAD_UP:
                self._on_dpad_up()
            elif event.button == BUTTON_DPAD_DOWN:
                self._on_dpad_down()
            elif event.button == BUTTON_DPAD_LEFT:
                self._on_dpad_left()
            elif event.button == BUTTON_DPAD_RIGHT:
                self._on_dpad_right()
        elif event.button == BUTTON_TOUCHPAD_CLICK:
            self._on_touchpad_click()

    # D-pad handlers with correct directions
    def _on_dpad_up(self) -> None:
        """D-pad up button sets position to straight forward (0, 1)"""
        if self.navigation_mode != "assisted":
            return
            
        logging.info("[MyController] D-pad up pressed.")
        self.position.x = 0
        self.position.y = 1  # Forward direction (corrected)
        logging.debug(f"[MyController] Position set to: {self.position}")

    def _on_dpad_down(self) -> None:
        """D-pad down button sets position to straight backward (0, -1)"""
        if self.navigation_mode != "assisted":
            return
            
        logging.info("[MyController] D-pad down pressed.")
        self.position.x = 0
        self.position.y = -1  # Backward direction (corrected)
        logging.debug(f"[MyController] Position set to: {self.position}")

    def _on_dpad_left(self) -> None:
        """D-pad left button sets position to straight left (-1, 0)"""
        if self.navigation_mode != "assisted":
            return
            
        logging.info("[MyController] D-pad left pressed.")
        self.position.x = -1  # Left direction
        self.position.y = 0
        logging.debug(f"[MyController] Position set to: {self.position}")

    def _on_dpad_right(self) -> None:
        """D-pad right button sets position to straight right (1, 0)"""
        if self.navigation_mode != "assisted":
            return
            
        logging.info("[MyController] D-pad right pressed.")
        self.position.x = 1  # Right direction
        self.position.y = 0
        logging.debug(f"[MyController] Position set to: {self.position}")

    # Additional handler for the left stick horizontal in manual mode
    def _on_left_stick_horizontal(self, value: float) -> None:
        if self.navigation_mode == "manual":
            logging.debug(f"[MyController] Left stick horizontal (unused in manual mode): {value}")

    # Other button handlers
    def _on_right_stick_horizontal(self, value: float) -> None:
        logging.debug(f"[MyController] Right stick horizontal: {value}")
        # Add your custom action for the right stick horizontal axis here

    def _on_right_stick_vertical(self, value: float) -> None:
        logging.debug(f"[MyController] Right stick vertical: {value}")
        # Add your custom action for the right stick vertical axis here

    def _on_left_trigger(self, value: float) -> None:
        logging.debug(f"[MyController] Left trigger: {value}")
        # Add your custom action for the left trigger here

    def _on_right_trigger(self, value: float) -> None:
        logging.debug(f"[MyController] Right trigger: {value}")
        # Add your custom action for the right trigger here

    def _on_cross_button(self) -> None:
        logging.info("[MyController] Cross button pressed.")
        # Reset position to (0,0) on cross button press
        self.position.x = 0
        self.position.y = 0
        logging.debug(f"[MyController] Position reset: {self.position}")

    def _on_circle_button(self) -> None:
        logging.info("[MyController] Circle button pressed.")
        # Add your custom action for the circle button here

    def _on_square_button(self) -> None:
        logging.info("[MyController] Square button pressed.")
        # Add your custom action for the square button here

    def _on_triangle_button(self) -> None:
        logging.info("[MyController] Triangle button pressed.")
        # Add your custom action for the triangle button here

    def _on_share_button(self) -> None:
        logging.info("[MyController] Share/Create button pressed.")
        # Add your custom action for the share button here

    def _on_ps_button(self) -> None:
        logging.info("[MyController] PS button pressed.")
        # Add your custom action for the PS button here

    def _on_options_button(self) -> None:
        logging.info("[MyController] Options button pressed.")
        # Add your custom action for the options button here

    def _on_left_stick_in(self) -> None:
        logging.info("[MyController] Left stick button pressed.")
        # Add your custom action for the left stick button here

    def _on_right_stick_in(self) -> None:
        logging.info("[MyController] Right stick button pressed.")
        # Add your custom action for the right stick button here

    def _on_left_bumper(self) -> None:
        logging.info("[MyController] Left bumper pressed.")
        # Add your custom action for the left bumper here

    def _on_right_bumper(self) -> None:
        logging.info("[MyController] Right bumper pressed.")
        # Add your custom action for the right bumper here

    def _on_touchpad_click(self) -> None:
        logging.info("[MyController] Touchpad clicked.")
        # Add your custom action for the touchpad click here

    def get_position(self) -> Position:
        """
        Get the current position of the controller.

        Returns:
            Position: Current position with x and y values between -1 and 1
        """
        return self.position