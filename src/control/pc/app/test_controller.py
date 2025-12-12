import pygame
import sys

# Initialize pygame
pygame.init()

# Create window
window = pygame.display.set_mode((800, 600))
pygame.display.set_caption("TRAVELERS PS5 Controller Test")
font = pygame.font.SysFont(None, 24)

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)
PURPLE = (128, 0, 128)

# PS5 Button names (may vary slightly from PS4)
BUTTON_NAMES = {
    0: "Cross (×)",
    1: "Circle (○)",
    2: "Square (□)",
    3: "Triangle (△)",
    4: "Share/Create",
    5: "PS Button",
    6: "Options",
    7: "L3 (Left Stick)",
    8: "R3 (Right Stick)",
    9: "L1",
    10: "R1",
    11: "D-Pad Up",
    12: "D-Pad Down",
    13: "D-Pad Left",
    14: "D-Pad Right",
    15: "Touchpad"
}

# PS5 Axis names
AXIS_NAMES = {
    0: "Left Stick Horizontal",
    1: "Left Stick Vertical",
    2: "Right Stick Horizontal",
    3: "Right Stick Vertical", 
    4: "L2 Trigger",
    5: "R2 Trigger"
}

# Check for controllers
pygame.joystick.init()
available = pygame.joystick.get_count()
if available == 0:
    print("No controllers found.")
    pygame.quit()
    sys.exit()

print(f"{available} controller(s) found.")
controller = pygame.joystick.Joystick(0)
controller.init()
print(f"Connected to: {controller.get_name()}")
print(f"Number of axes: {controller.get_numaxes()}")
print(f"Number of buttons: {controller.get_numbuttons()}")
print(f"Number of hats: {controller.get_numhats()}")

# Initialize variables to track state
button_states = [False] * controller.get_numbuttons()
axis_values = [0.0] * controller.get_numaxes()
hat_values = [(0, 0)] * controller.get_numhats()

def draw_joystick(x, y, value_x, value_y, label):
    """Draw a visual representation of a joystick"""
    # Draw the base circle
    pygame.draw.circle(window, WHITE, (x, y), 50, 2)
    
    # Draw the center point
    pygame.draw.circle(window, WHITE, (x, y), 5)
    
    # Calculate stick position (value_x and value_y are between -1 and 1)
    stick_x = x + int(value_x * 40)
    stick_y = y + int(value_y * 40)
    
    # Draw the stick
    pygame.draw.circle(window, RED, (stick_x, stick_y), 10)
    
    # Draw label
    text = font.render(label, True, WHITE)
    window.blit(text, (x - 40, y + 60))
    
    # Draw values
    value_text = font.render(f"X: {value_x:.2f}, Y: {value_y:.2f}", True, WHITE)
    window.blit(value_text, (x - 60, y + 80))

def draw_trigger(x, y, value, label):
    """Draw a visual representation of a trigger"""
    # Draw background bar
    pygame.draw.rect(window, WHITE, (x, y, 20, 100), 2)
    
    # Draw filled portion based on trigger value (0 to 1)
    height = int(value * 100)
    pygame.draw.rect(window, BLUE, (x, y + (100 - height), 20, height))
    
    # Draw label
    text = font.render(label, True, WHITE)
    window.blit(text, (x - 10, y + 110))
    
    # Draw value
    value_text = font.render(f"{value:.2f}", True, WHITE)
    window.blit(value_text, (x - 10, y + 130))

# Main game loop
clock = pygame.time.Clock()
done = False

while not done:
    window.fill(BLACK)
    
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        elif event.type == pygame.JOYBUTTONDOWN:
            button_states[event.button] = True
            print(f"Button {event.button} ({BUTTON_NAMES.get(event.button, 'Unknown')}) pressed")
        elif event.type == pygame.JOYBUTTONUP:
            button_states[event.button] = False
        elif event.type == pygame.JOYAXISMOTION:
            axis_values[event.axis] = event.value
            if abs(event.value) > 0.1:  # Filter out small movements
                print(f"Axis {event.axis} ({AXIS_NAMES.get(event.axis, 'Unknown')}): {event.value:.2f}")
        elif event.type == pygame.JOYHATMOTION:
            hat_values[event.hat] = event.value
            print(f"Hat {event.hat}: {event.value}")
            
    # Poll controller for current state
    for i in range(controller.get_numaxes()):
        axis_values[i] = controller.get_axis(i)
    
    for i in range(controller.get_numbuttons()):
        button_states[i] = controller.get_button(i)
    
    # Draw title
    title = font.render(f"PS5 Controller: {controller.get_name()}", True, WHITE)
    window.blit(title, (20, 20))
    
    # Draw joysticks
    draw_joystick(150, 150, axis_values[0], axis_values[1], "Left Stick")
    draw_joystick(650, 150, axis_values[2], axis_values[3], "Right Stick")
    
    # Draw triggers
    draw_trigger(250, 300, (axis_values[4] + 1) / 2, "L2")  # Convert -1..1 to 0..1
    draw_trigger(550, 300, (axis_values[5] + 1) / 2, "R2")  # Convert -1..1 to 0..1
    
    # Draw buttons
    button_y = 450
    for i in range(min(8, controller.get_numbuttons())):  # First row (8 buttons)
        color = RED if button_states[i] else WHITE
        pygame.draw.circle(window, color, (60 + i * 90, button_y), 20)
        btn_text = font.render(BUTTON_NAMES.get(i, f"Btn {i}"), True, WHITE)
        window.blit(btn_text, (40 + i * 90, button_y + 30))
    
    button_y = 530
    for i in range(8, min(16, controller.get_numbuttons())):  # Second row (next 8 buttons)
        color = RED if button_states[i] else WHITE
        pygame.draw.circle(window, color, (60 + (i-8) * 90, button_y), 20)
        btn_text = font.render(BUTTON_NAMES.get(i, f"Btn {i}"), True, WHITE)
        window.blit(btn_text, (40 + (i-8) * 90, button_y + 30))
    
    # Instructions
    instructions = font.render("Press buttons or move sticks to test. Close window to exit.", True, YELLOW)
    window.blit(instructions, (20, 50))
    
    pygame.display.flip()
    clock.tick(60)

pygame.quit()