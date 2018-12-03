#!/anaconda3/bin/python

import tensorflow as tf
import os
import pandas as pd
import numpy as np
import keras
import h5py
import matplotlib.pyplot as plt
import cv2
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt
import itertools
from keras.layers import *
from keras.preprocessing import image
from keras.layers.core import Dropout, Lambda
from keras.preprocessing.image import ImageDataGenerator
from keras.models import Sequential, Model
from keras.layers import Input, Conv2D, MaxPooling2D, Conv2DTranspose, Add, Flatten, UpSampling2D, BatchNormalization
from keras.layers import Dense, Activation, Flatten, Dropout, concatenate, Input
from keras.optimizers import Adam, Adadelta
from keras.callbacks import EarlyStopping, ModelCheckpoint
from keras.layers.advanced_activations import LeakyReLU
#from keras.layers.convolutional import Deconvolution2D
from keras.metrics import categorical_accuracy
from keras import optimizers

os.getcwd()
train = []

for file in os.listdir(os.path.join(os.getcwd(), 'RGB_data')):
    train.append([os.path.join(os.getcwd(), 'RGB_data', file),
                     os.path.join(os.getcwd(), 'Segmented_data', file)])
train = pd.DataFrame(train, columns=['Camera_Image', 'Segmented_Images'])

Augmented_data = []

for file in os.listdir(os.path.join(os.getcwd(), 'Augmented_RGB')):
    Augmented_data.append([os.path.join(os.getcwd(), 'Augmented_RGB', file),
                     os.path.join(os.getcwd(), 'Augmented_Seg', file)])
    
Augmented_data = pd.DataFrame(Augmented_data, columns=['Camera_Image', 'Segmented_Images'])

Train_data = [train, Augmented_data]
Train_data = pd.concat(Train_data)

RGB = []
Segmented= []

for i in range(len(Train_data)):
    img = cv2.imread(Train_data.iloc[i][0])
    img = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
    RGB.append(img)
    img = cv2.imread(Train_data.iloc[i][1])
    img = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
    Segmented.append(img)
    if i%999 == 0:
        print(i," images are done")
    if i==3000:
        break
RGB = np.array(RGB) #Camera RGB data
Segmented = np.array(Segmented) #Segmented data

del train
del Augmented_data
del Train_data



plt.imshow(Segmented[634])
plt.imshow(RGB[634])
print('Shape of RGB: ', RGB.shape, '  Shape of Segmented: ', Segmented.shape)

RGB, RGB_val, Segmented, Segmented_val = train_test_split(RGB, Segmented, test_size = 0.2, shuffle = True, random_state=125)
image_input=Input(shape=(360, 800, 3))


def GCN(c, k, ip):
    G_L1 = Conv2D(c, (k,1), padding='same')(ip)
    G_L2 = Conv2D(c, (1,k), padding='same')(G_L1)
    G_R1 = Conv2D(c, (1,k), padding='same')(ip)
    #print(G_R1.shape)
    G_R2 = Conv2D(c, (k,1), padding='same')(G_R1)
    #print(G_R1.shape)
    return Add()([G_L2, G_R2])


def BR(c, k, ip):
    B_L1 = Conv2D(c, (k,1), padding='same')(ip)
    B_L2 = BatchNormalization()(B_L1)
    B_L2 = LeakyReLU(alpha=0.15)(B_L2)
    B_L3 = Conv2D(c, (k,1), padding='same')(B_L2)
    return Add()([B_L3, ip])

def Conv_block(inp, f1, f2):
    x = Conv2D(f1, (3,3), padding = 'same')(inp)
    x = BatchNormalization()(x)
    x = LeakyReLU(alpha=0.3)(x)
    x = Conv2D(f2, (3,3), padding = 'same', activation= 'relu')(x)
    x = MaxPooling2D(pool_size=(2,2))(x)
    return x



#Input
ip = Input(shape=(360, 800, 3))

