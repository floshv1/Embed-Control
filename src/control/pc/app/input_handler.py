import logging
import websocket
import threading
from models.position import Position


class InputHandler:
    """Base handler for inputs."""

    def __init__(self, websocket_url: str):
        self.websocket_url = websocket_url
        self.ws = None
        self.connected = False
        self.ws_thread = None
        logging.info("[InputHandler] Initialized.")

    def on_open(self, ws):
        self.connected = True
        logging.info("[InputHandler] WebSocket connection opened.")

    def on_message(self, ws, message):
        logging.debug(f"[InputHandler] Received message: {message}")

    def on_error(self, ws, error):
        logging.error(f"[InputHandler] Error: {error}")
        self.connected = False

    def on_close(self, ws, close_status_code=None, close_reason=None):
        logging.info("[InputHandler] WebSocket connection closed.")
        self.connected = False

    def start_websocket(self):
        """Start the WebSocket connection."""
        if self.ws_thread and self.ws_thread.is_alive():
            logging.debug("[InputHandler] WebSocket thread already running")
            return

        self.ws = websocket.WebSocketApp(
            self.websocket_url,
            on_open=self.on_open,
            on_message=self.on_message,
            on_error=self.on_error,
            on_close=self.on_close,
        )
        self.ws_thread = threading.Thread(target=self.ws.run_forever)
        self.ws_thread.daemon = True
        self.ws_thread.start()
        logging.info("[InputHandler] WebSocket thread started")

    def send_position(self, position: Position):
        """Send position to the server."""
        if not self.connected:
            logging.debug("[InputHandler] Not sending position, not connected")
            return

        try:
            logging.debug(f"[InputHandler] Sending position: {position}")
            self.ws.send(str(position))
        except Exception as e:
            logging.error(f"[InputHandler] Error sending position: {e}")
            self.connected = False

    def stop(self):
        """Stop the WebSocket connection."""
        if self.ws:
            self.ws.close()
            self.connected = False
