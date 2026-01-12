from PIL import Image, ImageDraw

# Create a 64x64 image
size = 64
img = Image.new('RGBA', (size, size), (26, 26, 46, 255))
draw = ImageDraw.Draw(img)

# Draw rounded rectangle background
draw.rounded_rectangle([(0, 0), (size-1, size-1)], radius=8, fill=(26, 26, 46, 255))

# Draw T block (purple)
blocks = [
    # T block
    [(8, 8, 22, 22, (155, 89, 182, 255)),
     (24, 8, 38, 22, (155, 89, 182, 255)),
     (40, 8, 54, 22, (155, 89, 182, 255)),
     (24, 24, 38, 38, (155, 89, 182, 255))],
    # I block (cyan)
    [(48, 8, 62, 22, (0, 212, 255, 255)),
     (48, 24, 62, 38, (0, 212, 255, 255)),
     (48, 40, 62, 54, (0, 212, 255, 255))],
    # O block (yellow)
    [(8, 28, 22, 42, (241, 196, 15, 255)),
     (24, 28, 38, 42, (241, 196, 15, 255)),
     (8, 44, 22, 58, (241, 196, 15, 255)),
     (24, 44, 38, 58, (241, 196, 15, 255))],
]

for block_group in blocks:
    for x1, y1, x2, y2, color in block_group:
        draw.rounded_rectangle([(x1, y1), (x2-1, y2-1)], radius=2, fill=color)

# Save as ICO with multiple sizes
img.save('../resources/tetris.ico', format='ICO', sizes=[(16, 16), (32, 32), (48, 48), (64, 64), (256, 256)])
print("Icon created successfully!")
