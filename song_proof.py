import sys, pygame, io, fcntl, os, re

size = width, height = 1400, 800
speed = 3
block_height = 10
black = (17, 17, 17)
colours = [(163, 21, 141), (237, 15, 37), (238, 59, 63),
           (246, 129, 45), (249, 168, 48), (255, 225, 59),
           (112, 193, 80), (2, 167, 101), (5, 171, 173),
           (3, 100, 178), (34, 59, 153), (92, 38, 143)]
blocks = []
current_note = None

pygame.init()
screen = pygame.display.set_mode(size)

flags = fcntl.fcntl(sys.stdin, fcntl.F_GETFL) # get current p.stdout flags
fcntl.fcntl(sys.stdin, fcntl.F_SETFL, flags | os.O_NONBLOCK)

def midi_to_note(n):
    octave = n / 12
    note = n % octave
    return (note, octave)

while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()

    try:
        line = sys.stdin.readline()
        print line
        line_match = re.match('(ON|OFF): (\d*)', line)
        action, midi_note = line_match.groups()
        if action == 'ON':
            note, octave = midi_to_note(int(midi_note))
            current_note = [colours[note], pygame.Rect(width, octave * 80 + note * block_height, 0, block_height)]
        else:
            blocks.append(current_note)
            current_note = None

    except IOError:
        pass

    screen.fill(black)

    if current_note is not None:
        block_colour, block_rect = current_note
        pygame.draw.rect(screen, block_colour, block_rect)
        block_rect.inflate_ip(speed, 0)
        block_rect.right = width

    for block in blocks:
        block_colour, block_rect = block
        if block_rect.right < 0:
            blocks.remove(block)
        else:
            pygame.draw.rect(screen, block_colour, block_rect)
            block[1] = block_rect.move(speed * -1, 0)

    pygame.display.flip()