import logging
from input_handler import InputHandler
from models.position import Position
from pynput import keyboard


class KeyboardInputHandler(InputHandler):
    """
    Handles input from the keyboard and sends positions to the server.
    """

    def __init__(self, websocket_url: str) -> None:
        super().__init__(websocket_url)
        logging.info("[KeyboardInputHandler] Initialized.")
        self.listener = keyboard.Listener(on_press=self.on_press)
        self.key_actions = {
            keyboard.Key.up: lambda: self.send_position(Position(0, 1)),
            keyboard.Key.down: lambda: self.send_position(Position(0, -1)),
            keyboard.Key.right: lambda: self.send_position(Position(1, 0)),
            keyboard.Key.left: lambda: self.send_position(Position(-1, 0)),
            keyboard.Key.space: lambda: self.send_position(Position(0, 0)),
            keyboard.Key.esc: self.listener.stop,
        }

    def on_press(self, key: keyboard.Key) -> None:
        """
        Handle key press events and send corresponding positions to the server.
        """
        action = self.key_actions.get(key)
        if action:
            action()
            logging.debug(f"Key pressed: {key}")

    def handle_input(self) -> None:
        """
        Start the keyboard listener and handle input events.
        """
        try:
            with self.listener as listener:
                listener.join()
        except KeyboardInterrupt:
            logging.info("[KeyboardInputHandler] Stopping listener.")
        except Exception as e:
            logging.error(f"[KeyboardInputHandler] Error: {e}")

    def start(self) -> None:
        """
        Start the input handler.
        """
        super().start()
        self.handle_input()
