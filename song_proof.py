import sys, pygame, io, fcntl, os, re

size = width, height = 1400, 800
speed = 3
block_height = 10
black = (17, 17, 17)
colours = [(87, 74, 206), (8, 112, 244), (55, 166, 214), (91, 196, 246),
           (78, 216, 105), (252, 203, 47), (250, 147, 34), (249, 54, 48)]
blocks = []
current_note = None

pygame.init()
screen = pygame.display.set_mode(size)

flags = fcntl.fcntl(sys.stdin, fcntl.F_GETFL) # get current p.stdout flags
fcntl.fcntl(sys.stdin, fcntl.F_SETFL, flags | os.O_NONBLOCK)

while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()

    try:
        line = sys.stdin.readline()
        print line
        line_match = re.match('(ON|OFF): ([A-G])(\d*)', line)
        action, note, octave = line_match.groups()
        if action == 'ON':
            current_note = [colours[ord(note) - 65], pygame.Rect(width, 250 + (ord(note) - 65) * block_height, 0, block_height)]
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
