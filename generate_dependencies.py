from PIL import Image, ImageDraw, ImageFont
import os
from jinja2 import Environment as JinjaEnv, FileSystemLoader
import json
import re

bitmap_dir = 'src/assets/bitmaps'
font_size_list = [15, 20, 25, 30]

def sanitize_filename(char):
    """Return a safe filename for a given character."""
    if char.isalnum():
        numbers_to_words = {
            '0': 'zero',
            '1': 'one',
            '2': 'two',
            '3': 'three',
            '4': 'four',
            '5': 'five',
            '6': 'six',
            '7': 'seven',
            '8': 'eight',
            '9': 'nine'
        }
        return numbers_to_words.get(char, char)
    else:
        # For special characters, return a description (e.g., space becomes 'space')
        special_chars = {
            ' ': 'space',
            '!': 'exclamation',
            '@': 'at',
            '#': 'hash',
            '$': 'dollar',
            '%': 'percent',
            '^': 'caret',
            '&': 'ampersand',
            '*': 'asterisk',
            '(': 'left_paren',
            ')': 'right_paren',
            '-': 'dash',
            '+': 'plus',
            '=': 'equals',
            '{': 'left_brace',
            '}': 'right_brace',
            '[': 'left_bracket',
            ']': 'right_bracket',
            ':': 'colon',
            ';': 'semicolon',
            ',': 'comma',
            '.': 'period',
            '?': 'question',
            '/': 'slash',
            '\\': 'backslash',
            '|': 'pipe',
            '~': 'tilde',
            '`': 'backtick',
            '"': 'quote',
            '\'': 'apostrophe'
        }
        return special_chars.get(char, f'char_{ord(char)}')

def draw_arrow(direction, save_path=f'{bitmap_dir}/shapes'):
    os.makedirs(save_path, exist_ok=True)
    if direction == 'up':
        img_size = (10, 5)
        arrowhead = [(5, 0), (10, 5), (0, 5)]
    elif direction == 'down':
        img_size = (10, 5)
        arrowhead = [(5, 5), (10, 0), (0, 0)]
    elif direction == 'left':
        img_size = (5, 10)
        arrowhead = [(0, 5), (5, 10), (5, 0)]
    elif direction == 'right':
        img_size = (5, 10)
        arrowhead = [(5, 5), (0, 0), (0, 10)]
    else:
        raise ValueError("Direction must be 'up', 'down', 'left', or 'right'.")

    # Create a new image with a transparent background
    img = Image.new('RGBA', img_size, (255, 255, 255, 0))
    draw = ImageDraw.Draw(img)

    draw.polygon(arrowhead, fill='black')

    img.save(f'{save_path}/arrow_{direction}.bmp', format='BMP')

def draw_parallelogram(save_path=f'{bitmap_dir}/shapes'):
    os.makedirs(save_path, exist_ok=True)

    width, height = 20, 10
    image = Image.new("RGBA", (width, height), (255, 255, 255, 0))
    draw = ImageDraw.Draw(image)

    points = [(5, 0), (width - 2, 0), (width - 7, height), (0, height)]

    draw.polygon(points, fill=(0, 0, 0, 255))

    image.save(f'{save_path}/parallelogram_right.bmp', format='BMP')

def create_ascii_images(font_filename='arial.ttf', font_size=20, save_path=f'{bitmap_dir}/fonts'):
    font_name = os.path.splitext(os.path.basename(font_filename))[0].replace('-', '_')

    save_path = f'{save_path}/{font_name}/{font_size}'
    os.makedirs(save_path, exist_ok=True)

    try:
        font = ImageFont.truetype(font_filename, font_size)
    except IOError:
        font = ImageFont.load_default()

    for i in range(32, 127):  # ASCII printable characters range from 32 to 126
        char = chr(i)
        (width, height) = font.getsize(char)
        
        # Create a new image with transparent background, size just fit the character
        image = Image.new('RGBA', (width, height), (255, 255, 255, 0))
        draw = ImageDraw.Draw(image)
        
        # Draw the character in black
        draw.text((0, 0), char, font=font, fill=(0, 0, 0, 255))
        safe_filename = sanitize_filename(char)

        image.save(f'{save_path}/{font_name}_{font_size}_{safe_filename}.bmp', format='BMP')

STATE_FILE = 'src/assets/bitmaps_state.json'

def get_directory_state(directory):
    """Get the current state of the directory with file modification times."""
    directory_state = {}
    for root, _, files in os.walk(directory):
        for file in files:
            if file.lower().endswith('.bmp'):
                file_path = os.path.join(root, file)
                directory_state[file_path] = os.path.getmtime(file_path)
    return directory_state

def has_directory_changed(directory):
    """Check if the directory has changed by comparing the current state with the saved state."""
    current_state = get_directory_state(directory)
    
    if not os.path.exists(STATE_FILE):
        return True

    with open(STATE_FILE, 'r') as f:
        saved_state = json.load(f)

    return current_state != saved_state

