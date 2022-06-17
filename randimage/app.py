import numpy
from PIL import Image
import time


while True:
    imarray = numpy.random.rand(300,300,3) * 255
    im = Image.fromarray(imarray.astype('uint8')).convert('RGB')
    im.save('image.jpg')
    time.sleep(0.1)