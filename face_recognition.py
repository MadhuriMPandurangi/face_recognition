import cv2
import os
import time
import numpy as np
import time
import datetime
import serial
import numpy as np
import csv
from os.path import exists as file_exists
from csv import writer

data = serial.Serial(
                'COM6',
                baudrate = 9600,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                bytesize=serial.EIGHTBITS,
                timeout=1
                )

# Create Local Binary Patterns Histograms for face recognization
#recognizer = cv2.face.createLBPHFaceRecognizer()
recognizer = cv2.face_LBPHFaceRecognizer.create()
#recognizer = cv2.face.LBPHFaceRecognizer_create()

# Load the trained mode
recognizer.read('trainer/trainer.yml')

# Load prebuilt model for Frontal Face
cascadePath = "haarcascade_frontalface_default.xml"

# Create classifier from prebuilt model
faceCascade = cv2.CascadeClassifier(cascadePath);

# Set the font style
font = cv2.FONT_HERSHEY_SIMPLEX

# Initialize and start the video frame capture
cam = cv2.VideoCapture(0)

def Send():
        data.write(str.encode('A'))
        print('Data sent......')
        
def Read():
        while True:
            print('waiting.....')
            db=data.read(15)
            db=db.decode('UTF-8', 'ignore')
            if len(db.strip()) > 0:
                print(db)
                break

        db=db.split(',')
        row = (db[0], db[1], db[2])
        filename = Id1+'.csv'
        P=file_exists(filename)
        if P == False:
            fields = ['SpO2', 'HeartBeat', 'Temperature']
            with open(filename, 'w', newline='' ) as csvfile:
                csvwriter = csv.writer(csvfile)
                csvwriter.writerow(fields)
                
        with open(filename, 'a+', newline='' ) as csvfile:
                csvwriter = csv.writer(csvfile)
                csvwriter.writerow(row)
        print('SpO2: {}, HeartBeat: {}, Temperature: {}'.format(db[0], db[1], db[2]))
        print('Successfully Updated')
        
count = 0

while True:
        now = datetime.datetime.now()

        # Read the video frame
        ret, im =cam.read()

        # Convert the captured frame into grayscale
        gray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)

        # Get all face from the video frame
        faces = faceCascade.detectMultiScale(gray, 1.2,5)

        # For each face in faces
        for(x,y,w,h) in faces:

            # Create rectangle around the face
            cv2.rectangle(im, (x-20,y-20), (x+w+20,y+h+20), (0,255,0), 4)

            # Recognize the face belongs to which ID
            Id,i = recognizer.predict(gray[y:y+h,x:x+w])
            #id = int(os.path.split(imagePath)[-1].split(".")[1])
            print(Id)
            print(i)
            Id1=''
            # Check the ID if exist
            if i < 65:
                if Id == 1 :
                        Id1 = 'Harshitha'
                        print(Id1)
                if Id == 2 :
                        Id1 = 'Madhuri'
                        print(Id1)
            else:
                # count=count+1

                # if count > 10:
                    # count=0
                Id1 = 'unknown'
                print(Id1)

            # Put text describe who is in the picture
            cv2.rectangle(im, (x-22,y-90), (x+w+22, y-22), (0,255,0), -1)
            cv2.putText(im, str(Id1), (x,y-40), font, 2, (255,255,255), 3)
            # Display the video frame with the bounded rectangle
        cv2.imshow('im',im)
        # If 'q' is pressed, close program
        if cv2.waitKey(2) & 0xFF == ord('q'): #if cv2.waitKey(10) & 0xFF == ord('q'):
            break

#stop camera     
cam.release()
# Close all windows
cv2.destroyAllWindows()
Send()
Read()
