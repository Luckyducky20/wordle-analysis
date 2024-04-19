import keyboard
import time

def load_words(filename):
    # Load words from a file
    with open(filename, 'r') as file:
        return file.read().split()

def type_words(words):
    # Give some time to switch to the target text field after running the script
    print("Starting in 5 seconds...")
    time.sleep(5)

    # Type each word, press Enter, and then backspace the word
    for word in words:
        keyboard.write(word)  # Type the word
        keyboard.press_and_release('enter')  # Press Enter
        time.sleep(0.5)  # Wait a bit to simulate typing and let the action complete
        
        # Backspace the word including the newline character if necessary
        # Some text fields may not need to remove the newline, adjust accordingly
        for _ in range(len(word) + 1):  # Adjust +1 if needed depending on the application behavior
            keyboard.press_and_release('backspace')
        time.sleep(0.5)  # Wait a bit between actions

if __name__ == '__main__':
    words = load_words('words.txt')
    type_words(words)
