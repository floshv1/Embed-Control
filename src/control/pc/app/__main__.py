import logging
import argparse
from logs import Logs
from models.server_pinger import ServerPinger
from keyboard_input_handler import KeyboardInputHandler
from controller_input_handler import ControllerInputHandler

# Server address:
# For development
# SERVER = "localhost"
# For production
SERVER = "travelers5.local"

SERVER_HTTP_ADDRESS = f"http://{SERVER}:8765/"
SERVER_WS_ADDRESS = f"ws://{SERVER}:8765/"
CONTROLLER_URL = SERVER_WS_ADDRESS + "general/controller"

Logs.start()

# Parse command-line arguments
parser = argparse.ArgumentParser(description='Controller application for Travelers rover')
parser.add_argument('-i', '--input', choices=['keyboard', 'controller'], 
                    help='Input method (keyboard or controller)')
args = parser.parse_args()

pinger = ServerPinger(SERVER_HTTP_ADDRESS)
if not pinger.ping():
    logging.critical("[Main] Server is not active. Exiting.")
    exit()

# Determine input method based on command-line args or platform
if args.input:
    # Use explicitly specified input method
    if args.input == 'controller':
        logging.info("[Main] Using controller input (specified by argument).")
        input_handler = ControllerInputHandler(CONTROLLER_URL)
    elif args.input == 'keyboard':
        logging.info("[Main] Using keyboard input (specified by argument).")
        input_handler = KeyboardInputHandler(CONTROLLER_URL)
else:
    logging.info("[Main] Using keyboard input (default).")
    input_handler = KeyboardInputHandler(CONTROLLER_URL)

logging.info("[Main] Starting input handler.")
# Run both WebSocket and input handling in a single thread
input_handler.start_websocket()
input_handler.handle_input()  # This must run on the main thread