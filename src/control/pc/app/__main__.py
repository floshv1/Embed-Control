import logging
import argparse
from logs import Logs
from models.server_pinger import ServerPinger
from keyboard_input_handler import KeyboardInputHandler
from controller_input_handler import ControllerInputHandler

# Server addresses configuration
SERVER_CONFIGS = {"dev": "localhost", "prod": "travelers5.local"}

Logs.start(logging.INFO)

# Parse command-line arguments
parser = argparse.ArgumentParser(
    description="Controller application for Travelers rover"
)
parser.add_argument(
    "-i",
    "--input",
    choices=["keyboard", "controller"],
    default="controller",
    help="Input method (keyboard or controller)",
)
parser.add_argument(
    "-v",
    "--version",
    choices=["dev", "prod"],
    default="prod",
    help="Environment version: dev (localhost) or prod (travelers5.local)",
)
parser.add_argument(
    "-n",
    "--navigation",
    choices=["manual", "assisted"],
    default="manual",
    help="Navigation mode: manual or assisted",
)
args = parser.parse_args()

SERVER = SERVER_CONFIGS[args.version]
logging.info(f"[Main] Using {args.version} environment with server: {SERVER}")

SERVER_HTTP_ADDRESS = f"http://{SERVER}:8765/"
SERVER_WS_ADDRESS = f"ws://{SERVER}:8765/"
CONTROLLER_URL = SERVER_WS_ADDRESS + "general/controller"

pinger = ServerPinger(SERVER_HTTP_ADDRESS)
if not pinger.ping():
    logging.critical("[Main] Server is not active. Exiting.")
    exit()

if args.input == "controller":
    logging.info("[Main] Using controller input (specified by argument).")
    input_handler = ControllerInputHandler(CONTROLLER_URL, args.navigation)
elif args.input == "keyboard":
    logging.info("[Main] Using keyboard input (specified by argument).")
    input_handler = KeyboardInputHandler(CONTROLLER_URL)

logging.info("[Main] Starting input handler.")
input_handler.start_websocket()
input_handler.handle_input()