#Conv Block1
x1 = Conv_block(ip, 4, 10)
#print(x1)

#Conv Layer2
x2 = Conv_block(x1, 8, 15)
#print(x2)

#Conv Layer3
x3 = Conv_block(x2, 16, 20)
#print(x3)


#Conv Layer4
x4 = Conv_block(x3, 32, 30)
#print(x4)

#Conv Layer5
x5 = Conv2D(40, (3,3), padding = 'same')(x4)
x5 = BatchNormalization()(x5)
x5 = LeakyReLU(alpha=0.3)(x5)
x5 = Conv2D(50, (3,3), padding = 'same', activation= 'relu')(x5)

#print(x5)
x5 = GCN(30, 3, x5)
x5 = BR(30, 3, x5)


#Deconv5
#x5 = Deconvolution2D(30, 3, 3, output_shape= (24, 50,30), activation='relu')(x5)
#x5 = Conv2DTranspose(30, 3, strides=(2, 2), padding='valid')(x5)
x5 = BatchNormalization()(x5)
x5 = LeakyReLU(alpha=0.3)(x5)
#print(x5)

#Add 4 and 5
x5 = Add()([x5, x4])
x5 = GCN(25, 3, x5)
x5 = BR(25, 3, x5)

#print(x5)
#Deconv4
x5 = Conv2DTranspose(20, (3,2), strides=(2, 2), padding='valid')(x5)
x5 = BatchNormalization()(x5)
x5 = LeakyReLU(alpha=0.3)(x5)
#print(x5)

#Add 3 and 4
x5 = Add()([x5, x3])
x5 = GCN(25, 3, x5)
x5 = BR(25, 3, x5)


#Deconv3
#x5 = Deconvolution2D(15, 3, 3, output_shape= x2.output_shape, activation='relu')(x5)
x5 = Conv2DTranspose(15, (2,2), strides=(2, 2), padding='valid')(x5)
x5 = BatchNormalization()(x5)
x5 = LeakyReLU(alpha=0.3)(x5)

#Add 3 and 4
x5 = Add()([x5, x2])
x5 = GCN(20, 3, x5)
x5 = BR(20, 3, x5)


#Deconv2
#x5 = Deconvolution2D(10, 3, 3, output_shape= x1.output_shape, activation='relu')(x5)
x5 = Conv2DTranspose(10, (2,2), strides=(2, 2), padding='valid')(x5)
x5 = BatchNormalization()(x5)
x5 = LeakyReLU(alpha=0.3)(x5)

#Add 3 and 4
x5 = Add()([x5, x1])
x5 = GCN(15, 3, x5)
x5 = BR(15, 3, x5)


#Deconv1
#x5 = Deconvolution2D(3, 3, 3, output_shape= (-1, 3, 360,800), activation='relu')(x5)
x5 = Conv2DTranspose(3, (2,2), strides=(2, 2), padding='valid')(x5)
x5 = BatchNormalization()(x5)
x5 = LeakyReLU(alpha=0.3)(x5)
x5 = GCN(3, 3, x5)
x5 = BR(3, 3, x5)


model_new = Model(ip, x5)
model_new.summary()
 


Ndm = optimizers.Nadam(lr=0.002, beta_1=0.9, beta_2=0.999, epsilon=None, schedule_decay=0.004)
model_new.compile(optimizer='Adam', loss = 'mse', metrics = [categorical_accuracy])
history = model_new.fit(RGB, Segmented, epochs=40, batch_size=16, validation_split=0.2)
model_new.save('Lane_MOdel_U_GCN.h5')
model_yaml = model_new.to_yaml()
with open("Lane_MOdel_U_GCN_OnlyModel.yaml", "w") as yaml_file:
    yaml_file.write(model_yaml)
model_new.save_weights('Lane_MOdel_U_GCN_OnlyWeights.h5')


y_5 = model_new.predict(RGB[2:5])
added_image = cv2.addWeighted(RGB[2],1,y_5[0],1,0)

