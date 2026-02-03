import gpiod
import time
from gpiod.line import Direction, Value

# Define our chip and pin (offset)
CHIP_PATH = "/dev/gpiochip0"
LINE_OFFSET = 23
# 1. Open the chip and request the line as output
# The 'with' statement automatically closes the chip when finished
with gpiod.request_lines(
    CHIP_PATH,
    consumer="BlinkPy",
    config={
        LINE_OFFSET: gpiod.LineSettings(
            direction=Direction.OUTPUT,
            output_value=Value.INACTIVE
        )
    }
) as request:
    print(f"Blinking LED on {CHIP_PATH} offset {LINE_OFFSET}...")
    
    try:
        while True:
            # 2. Get current value and flip it
            current_val = request.get_value(LINE_OFFSET)
            new_val = Value.ACTIVE if current_val == Value.INACTIVE else Value.INACTIVE
            
            request.set_value(LINE_OFFSET, new_val)
            
            print(f"LED is {'ON' if new_val == Value.ACTIVE else 'OFF'}")
            time.sleep(0.5)
            
    except KeyboardInterrupt:
        print("\nStopping...")
        # Pins are automatically released here