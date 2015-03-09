import sys, random, math, serial, pygame
from pygame.locals import *
from pygame.examples import *


global screen_width, screen_height, positions, colors

GRAY = ( 35,  35,  35)
RED  = (255,   0,   0)
BLUE = (  1,   1, 255)
screen_width = 800
screen_height = 800
positions = []
colors = []
volumes = []
for i in range (0 ,48):
	volumes.append(0)

def get_color():
	ind = [random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)]
	random.shuffle(ind)
	return (ind[0], ind[1], ind[2])

def draw_circles(screen, no, centre,  width):
	for circle in range (0, no):
		pygame.draw.circle(screen, get_color(), [centre+random.randint(-60, 60), centre+random.randint(-60, 60)], random.randint(width, centre - 120), width)
	return
def draw_lines(screen, line_width):
	for line in range(1, screen_width - 1, line_width + 1):
		pygame.draw.line(screen, get_color(), (line, screen_height), (line, screen_height - random.randint(0, screen_height)), line_width)
	return

def assign_colors(divisions):
	for i in range (0, divisions):
		colors.append(get_color())

def assign_positions(divisions, radius):
	angle = 0.0
	assign_colors(divisions)
	for i in range (0,divisions):
		positions.append([0, 0])
		positions[i][0] = int( screen_width/2 + (radius*math.cos(math.radians(angle))))
		positions[i][1] = int( screen_height/2 + (radius*math.sin(math.radians(angle))))			
		angle += 360.0/(divisions-1)
	return

ser = serial.Serial('/dev/cu.usbmodemfd122', 9600, timeout=1, parity=serial.PARITY_EVEN, rtscts=1)
pygame.init()
screen = pygame.display.set_mode([screen_width, screen_height], pygame.RESIZABLE)

pygame.display.set_caption("Fo real?!")

done = False
clock = pygame.time.Clock()
assign_positions(48, 50)
while not done:
	# This limits the while loop to a max of 10 times per second.
    # Leave this out and we will use all CPU we can.
	clock.tick(9600)
     
	for event in pygame.event.get(): # User did something
		if event.type == pygame.QUIT or event.__dict__ == {'scancode': 53, 'key': 27, 'unicode': u'\x1b', 'mod': 0}: # If user clicked close or pressed ESC
			done=True # Flag that we are done so we exit this loop
	screen.fill(GRAY)

	try:
		line = ser.readline()
		line = line.split(',')
		if len(line) == 3 and line != [0,0,0]:
			line[0] = int(line[0])
			line[1] = int(line[1])
			line[2] = int(float(line[2][:6]))
			index = line[0]*line[1]%48
			volumes[index] = line[2]/400.0
		else:
			line = [0, 0, 0]
			for i in range (0 ,48):
				volumes[i] = 0
			
	except serial.SerialException:
		for i in range (0 ,48):
			volumes[i] = 0
		line = [0, 0, 0]
	
	#print line
	
	for i in range (0, 48):
		a = volumes[i]

		pygame.draw.aaline(screen, colors[i], (positions[i][0], positions[i][1]), (positions[i][0] + (a*(positions[i][0] - (screen_width/2))), positions[i][1] + (a*(positions[i][1]-(screen_height/2)))), 1)
		pygame.draw.circle(screen, GRAY, [screen_width/2, screen_height/2], 55, 0)

	pygame.display.update()

ser.close()
pygame.quit()



