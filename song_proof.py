import sys, pygame, io, fcntl, os, re

size = width, height = 1680, 800
speed = 2
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

stream = open('/dev/ttyACM0')
flags = fcntl.fcntl(stream, fcntl.F_GETFL) # get current p.stdout flags
fcntl.fcntl(stream, fcntl.F_SETFL, flags | os.O_NONBLOCK)

r = pygame.Rect(width - 30, height / 2, 30, block_height)

def midi_to_note(n):
    octave = n / 12
    if octave > 1:
        note = n % octave
    else:
        note = n
    return (note, octave)

def midi_to_note(n):
    octave = n / 12
    note = n % octave
    return (note, octave)

while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: 
            stream.close()
            sys.exit()

    try:
        line = stream.readline()
        line_match = re.match('(ON|OFF): (\d*)', line)
        if line_match:
            action, midi_note = line_match.groups()
            if current_note != None:
                blocks.append(current_note)
                current_note = None
            if action == 'ON':
                note, octave = midi_to_note(int(midi_note))
                try:
                    c = colours[note]
                except IndexError:
                    pass
                current_note = [c, pygame.Rect(width, height - (octave * 80 + note * block_height), 0, block_height)]

    except IOError:
        pass

    screen.fill(black)

    if current_note is not None:
        block_colour, block_rect = current_note
        pygame.draw.rect(screen, block_colour, block_rect)
        r.topleft = (r.topleft[0], block_rect.topleft[1])

        block_rect.inflate_ip(speed, 0)
        block_rect.right = width

    for block in blocks:
        block_colour, block_rect = block
        if block_rect.right < 0:
            blocks.remove(block)
        else:
            pygame.draw.rect(screen, block_colour, block_rect)
            block[1] = block_rect.move(speed * -1, 0)

    pygame.draw.rect(screen, (255, 255, 255), r)
    pygame.display.flip()
