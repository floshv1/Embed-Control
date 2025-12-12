import logging
import pygame
import time
from input_handler import InputHandler
from models.controller import MyController
from models.position import Position


class ControllerInputHandler(InputHandler):
    """
    Handles input from the PS5 controller and sends positions to the server.
    """

    def __init__(self, websocket_url: str, navigation: str) -> None:
        super().__init__(websocket_url)
        
        # Initialize controller
        self.controller = MyController(navigation)
        self.running = True
        self.last_send_time = time.time()
        self.deadzone = 0.08  # Apply deadzone to the controller values
        self.send_interval = 0.2  # Send position every 15ms (faster updates)
        self.last_sent_position = Position(0.0, 0.0)
        self.last_zeros_sent = False  # Track if we've already sent zeros
        self.heartbeat_interval = 5.0  # Send a heartbeat every 5 seconds if inactive
        logging.info("[ControllerInputHandler] Initialized.")
        
        # Start websocket connection right away
        self.start_websocket()

    def handle_input(self) -> None:
        """
        Handle input from the PS5 controller and send positions to the server.
        """
        try:
            while self.running:
                try:
                    # Handle events and get position
                    self.controller.handle_events()
                    current_position = self.controller.position
                    current_time = time.time()
                    
                    # Apply deadzone to avoid small drifts
                    if abs(current_position.x) < self.deadzone:
                        current_position.x = 0.0
                    if abs(current_position.y) < self.deadzone:
                        current_position.y = 0.0
                    
                    # Check if current position is zeros
                    is_zero_position = current_position.x == 0.0 and current_position.y == 0.0
                    
                    # Determine if we should send the position
                    position_changed = (
                        current_position.x != self.last_sent_position.x or 
                        current_position.y != self.last_sent_position.y
                    )
                    time_to_send = current_time - self.last_send_time >= self.send_interval
                    inactive_time = current_time - self.last_send_time
                    
                    should_send = False
                    
                    # 1. If not zeros or position changed, send it
                    if not is_zero_position or (position_changed and time_to_send):
                        should_send = True
                        self.last_zeros_sent = False
                    # 2. If zeros and we haven't sent zeros yet, send once
                    elif is_zero_position and position_changed and not self.last_zeros_sent:
                        should_send = True
                        self.last_zeros_sent = True
                    # 3. If inactive for heartbeat_interval, send zeros as heartbeat
                    elif inactive_time >= self.heartbeat_interval:
                        should_send = True
                        # Only log at debug level when sending heartbeats
                        logging.info(f"[ControllerInputHandler] Sending heartbeat after {inactive_time:.1f}s of inactivity")
                    
                    if should_send:
                        self.send_position(current_position)
                        self.last_sent_position = Position(current_position.x, current_position.y)
                        self.last_send_time = current_time
                    
                    time.sleep(self.send_interval)  # Small delay to prevent high CPU usage
                    
                except Exception as e:
                    logging.error(f"[ControllerInputHandler] Error: {e}")
                    time.sleep(0.5)
                    
        except KeyboardInterrupt:
            logging.info("[ControllerInputHandler] Keyboard interrupt received")
        finally:
            try:
                self.stop()
                pygame.quit()
            except Exception as e:
                logging.error(f"[ControllerInputHandler] Error during cleanup: {e}")

    def send_position(self, position: Position) -> None:
        """
        Send position data to the server, ensuring connection is active.
        """
        try:
            # Force reconnection if needed
            if not self.connected:
                logging.info("[ControllerInputHandler] Connection not active, reconnecting...")
                self.start_websocket()
                time.sleep(0.5)  # Give it time to connect
            
            # Now send the position
            is_zero = position.x == 0.0 and position.y == 0.0
            
            # Use different log levels based on if it's a zero position
            if is_zero:
                # Check if it's a heartbeat (time since last send is close to heartbeat interval)
                time_since_last = time.time() - self.last_send_time
                if time_since_last > self.heartbeat_interval * 0.9:  # 90% of heartbeat interval
                    logging.debug("[ControllerInputHandler] Sending zero heartbeat")
                else:
                    logging.debug(f"[ControllerInputHandler] Sending zero position: {position}")
            else:
                logging.info(f"[ControllerInputHandler] Sending position: {position}")
            
            super().send_position(position)
            
        except Exception as e:
            logging.error(f"[ControllerInputHandler] Error sending position: {e}")
            # Mark as disconnected so we'll try to reconnect next time
            self.connected = False

    def stop(self):
        """Stop the controller input handler"""
        self.running = False
        try:
            super().stop()
        except Exception as e:
            logging.error(f"[ControllerInputHandler] Error stopping handler: {e}")