def save_directory_state(directory):
    """Save the current state of the directory to a file."""
    current_state = get_directory_state(directory)
    with open(STATE_FILE, 'w') as f:
        json.dump(current_state, f)

def get_bitmap_data(file_path):
    with Image.open(file_path) as img:
        # Ensure the image is in Grayscale format
        img = img.convert("L")

        width, height = img.size

        raw_data = list(img.getdata())

    return width, height, raw_data

def split_at_first_number(s):
    match = re.search(r'\d+', s)
    if match:
        index = match.end()
        left_part = s[:index]
        return left_part
    return s

from collections import defaultdict


def generate_code():
    jinja_env = JinjaEnv(loader=FileSystemLoader('src/assets/bitmaps'))

    # Load templates for header, source, sub-factory header, sub-factory source, and main factory header/source
    header_bitmap_template = jinja_env.get_template('bitmap_class_template.h.jinja2')
    source_bitmap_template = jinja_env.get_template('bitmap_class_template.cpp.jinja2')
    sub_factory_header_template = jinja_env.get_template('bitmap_class_sub_factory.h.jinja2')
    sub_factory_source_template = jinja_env.get_template('bitmap_class_sub_factory.cpp.jinja2')
    factory_header_template = jinja_env.get_template('bitmap_class_factory.h.jinja2')
    factory_source_template = jinja_env.get_template('bitmap_class_factory.cpp.jinja2')

    grouped_classes_data = defaultdict(list)

    # Collect class data and group by the common base name
    for root, _, files in os.walk(bitmap_dir):
        for file in files:
            if file.lower().endswith('.bmp'):
                bmp_file = os.path.join(root, file)
                width, height, data = get_bitmap_data(bmp_file)
                class_name = os.path.splitext(file)[0]
                base_name = split_at_first_number(class_name)
                grouped_classes_data[base_name].append({
                    'class_name': class_name,
                    'width': width,
                    'height': height,
                    'data': data,
                })

    # Generate header, source, and sub-factory files for each group of classes
    for base_name, classes_data in grouped_classes_data.items():
        # Header file for bitmap data classes
        header_file = f'src/generated/bitmap_datas/{base_name}.h'
        os.makedirs(os.path.dirname(header_file), exist_ok=True)
        with open(header_file, 'w') as f:
            f.write(header_bitmap_template.render(base_name=base_name, classes_data=classes_data))
        
        # Source file for bitmap data classes
        source_file = f'src/generated/bitmap_datas/{base_name}.cpp'
        os.makedirs(os.path.dirname(source_file), exist_ok=True)
        with open(source_file, 'w') as f:
            f.write(source_bitmap_template.render(base_name=base_name, classes_data=classes_data))

        # Sub-factory header file for each base_name
        sub_factory_header_file = f'src/generated/bitmap_factories/{base_name}_factory.h'
        os.makedirs(os.path.dirname(sub_factory_header_file), exist_ok=True)
        with open(sub_factory_header_file, 'w') as f:
            f.write(sub_factory_header_template.render(base_name=base_name, classes=classes_data))

        # Sub-factory source file for each base_name
        sub_factory_source_file = f'src/generated/bitmap_factories/{base_name}_factory.cpp'
        os.makedirs(os.path.dirname(sub_factory_source_file), exist_ok=True)
        with open(sub_factory_source_file, 'w') as f:
            f.write(sub_factory_source_template.render(base_name=base_name, classes=classes_data))

    # Generate the main factory header file
    factory_header_file = 'src/generated/bitmap_class_factory.h'
    with open(factory_header_file, 'w') as f:
        f.write(factory_header_template.render(base_names=grouped_classes_data.keys()))

    # Generate the main factory source file
    factory_source_file = 'src/generated/bitmap_class_factory.cpp'
    with open(factory_source_file, 'w') as f:
        # Render factory source file, including base_name keys for factory map
        f.write(factory_source_template.render(base_names=grouped_classes_data.keys()))

    print(f'Generated header and source files for all grouped classes.')
    print(f'Generated main factory header written to {factory_header_file}')
    print(f'Generated main factory source written to {factory_source_file}')

if __name__ == '__main__':
    font_dir = 'src/assets/fonts'
    for root, _, files in os.walk(font_dir):
        for file in files:
            if file.lower().endswith('.ttf'):
                font_file = os.path.join(root, file)
                for i in font_size_list:
                    create_ascii_images(font_filename=font_file, font_size=i)
                    
    draw_arrow('up')
    draw_arrow('down')
    draw_arrow('left')
    draw_arrow('right')
    draw_parallelogram()
    
    if has_directory_changed(bitmap_dir):
        print("Changes detected, regenerating code...")
        generate_code()
        save_directory_state(bitmap_dir)
    else:
        print("No changes detected, skipping code generation.")
