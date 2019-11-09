from flask import Flask, request
from pynput.mouse import Button,Controller
import pygame

moveRange = 50
def move(pY,pX,nY,nX):
    difX = (nY-pY)
    difY = (nX-pX)
    if(0 < difX < 0.1):    difX = 0
    if(0 < difY < 0.1):    difY = 0
    mouse.move(difX,difY)

def click(LC,RC):
    if(LC):
        mouse.click(Button.left, 1)
        pygame.mixer.init()
        pygame.mixer.music.load("Click.wav")
        pygame.mixer.music.play()
    
    if(RC):
        mouse.click(Button.right, 1)
        pygame.mixer.init()
        pygame.mixer.music.load("Click.wav")
        pygame.mixer.music.play()

def warning():
    pygame.mixer.init()
    pygame.mixer.music.load("warning.wav")
    pygame.mixer.music.play()

app = Flask(__name__)
mouse = Controller()

@app.route("/")
def main():
    prevAccX = int(request.args.get('pX'))
    prevAccY = int(request.args.get('pY'))
    nowAccX = int(request.args.get('nX'))
    nowAccY = int(request.args.get('nY'))
    LC = request.args.get('LC')
    RC = request.args.get('RC')
    Warn = request.args.get('Wn')
    if(LC == 'true'):
        leftClick = True
    else:
        leftClick = False
    if(RC == 'true'):
        rightClick = True
    else:
        rightClick = False
    if(Warn == 'true'):
        warning();

    move(prevAccY,prevAccX,nowAccY,nowAccX)
    click(leftClick,rightClick)
   
    
    return 'success'

if __name__ == "__main__":
    app.run(debug = True, host="0.0.0.0", port = 80)

