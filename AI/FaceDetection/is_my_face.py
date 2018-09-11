import tensorflow as tf
import cv2
import dlib
import numpy as np
import os
import random
import sys
from sklearn.model_selection import train_test_split

size = 64

x = tf.placeholder(tf.float32, [None, size, size, 3])

keep_prob_5 = tf.placeholder(tf.float32)
keep_prob_75 = tf.placeholder(tf.float32)

def weightVariable(shape):
    init = tf.random_normal(shape, stddev=0.01)
    return tf.Variable(init)

def biasVariable(shape):
    init = tf.random_normal(shape)
    return tf.Variable(init)

def conv2d(x, W):
    return tf.nn.conv2d(x, W, strides=[1,1,1,1], padding='SAME')

def maxPool(x):
    return tf.nn.max_pool(x, ksize=[1,2,2,1], strides=[1,2,2,1], padding='SAME')

def dropout(x, keep):
    return tf.nn.dropout(x, keep)

def cnnLayer():
    # 第一层
    W1 = weightVariable([3,3,3,32]) # 卷积核大小(3,3)， 输入通道(3)， 输出通道(32)
    b1 = biasVariable([32])
    # 卷积
    conv1 = tf.nn.relu(conv2d(x, W1) + b1)
    # 池化
    pool1 = maxPool(conv1)
    # 减少过拟合，随机让某些权重不更新
    drop1 = dropout(pool1, keep_prob_5)

    # 第二层
    W2 = weightVariable([3,3,32,64])
    b2 = biasVariable([64])
    conv2 = tf.nn.relu(conv2d(drop1, W2) + b2)
    pool2 = maxPool(conv2)
    drop2 = dropout(pool2, keep_prob_5)

    # 第三层
    W3 = weightVariable([3,3,64,64])
    b3 = biasVariable([64])
    conv3 = tf.nn.relu(conv2d(drop2, W3) + b3)
    pool3 = maxPool(conv3)
    drop3 = dropout(pool3, keep_prob_5)

    # 全连接层
    Wf = weightVariable([8*16*32, 512])
    bf = biasVariable([512])
    drop3_flat = tf.reshape(drop3, [-1, 8*16*32])
    dense = tf.nn.relu(tf.matmul(drop3_flat, Wf) + bf)
    dropf = dropout(dense, keep_prob_75)

    # 输出层
    Wout = weightVariable([512,2])
    bout = biasVariable([2])
    out = tf.add(tf.matmul(dropf, Wout), bout)
    return out

output = cnnLayer()  
predict = tf.argmax(output, 1)  
   
saver = tf.train.Saver()  
sess = tf.Session()  
saver.restore(sess, tf.train.latest_checkpoint(r'E:\VS\QT\AI\x64\Release\FaceDetection\checkpoint'))  
   
def is_my_face(image):  
    res = sess.run(predict, feed_dict={x: [image/255.0], keep_prob_5:1.0, keep_prob_75: 1.0})  
    if res[0] == 1:  
        return True  
    else:  
        return False  

#使用dlib自带的frontal_face_detector作为我们的特征提取器
# detector = dlib.get_frontal_face_detector()
haar = cv2.CascadeClassifier(r'D:\soft\study\opencv\opencv\build\install\etc\haarcascades\haarcascade_frontalface_default.xml')
cam = cv2.VideoCapture(0)  

def detection_face():
	isDetect = True
	while isDetect:
		_, img = cam.read()
		gray_image = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
		faces = haar.detectMultiScale(gray_image, 1.3, 5)
		cv2.imshow('img', img)
		key = cv2.waitKey(30) & 0xff
		if key == 27:
			sys.exit(0)
				
		for f_x, f_y, f_w, f_h in faces:
			face = img[f_y:f_y+f_h, f_x:f_x+f_w]
			face = cv2.resize(face, (size,size))
			if is_my_face(face):
				isDetect = False
				break
			# cv2.rectangle(img, (x2,x1),(y2,y1), (255,0,0),3)
			# cv2.imshow('image',img)
			# key = cv2.waitKey(30) & 0xff
			# if key == 27:
				# sys.exit(0)
				
	sess.close() 
  
detection_face()