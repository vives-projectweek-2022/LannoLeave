import numpy as np

def dist(x1,y1,x2,y2):
  d = np.sqrt((x2-x1)**2+(y2-y1)**2)
  return d

def intrepolate(image, x, y):
  vdist = np.vectorize(dist)

  im = np.array(image)

  nx, ny = im.shape[1], im.shape[0]
  xx = np.arange(0, nx, 1)
  yy = np.arange(0, ny, 1)

  newx = x
  newy = y

  vnewx = np.transpose(np.tile(newx,[len(xx),len(yy),1]),(2,1,0))
  vnewy = np.transpose(np.tile(newy,[len(xx),len(yy),1]),(2,1,0))
  vxx = np.tile(xx,[len(newx),len(yy),1])
  vyy = np.transpose(np.tile(yy,[len(newx),len(xx),1]),(0,2,1))
  distances = vdist(vnewx,vnewy,vxx,vyy)

  mind = np.argmin(distances, 0)

  imr = im[:,:,0]
  img = im[:,:,1]
  imb = im[:,:,2]

  newc = np.zeros((len(newx),3))
  for i in range(newc.shape[0]):
    newc[i,0] = np.mean(imr[np.where(mind==i)])
    newc[i,1] = np.mean(img[np.where(mind==i)])
    newc[i,2] = np.mean(imb[np.where(mind==i)])

  return newc
