#!/usr/bin/env python

import numpy as np
import os
import six.moves.urllib as urllib
import sys
import tarfile
import tensorflow as tf
import zipfile

from collections import defaultdict
from io import StringIO
#from matplotlib import pyplot as plt
#from PIL import Image

import roslib; roslib.load_manifest('learning_image_transport')
import rospy
from std_msgs.msg import String , Header
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from vision_msgs.msg import Detection2D, Detection2DArray, ObjectHypothesisWithPose

import cv2

from utils import label_map_util
from utils import visualization_utils as vis_util

# What model to download.
MODEL_NAME = 'ssd_mobilenet_v1_coco_2017_11_17'
MODEL_FILE = MODEL_NAME + '.tar.gz'
DOWNLOAD_BASE = 'http://download.tensorflow.org/models/object_detection/'

# Path to frozen detection graph. This is the actual model that is used for the object detection.
MODEL_PATH = os.path.join(os.path.dirname(sys.path[0]), \
    'object_detection', MODEL_NAME)
#PATH_TO_CKPT = MODEL_NAME + '/frozen_inference_graph.pb'
PATH_TO_CKPT = MODEL_PATH + '/frozen_inference_graph.pb'

# List of the strings that is used to add correct label for each box.
LABEL_NAME = 'mscoco_label_map.pbtxt'
#PATH_TO_LABELS = os.path.join('data', 'mscoco_label_map.pbtxt')
PATH_TO_LABELS = os.path.join(os.path.dirname(sys.path[0]), \
    'object_detection', 'data', LABEL_NAME)

NUM_CLASSES = 90

detection_graph = tf.Graph()
with detection_graph.as_default():
  od_graph_def = tf.GraphDef()
  with tf.gfile.GFile(PATH_TO_CKPT, 'rb') as fid:
    serialized_graph = fid.read()
    od_graph_def.ParseFromString(serialized_graph)
    tf.import_graph_def(od_graph_def, name='')

label_map = label_map_util.load_labelmap(PATH_TO_LABELS)
categories = label_map_util.convert_label_map_to_categories( \
    label_map, max_num_classes=NUM_CLASSES, use_display_name=True)
category_index = label_map_util.create_category_index(categories)

def load_image_into_numpy_array(image):
  (im_width, im_height) = image.size
  return np.array(image.getdata()).reshape(
      (im_height, im_width, 3)).astype(np.uint8)

# # Detection

# Define the codec and create VideoWriter object
#fourcc = cv2.VideoWriter_fourcc(*'XVID')
#out = cv2.VideoWriter('output.avi',fourcc, 15.0, (640,480))
#counter = 1

# detect objects
with detection_graph.as_default():
  with tf.Session(graph=detection_graph) as sess:
    class image_converter:
      def __init__(self):
        # publishes the images after objects are detected
        self.image_pub = rospy.Publisher("image_topic_2", Image, queue_size=1)
        self.object_pub = \
            rospy.Publisher("objects", Detection2DArray, queue_size=1)
        self.bridge = CvBridge()
        self.image_sub = rospy.Subscriber("/csi_cam/image_raw", Image, \
            self.callback)
            #self.callback, queue_size=1)
    
      def callback(self,data):
        objArray = Detection2DArray()
        try:
          cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
        except CvBridgeError as e:
          print(e)
        # it seems that CvBridge already transforms the image into the correct
        # color format, and into a numpy array. So let's just feed the image 
        # directly into the detection graph
        image_np = cv_image
        # Expand dimensions since the model expects images to have shape: [1, None, None, 3]
        image_np_expanded = np.expand_dims(image_np, axis=0)
        image_tensor = detection_graph.get_tensor_by_name('image_tensor:0')
        # Each box represents a part of the image where a particular object was detected.
        boxes = detection_graph.get_tensor_by_name('detection_boxes:0')
        # Each score represent how level of confidence for each of the objects.
        # Score is shown on the result image, together with the class label.
        scores = detection_graph.get_tensor_by_name('detection_scores:0')
        classes = detection_graph.get_tensor_by_name('detection_classes:0')
        num_detections = detection_graph.get_tensor_by_name('num_detections:0')
        # Actual detection.
        (boxes, scores, classes, num_detections) = sess.run(
            [boxes, scores, classes, num_detections],
            feed_dict={image_tensor: image_np_expanded})
        # Visualization of the results of a detection.
        objects = vis_util.visualize_boxes_and_labels_on_image_array(
            image_np,
            np.squeeze(boxes),
            np.squeeze(classes).astype(np.int32),
            np.squeeze(scores),
            category_index,
            use_normalized_coordinates=True,
            line_thickness=6)

        objArray.detections =[]
        objArray.header=data.header
        object_count=1

        print("OBJECTS: ", len(objects))

        for i in range(len(objects)):
            object_count+=1
            objArray.detections.append(self.object_predict(objects[i],data.header,image_np,cv_image))

        self.object_pub.publish(objArray)

        #img=cv2.cvtColor(image_np, cv2.COLOR_BGR2RGB)
        img = image_np
        image_out = Image()
        try:
          image_out = self.bridge.cv2_to_imgmsg(img, "bgr8")
        except CvBridgeError as e:
          print(e)
        image_out.header = data.header
        self.image_pub.publish(image_out)


      def object_predict(self,object_data, header, image_np,image):
        image_height,image_width,channels = image.shape
        obj=Detection2D()
        obj_hypothesis= ObjectHypothesisWithPose()
         
#        print("OBJ_DATA LEN: ", len(object_data))

        object_id=object_data[0]
        object_score=object_data[1]
        dimensions=object_data[2]
        print("DIMENSIONS: ", len(dimensions))

        obj.header=header
        obj_hypothesis.id = object_id
        obj_hypothesis.score = object_score
        obj.results.append(obj_hypothesis)
        #obj.bbox.size_y = int((dimensions[2]-dimensions[0])*image_height)
#        obj.bbox.size_x = int((dimensions[3]-dimensions[1] )*image_width)
#        obj.bbox.center.x = int((dimensions[1] + dimensions [3])*image_height/2)
#        obj.bbox.center.y = int((dimensions[0] + dimensions[2])*image_width/2)
        return obj

def main(args):
    rospy.init_node('object_detection_node', anonymous=False)
    ic = image_converter()
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
    #cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)

